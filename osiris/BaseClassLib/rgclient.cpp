/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: rgclient.cpp
*  Author:   Robert Goor
*
*/
//
//  Source file for client operations in high level IPC.  Uses shared memory and Windows Win32 Events to coordinate
// between client and server.  Implements Send/Receive/Reply paradigm for blocking/non-blocking control and message
// passing.
//
//  RGServerPort is a class that maintains communication with a particular server.  RGClient is a class that performs all
// of the client functions.  To Send to a server requires that an RGClient object work through an RGServerPort object.
// Each RGClient object maintains a list of servers with which it has communicated.
//


#include "rgclient.h"
#include "rgcommdefs.h"


int RGClient::ReplyBufferSize = 10240;


RGServerPort :: RGServerPort () : OKToReadHandle (0), OKToWriteHandle (0), ContinuationHandle (0), ImTryingToContactYouHandle (0),
ServerPidHandle (0), BufferHandle (0), HotLineHandle (0), LastError (RGOK), FirstTime (TRUE), Connected (FALSE), ClientId (0),
Parent (NULL) {}


RGServerPort :: RGServerPort (const RGString& name) : Name (name), OKToReadHandle (0), OKToWriteHandle (0), ContinuationHandle (0), 
ImTryingToContactYouHandle (0), ServerPidHandle (0), BufferHandle (0), HotLineHandle (0), LastError (RGOK), 
FirstTime (TRUE), Connected (FALSE), ClientId (0), Parent (NULL) {}


RGServerPort :: ~RGServerPort () {

	if (Connected) {

		UnmapViewOfFile (BufferMapAddress);
		CloseHandle (OKToReadHandle);
		CloseHandle (OKToWriteHandle);
		CloseHandle (ContinuationHandle);
		CloseHandle (ImTryingToContactYouHandle);
		CloseHandle (BufferHandle);
		CloseHandle (ServerPidHandle);
		CloseHandle (HotLineHandle);
	}
}


int RGServerPort :: Connect (const RGString& name, RGClient* parent) {

	Name = name;
	return Connect (parent);
}


int RGServerPort :: Connect (RGClient* parent) {

	if (Connected) {

		LastError = RGALREADYCONNECTED;
		return -1;
	}

	if (parent == NULL) {

		LastError = RGCLIENTNULL;
		return -1;
	}
	
	Parent = parent;

	//
	//  Name is the name of the server.  To get the client name, use Parent.
	//

	RGString OKToReadName = Name + "OKToReadReceiveBuffer";
	RGString OKToWriteName = Name + "OKToWriteIntoReceiveBuffer";
	RGString ContinuationName = Name + "ReceiveContinuation";
	RGString ImTryingToContactYouName = Name + "ImTryingToContactYou";
	RGString BufferName = Name + "ReceiveBuffer";
	RGString To ("To");
	RGString HotLineName = "HotLineFrom" + Name + To + Parent->GetName ();

	FirstTime = TRUE;
	LastError = RGOK;

//	BufferHandle = OpenFileMapping (PAGE_READWRITE, FALSE, BufferName);

	BufferHandle = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                4, BufferName);
	
	if (BufferHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}
	
	BufferMapAddress = (unsigned char*)MapViewOfFile (BufferHandle, 
		FILE_MAP_WRITE, 0, 0, 0);
	
	if (BufferMapAddress == NULL) {
		
		cout << "Map Error = " << GetLastError () << endl;
		LastError = RGNOBUFFER;
		return -1;
	}

	int LongLength = sizeof (unsigned long);

	unsigned long* size = (unsigned long*) BufferMapAddress;
	Type = (unsigned long*) (BufferMapAddress + LongLength);
	Length = (unsigned long*) (BufferMapAddress + 2 * LongLength);
	ReplyPointer = (unsigned long*) (BufferMapAddress + 3 * LongLength);
	ServerPid = (unsigned long*) (BufferMapAddress + 4 * LongLength);
	BufferSize = *size;
	DataBegin = BufferMapAddress + DATAMAPOFFSET;
	
	OKToReadHandle = OpenEvent (EVENT_ALL_ACCESS, FALSE, OKToReadName);
	
	if (OKToReadHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	OKToWriteHandle = OpenEvent (EVENT_ALL_ACCESS, FALSE, OKToWriteName);
	
	if (OKToWriteHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	ContinuationHandle = OpenEvent (EVENT_ALL_ACCESS, FALSE, ContinuationName);
	
	if (ContinuationHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	ImTryingToContactYouHandle = OpenSemaphore (SEMAPHORE_ALL_ACCESS, FALSE, ImTryingToContactYouName);
	
	if (ImTryingToContactYouHandle == NULL) {
		
		cout << "Windows error:  " << GetLastError () << endl;
		LastError = RGNOHANDLE;
		return -1;
	}

	ServerPidHandle = OpenProcess (SYNCHRONIZE, FALSE, *ServerPid);  // SYNCHRONIZE	required by "wait" functions

	if (ServerPidHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	HotLineHandle = CreateEvent (NULL, FALSE, FALSE, HotLineName);
	
	if (HotLineHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}
	
	Connected = TRUE;
	return 0;
}


int RGServerPort :: Disconnect () {

	if (Connected) {

		UnmapViewOfFile (BufferMapAddress);
		CloseHandle (OKToReadHandle);
		CloseHandle (OKToWriteHandle);
		CloseHandle (ContinuationHandle);
		CloseHandle (ImTryingToContactYouHandle);
		CloseHandle (BufferHandle);
		CloseHandle (ServerPidHandle);
		CloseHandle (HotLineHandle);
		OKToReadHandle = NULL;
		OKToWriteHandle = NULL;
		ContinuationHandle = NULL;
		ImTryingToContactYouHandle = NULL;
		BufferHandle = NULL;
		ServerPidHandle = NULL;
		HotLineHandle = NULL;
		Type = NULL;
		Length = NULL;
		ReplyPointer = NULL;
		ServerPid = NULL;
		BufferSize = 0;
		DataBegin = NULL;
		FirstTime = TRUE;
		ClientId = 0;
		Parent = NULL;
		Connected = FALSE;
	}

	return 0;
}


int RGServerPort :: Send (int len, unsigned char* data) {

	if (!Connected) {
		
		LastError = RGCLIENTNOTCONNECTEDTOSERVER;
		return -1;
	}

	HANDLE HandleArray [3];
	HandleArray [0] = ServerPidHandle;
	HandleArray [1] = OKToWriteHandle;
	HandleArray [2] = HotLineHandle;
	int status;

	if (FirstTime) {

		//
		//  Release ImTryingToContactYouHandle semaphore on first contact only
		//

		if (!ReleaseSemaphore (ImTryingToContactYouHandle, 1, NULL)) {

			LastError = RGSERVERTERMINATED;
			return -1;
		}
	}

	status = WaitForSendAccess (HandleArray);
	HandleArray [2] = NULL;
	Parent->ClearServerPid ();

	if (status < 0)
		return status;

	if (FirstTime) {

		*Type = RGFIRSTSEND;
		*ReplyPointer = ClientId;

		//
		// copy client "name" into data.  RGClient has it...get it from parent.  Set length to name length + 1.
		//

		strcpy ((char*)DataBegin, Parent->GetName ());
		*Length = Parent->GetNameLength ();

		SetEvent (OKToReadHandle);
		HandleArray [1] = ContinuationHandle;
		status = WaitForWriteAccess (HandleArray);  // Checks for continuation handle this time

		if (status < 0)
			return status;
		
		ClientId = *ReplyPointer;
		FirstTime = FALSE;
	}

	//
	// Test for continuation
	//

	if (len <= BufferSize) {
		
		//
		// No continuation needed
		//

		*Type = RGBLOCKINGSEND;
		*Length = len;
		*ReplyPointer = ClientId;

		memcpy (DataBegin, data, len);
		SetEvent (OKToReadHandle);
		return 0;
	}

	//
	// Need continuation
	//

	Boolean Loop = TRUE;
	unsigned char* CurrentDataPointer = data;
	int WriteSize = BufferSize;
	unsigned long CurrentType = RGBLOCKINGSENDWITHCONTINUATION;
	int LeftToWrite = len;
	Boolean Wait = FALSE;

	while (Loop) {
		
		if (Wait) {
		
			status = WaitForWriteAccess (HandleArray);  // wait for continuation clearance

			if (status < 0)
				return status;
		}

		Wait = TRUE;
		*Type = CurrentType;
		*Length = len;  // set to WriteSize?
		*ReplyPointer = ClientId;
		memcpy (DataBegin, CurrentDataPointer, WriteSize);
		CurrentDataPointer += BufferSize;
		LeftToWrite -= BufferSize;
		SetEvent (OKToReadHandle);
		HandleArray [1] = ContinuationHandle;

		if (LeftToWrite <= 0)
			Loop = FALSE;

		else if (LeftToWrite <= BufferSize) {
			
			CurrentType = RGBLOCKINGSEND;
			WriteSize = LeftToWrite;
		}
	}  // end of while loop

	return 0;
}


unsigned long RGServerPort :: GetServerPid () const {

	if (Connected)
		return *ServerPid;

	return 0;
}


Boolean RGServerPort :: isEqual (const RGServerPort* port) {

	return Name == (port->Name);
}



int RGServerPort :: WaitForWriteAccess (HANDLE* HandleArray) {
	
	DWORD WaitStatus;

	//
	// wait for first object
	//

	WaitStatus = WaitForMultipleObjects (2, HandleArray, FALSE, INFINITE);

	if (WaitStatus == WAIT_FAILED) {
		
		LastError = RGWAITFAILED;
		return -1;
	}

	if ((WaitStatus == WAIT_ABANDONED_0) || (WaitStatus == WAIT_OBJECT_0)) {
		
		LastError = RGSERVERTERMINATED;
		return -1;
	}

	return 0;
}


int RGServerPort :: WaitForSendAccess (HANDLE* HandleArray) {
	
	DWORD WaitStatus;

	//
	//  Wait for first object or second.  First is general OKToWrite.  Second is specific HotLine from server
	//  to this client.
	//

	WaitStatus = WaitForMultipleObjects (3, HandleArray, FALSE, INFINITE);

	if (WaitStatus == WAIT_FAILED) {
		
		LastError = RGWAITFAILED;
		return -1;
	}

	if ((WaitStatus == WAIT_ABANDONED_0) || (WaitStatus == WAIT_OBJECT_0)) {
		
		LastError = RGSERVERTERMINATED;
		return -1;
	}

	return 0;
}


Boolean operator==(const RGServerPort& sp1, const RGServerPort& sp2) {

	return sp1.Name == sp2.Name;
}


RGClient :: RGClient (const RGString& name) : NextServerId (1), Name (name), OKToReadHandle (0), OKToWriteHandle (0), 
ContinuationHandle (0), BufferSize (0),	BufferHandle (0), LastError (RGOK), Initialized (FALSE) {

	Initialize ();
}


RGClient :: ~RGClient () {

	UnmapViewOfFile (BufferMapAddress);
	CloseHandle (OKToReadHandle);
	CloseHandle (OKToWriteHandle);
	CloseHandle (ContinuationHandle);
	CloseHandle (BufferHandle);
	ServerPortList.ClearAndDestroy ();
	UnusedServerIds.clearAndDestroy ();
}



void RGClient :: ResetReplyBufferSize (int newSize) {

	if (newSize > 0)
		ReplyBufferSize = newSize;
}


int RGClient :: Initialize () {

	RGString OKToReadName = Name + "OKToReadReply";
	RGString OKToWriteName = Name + "OKToWriteReply";
	RGString ContinuationName = Name + "ReplyContinuation";
	RGString BufferName = Name + "ReplyBuffer";
	Initialized = FALSE;

	LastError = RGOK;
	int TotalSize = RGClient::ReplyBufferSize + DATAMAPOFFSET;

	HANDLE temp = OpenEvent (EVENT_ALL_ACCESS, FALSE, OKToReadName);

	if (temp != NULL) {

		CloseHandle (temp);
		LastError = RGCLIENTNAMEALREADYEXISTS;
		return -1;
	}
	
	BufferHandle = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                TotalSize, BufferName);
	
	if (BufferHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	BufferMapAddress = (unsigned char*)MapViewOfFile (BufferHandle, 
		FILE_MAP_ALL_ACCESS, 0, 0, 0);

//	CloseHandle (BufferHandle);
	
	if (BufferMapAddress == NULL) {
		
		LastError = RGNOBUFFER;
		return -1;
	}

	int LongLength = sizeof (unsigned long);

	unsigned long* size = (unsigned long*) BufferMapAddress;
	Type = (unsigned long*) (BufferMapAddress + LongLength);
	Length = (unsigned long*) (BufferMapAddress + 2 * LongLength);
	ServerPid = (unsigned long*) (BufferMapAddress + 3 * LongLength);
	MyPid = (unsigned long*) (BufferMapAddress + 4 * LongLength);
	BufferSize = *size = RGClient::ReplyBufferSize;
	DataBegin = BufferMapAddress + DATAMAPOFFSET;
	*ServerPid = 0;
	*MyPid = GetCurrentProcessId ();
	
	OKToReadHandle = CreateEvent (NULL, FALSE, FALSE, OKToReadName);
	
	if (OKToReadHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	OKToWriteHandle = CreateEvent (NULL, FALSE, FALSE, OKToWriteName);
	
	if (OKToWriteHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}
	
	ContinuationHandle = CreateEvent (NULL, FALSE, FALSE, ContinuationName);
	
	if (ContinuationHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	SetEvent (OKToWriteHandle);
	Initialized = TRUE;
	return 0;
}


int RGClient :: LocateServer (const RGString& name) {
	
	// use the output of this method as server id to call Send.  This is local only

	RGServerPort* port = new RGServerPort (name);
	int status = port->Connect (this);
	int id;
	int* first;

	if (status < 0) {

		LastError = port->GetLastError ();
		delete port;
		return -1;
	}

	first = UnusedServerIds.RemoveFirst ();
	
	if (first != NULL) {
		
		id = *first;
		delete first;
	}

	else {

		id = NextServerId;
		NextServerId++;
	}

	ServerPortList.ReplaceElementAt (id, port);
	return id;
}


int RGClient :: Send (int serverId, unsigned char* sendBuffer, unsigned char*& replyBuffer, int sendLength, int& replyMax) {

	int len;
	LastError = RGOK;
	
	if (serverId <= 0) {

		LastError = RGILLEGALSERVERID;
		return -1;
	}
	
	RGServerPort* port = ServerPortList.GetElementAt (serverId);

	if (port == NULL) {

		LastError = RGSERVERNULL;
		return -1;
	}

	*ServerPid = port->GetServerPid ();
	int status = port->Send (sendLength, sendBuffer);

	if (status < 0) {

		LastError = port->GetLastError ();
		RemoveServer (serverId);
		return -1;
	}

	HANDLE HandleArray [2];
	HandleArray [0] = port->GetServerHandle ();
	HandleArray [1] = OKToReadHandle;

	status = WaitForReadAccess (HandleArray);

	if (status < 0) {

		RemoveServer (serverId);
		return -1;
	}

	len = *Length;

	//
	// Test for buffer overflow...we don't like to encourage this!
	//

	if (len > replyMax) {

		delete[] replyBuffer;
		replyBuffer = new unsigned char [len];
		replyMax = len;
	}

	//
	// Test for continuation
	//

	if (*Type == RGBLOCKINGSEND) {
		
		//
		// No continuation needed
		//	

		memcpy (replyBuffer, DataBegin, len);
		return 0;
	}

	//
	// Need continuation
	//

	Boolean Loop = TRUE;
	unsigned char* CurrentOutputPointer = replyBuffer;
	int WriteSize = BufferSize;
	int LeftToWrite = len;
	Boolean Wait = FALSE;

	while (Loop) {
		
		if (Wait) {
		
			status = WaitForReadAccess (HandleArray);  // wait for continuation clearance

			if (status < 0)
				return status;
		}

		Wait = TRUE;
		memcpy (CurrentOutputPointer, DataBegin, WriteSize);
		CurrentOutputPointer += BufferSize;
		LeftToWrite -= BufferSize;
		HandleArray [1] = ContinuationHandle;

		if ((LeftToWrite <= 0) || (*Type == RGBLOCKINGSEND))
			Loop = FALSE;

		else {

			SetEvent (OKToWriteHandle);
		
			if (LeftToWrite <= BufferSize)
				WriteSize = LeftToWrite;
		}
	}  // end of while loop

	return 0;
}


void RGClient :: RemoveServer (int id) {

	RGServerPort* port = ServerPortList.RemoveElementAt (id);
	delete port;
	int* newInt = new int;
	*newInt = id;
	UnusedServerIds.append (newInt);
}


void RGClient :: ClearServerPid () const {

	*ServerPid = 0;
}


int RGClient :: WaitForReadAccess (HANDLE* HandleArray) {

	DWORD WaitStatus;

	//
	// wait for first object
	//

	WaitStatus = WaitForMultipleObjects (2, HandleArray, FALSE, INFINITE);

	if (WaitStatus == WAIT_FAILED) {
		
		LastError = RGWAITFAILED;
		return -1;
	}

	if ((WaitStatus == WAIT_ABANDONED_0) || (WaitStatus == WAIT_OBJECT_0)) {
		
		LastError = RGSERVERTERMINATED;
		return -1;
	}

	return 0;
}
