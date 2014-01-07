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
*  FileName: rgserver.cpp
*  Author:   Robert Goor
*
*/
//
//  Source file for server operations in high level IPC.  Uses shared memory and Windows Win32 Events to coordinate
// between client and server.  Implements Send/Receive/Reply paradigm for blocking/non-blocking control and message
// passing.
//
//  RGClientPort is a class that maintains communication with a particular client.  RGServer is a class that performs all
// of the server functions.  To Receive from and Reply to a client requires that an RGServer object work through an RGClientPort object.
// Each RGServer object maintains a list of clients with which it has communicated.
//

#include "rgserver.h"
#include "rgcommdefs.h"


int RGServer::ReceiveBufferSize = 10240;


RGClientPort :: RGClientPort () : OKToReadHandle (0), OKToWriteHandle (0), ContinuationHandle (0), 
ClientPidHandle (0), HotLineHandle (0), BufferHandle (0), LastError (RGOK), FirstTime (TRUE), Connected (FALSE), ServerId (0),
Parent (NULL) {}



RGClientPort :: RGClientPort (const RGString& name) : Name (name), OKToReadHandle (0), OKToWriteHandle (0), ContinuationHandle (0), 
ClientPidHandle (0), HotLineHandle (0), BufferHandle (0), LastError (RGOK), FirstTime (TRUE), Connected (FALSE), ServerId (0),
Parent (NULL) {}



RGClientPort :: ~RGClientPort () {

	if (Connected) {

		UnmapViewOfFile (BufferMapAddress);
		CloseHandle (OKToReadHandle);
		CloseHandle (OKToWriteHandle);
		CloseHandle (ContinuationHandle);
		CloseHandle (BufferHandle);
		CloseHandle (ClientPidHandle);
		CloseHandle (HotLineHandle);
	}
}

int RGClientPort :: Connect (const RGString& name, RGServer* parent) {

	Name = name;
	return Connect (parent);
}


int RGClientPort :: Connect (RGServer* parent) {

	if (Connected) {

		LastError = RGALREADYCONNECTED;
		return -1;
	}

	if (parent == NULL) {

		LastError = RGSERVERNULL;
		return -1;
	}
	
	Parent = parent;

	//
	//  Name is the name of the client.  To get server name, use Parent.

	RGString OKToReadName = Name + "OKToReadReply";
	RGString OKToWriteName = Name + "OKToWriteReply";
	RGString ContinuationName = Name + "ReplyContinuation";
	RGString BufferName = Name + "ReplyBuffer";
	RGString To ("To");
	RGString Hot ("HotLineFrom");
	RGString HotLineName = Hot + Parent->GetName () + To + Name;

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
		FILE_MAP_ALL_ACCESS, 0, 0, 0);
	
	if (BufferMapAddress == NULL) {
		
		LastError = RGNOBUFFER;
		return -1;
	}

	int LongLength = sizeof (unsigned long);

	unsigned long* size = (unsigned long*) BufferMapAddress;
	Type = (unsigned long*) (BufferMapAddress + LongLength);
	Length = (unsigned long*) (BufferMapAddress + 2 * LongLength);
	ServerPid = (unsigned long*) (BufferMapAddress + 3 * LongLength);
	ClientPid = (unsigned long*) (BufferMapAddress + 4 * LongLength);
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

	ClientPidHandle = OpenProcess (SYNCHRONIZE, FALSE, *ClientPid);  // SYNCHRONIZE	required by "wait" functions

	if (ClientPidHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	HotLineHandle = OpenEvent (EVENT_ALL_ACCESS, FALSE, HotLineName);
	
	if (HotLineHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}
	
	Connected = TRUE;
	return 0;
}



int RGClientPort :: Disconnect () {

	if (Connected) {

		UnmapViewOfFile (BufferMapAddress);
		CloseHandle (OKToReadHandle);
		CloseHandle (OKToWriteHandle);
		CloseHandle (ContinuationHandle);
		CloseHandle (BufferHandle);
		CloseHandle (ClientPidHandle);
		CloseHandle (HotLineHandle);
		OKToReadHandle = NULL;
		OKToWriteHandle = NULL;
		ContinuationHandle = NULL;
		HotLineHandle = NULL;
		BufferHandle = NULL;
		ClientPidHandle = NULL;
		Type = NULL;
		Length = NULL;
		ClientPid = NULL;
		ServerPid = NULL;
		BufferSize = 0;
		DataBegin = NULL;
		FirstTime = TRUE;
		ServerId = 0;
		Parent = NULL;
		Connected = FALSE;
	}

	return 0;
}


int RGClientPort :: Reply (int len, unsigned char* data) {

	if (!Connected) {
		
		LastError = RGSERVERNOTCONNECTEDTOCLIENT;
		return -1;
	}

	int status;

	//
	// Test for continuation
	//

	if (len <= BufferSize) {
		
		//
		// No continuation needed
		//

		*Type = RGBLOCKINGSEND;
		*Length = len;

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
	HANDLE HandleArray [2];
	HandleArray [0] = ClientPidHandle;
	HandleArray [1] = ContinuationHandle;

	while (Loop) {
		
		if (Wait) {
		
			status = WaitForWriteAccess (HandleArray);  // wait for continuation clearance

			if (status < 0)
				return status;
		}

		Wait = TRUE;
		*Type = CurrentType;
		*Length = len;  // set to WriteSize?
		memcpy (DataBegin, CurrentDataPointer, WriteSize);
		CurrentDataPointer += BufferSize;
		LeftToWrite -= BufferSize;
		SetEvent (OKToReadHandle);

		if (LeftToWrite <= 0)
			Loop = FALSE;

		else if (LeftToWrite <= BufferSize) {
			
			CurrentType = RGBLOCKINGSEND;
			WriteSize = LeftToWrite;
		}
	}  // end of while loop

	return 0;
}


unsigned long RGClientPort :: GetClientPid () const {

	if (Connected)
		return *ClientPid;

	return 0;
}


unsigned long RGClientPort :: GetPidOfBlockingServer () const {

	if (Connected)
		return *ServerPid;

	return 0;
}


Boolean RGClientPort :: isEqual (const RGClientPort* port) {

	return Name == (port->Name);
}

int RGClientPort :: WaitForReadAccessFromClient (HANDLE specificHandle) {

	// when receiving from a specific client, it would be OK to read receiveBuffer
	HANDLE HandleArray [2];

	HandleArray [1] = specificHandle;  // Put specific client/server handle here!!!!!!!
	HandleArray [0] = ClientPidHandle;
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
		
		LastError = RGCLIENTTERMINATED;
		return -1;
	}

	return 0;
}


void RGClientPort :: TriggerHotLine () {

	SetEvent (HotLineHandle);
}


int RGClientPort :: WaitForWriteAccess (HANDLE* HandleArray) {
	
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

Boolean operator==(const RGClientPort& sp1, const RGClientPort& sp2) {

	return sp1.Name == sp2.Name;
}


RGServer :: RGServer (const RGString& name) : Name (name), NextClientId (1), OKToReadHandle (0), OKToWriteHandle (0),
ContinuationHandle (0), ImTryingToContactYouHandle (0), BufferHandle (0), LastError (RGOK), Initialized (FALSE), MyOwnPid (0) {

	RegisterName ();
}


RGServer :: ~RGServer () {

	if (Initialized) {

		UnmapViewOfFile (BufferMapAddress);
		CloseHandle (OKToReadHandle);
		CloseHandle (OKToWriteHandle);
		CloseHandle (ContinuationHandle);
		CloseHandle (ImTryingToContactYouHandle);
		CloseHandle (BufferHandle);
		ClientPortList.ClearAndDestroy ();
		UnusedClientIds.clearAndDestroy ();
	}
}

void RGServer :: ResetReceiveBufferSize (int newSize) {

	if (newSize > 0)
		ReceiveBufferSize = newSize;
}

int RGServer :: RegisterName () {

	RGString OKToReadName = Name + "OKToReadReceiveBuffer";
	RGString OKToWriteName = Name + "OKToWriteIntoReceiveBuffer";
	RGString ContinuationName = Name + "ReceiveContinuation";
	RGString ImTryingToContactYouName = Name + "ImTryingToContactYou";
	RGString BufferName = Name + "ReceiveBuffer";
	Initialized = FALSE;

	LastError = RGOK;
	int TotalSize = RGServer::ReceiveBufferSize + DATAMAPOFFSET;

	HANDLE temp = OpenEvent (EVENT_ALL_ACCESS, FALSE, OKToReadName);

	if (temp != NULL) {

		CloseHandle (temp);
		LastError = RGSERVERNAMEALREADYEXISTS;
		return -1;
	}

	BufferHandle = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                TotalSize, BufferName);
	
	if (BufferHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	BufferMapAddress = (unsigned char*)MapViewOfFile (BufferHandle, 
		FILE_MAP_WRITE, 0, 0, 0);
	
	if (BufferMapAddress == NULL) {
		
		LastError = RGNOBUFFER;
		return -1;
	}

	int LongLength = sizeof (unsigned long);

	unsigned long* size = (unsigned long*) BufferMapAddress;
	Type = (unsigned long*) (BufferMapAddress + LongLength);
	Length = (unsigned long*) (BufferMapAddress + 2 * LongLength);
	ReplyPointer = (unsigned long*) (BufferMapAddress + 3 * LongLength);  // to ID client sender
	ServerPid = (unsigned long*) (BufferMapAddress + 4 * LongLength);  // to ID self
	BufferSize = *size = RGServer::ReceiveBufferSize;
	DataBegin = BufferMapAddress + DATAMAPOFFSET;

	MyOwnPid = *ServerPid = GetCurrentProcessId ();
	
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

	int MaxCount = 0x7FFFFFFF;

	ImTryingToContactYouHandle = CreateSemaphore (NULL, 0, MaxCount, ImTryingToContactYouName);
	
	if (ImTryingToContactYouHandle == NULL) {
		
		LastError = RGNOHANDLE;
		return -1;
	}

	Initialized = TRUE;
	return 0;
}


int RGServer :: Receive (int clientId, unsigned char*& receiveBuffer, int& receiveLength) {

	//
	//  returns Id of client
	//
	
	DWORD WaitStatus;
	int Id;
	char* CClientName;
	RGString ClientName;
	RGClientPort* port;
	int status;
	int* first;
	unsigned long len;
	LastError = RGOK;
	HANDLE HandleArray [2];
	
	if (clientId == 0) {

		SetEvent (OKToWriteHandle);
		WaitStatus = WaitForSingleObject (OKToReadHandle, INFINITE);

		if (WaitStatus != WAIT_OBJECT_0) {

			LastError = RGWAITFAILED;
			return -1;
		}

		//
		//  Test for first send
		//

		if (*Type == RGFIRSTSEND) {

			//
			// Client releases ImTryingToContactYouHandle semaphore on first send only - clear it
			//

			WaitForSingleObject (ImTryingToContactYouHandle, 0);
			CClientName = new char [*Length + 5];
			strcpy (CClientName, (char*) DataBegin);
			ClientName = CClientName;
			delete[] CClientName;
			port = new RGClientPort (ClientName);
			status = port->Connect (this);

			if (status < 0) {

				LastError = port->GetLastError ();
				delete port;
				return -1;
			}

			//
			// Add port to client list
			//

			first = UnusedClientIds.RemoveFirst ();
	
			if (first != NULL) {
				
				Id = *first;
				delete first;
			}

			else {

				Id = NextClientId;
				NextClientId++;
			}

			ClientPortList.ReplaceElementAt (Id, port);
			*ReplyPointer = Id;

			//
			// send back to client and await actual message
			//

			SetEvent (ContinuationHandle);
			status = port->WaitForReadAccessFromClient (OKToReadHandle);

			if (status < 0) {

				LastError = port->GetLastError ();
				RemoveClient (Id);
				return -1;
			}
		}		

		//
		//  get clientID for return value
		//

		Id = *ReplyPointer;
		len = *Length;

		//
		// Test for buffer overflow...we don't like to encourage this!
		//

		if (len > receiveLength) {

			delete[] receiveBuffer;
			receiveBuffer = new unsigned char [len];
			receiveLength = len;
		}

		//
		// Test for continuation
		//

		if (*Type == RGBLOCKINGSEND) {
			
			//
			// No continuation needed
			//	

			memcpy (receiveBuffer, DataBegin, len);
			return Id;
		}

		//
		// Need continuation
		//

		Boolean Loop = TRUE;
		unsigned char* CurrentOutputPointer = receiveBuffer;
		int WriteSize = BufferSize;
		int LeftToWrite = len;
		Boolean Wait = FALSE;
		port = ClientPortList.GetElementAt (Id);

		if (port == NULL) {

			LastError = RGCLIENTNULL;
			return -1;
		}

		HandleArray [0] = port->GetClientHandle ();
		HandleArray [1] = OKToReadHandle;

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

			if ((LeftToWrite <= 0) || (*Type == RGBLOCKINGSEND))
				Loop = FALSE;

			else {

				SetEvent (ContinuationHandle);
			
				if (LeftToWrite <= BufferSize)
					WriteSize = LeftToWrite;
			}
		}  // end of while loop

		return Id;
	}

	else {

		port = ClientPortList.GetElementAt (clientId);

		if (port == NULL) {

			LastError = RGCLIENTNULL;
			return -1;
		}

		//
		// arrange to receive only from client at clientId - requires an additional event flag
		//

		port->TriggerHotLine ();
		WaitStatus = port->WaitForReadAccessFromClient (OKToReadHandle);

		if (WaitStatus < 0) {

			RemoveClient (clientId);
			return -1;
		}

		len = *Length;

		//
		// Test for buffer overflow...we don't like to encourage this!
		//

		if (len > receiveLength) {

			delete[] receiveBuffer;
			receiveBuffer = new unsigned char [len];
			receiveLength = len;
		}

		//
		// Test for continuation
		//

		if (*Type == RGBLOCKINGSEND) {
			
			//
			// No continuation needed
			//	

			memcpy (receiveBuffer, DataBegin, len);
			return clientId;
		}

		//
		// Need continuation
		//

		Boolean Loop = TRUE;
		unsigned char* CurrentOutputPointer = receiveBuffer;
		int WriteSize = BufferSize;
		int LeftToWrite = len;
		Boolean Wait = FALSE;

		while (Loop) {
			
			if (Wait) {
			
				status = port->WaitForReadAccessFromClient (OKToReadHandle);  // wait for read clearance on continuation

				if (status < 0) {

					LastError = port->GetLastError ();
					return status;
				}
			}

			Wait = TRUE;
			memcpy (CurrentOutputPointer, DataBegin, WriteSize);
			CurrentOutputPointer += BufferSize;
			LeftToWrite -= BufferSize;

			if ((LeftToWrite <= 0) || (*Type == RGBLOCKINGSEND))
				Loop = FALSE;

			else {

				SetEvent (ContinuationHandle);
			
				if (LeftToWrite <= BufferSize)
					WriteSize = LeftToWrite;
			}
		}  // end of while loop

		return clientId;
	}

	LastError = RGNOTYETSUPPORTED;
	return -1;
}


int RGServer :: TestReceive (int clientId, unsigned char*& receiveBuffer, int& receiveLength) {

	//
	//  returns Id of client
	//
	
	RGClientPort* port;
	int ListLength;
	int i;
	Boolean Found = FALSE;
	LastError = RGOK;
	DWORD WaitStatus;
	
	if (clientId == 0) {

		ListLength = ClientPortList.Length ();

		for (i=1; i<ListLength; i++) {

			port = ClientPortList.GetElementAt (i);

			//
			// check to see if client is blocking on send to this process
			//

			if (MyOwnPid == port->GetPidOfBlockingServer ()) {

				Found = TRUE;
				break;
			}
		}

		if (Found)  // at least one client is trying to send, so can safely receive from anyone, highest priority first
			return Receive (0, receiveBuffer, receiveLength);

		WaitStatus = WaitForSingleObject (ImTryingToContactYouHandle, 0);

		if (WaitStatus == WAIT_TIMEOUT)
			return 0;

		ReleaseSemaphore (ImTryingToContactYouHandle, 1, NULL);
		return Receive (0, receiveBuffer, receiveLength);
	}

	else {

		port = ClientPortList.GetElementAt (clientId);

		if (port == NULL) {

			LastError = RGCLIENTNULL;
			return -1;
		}

		if (MyOwnPid == port->GetPidOfBlockingServer ())
			return Receive (clientId, receiveBuffer, receiveLength);

		return 0;
	}

	return -1;
}


int RGServer :: Reply (int clientId, unsigned char* replyBuffer, int replyLength) {

	RGClientPort* port = ClientPortList.GetElementAt (clientId);

	if (port == NULL) {

		LastError = RGCLIENTNULL;
		return -1;
	}

	int status = port->Reply (replyLength, replyBuffer);

	if (status < 0) {

		LastError = port->GetLastError ();
		RemoveClient (clientId);
		return -1;
	}

	return 0;
}


void RGServer :: RemoveClient (int id) {

	RGClientPort* port = ClientPortList.RemoveElementAt (id);
	delete port;
	int* newInt = new int;
	*newInt = id;
	UnusedClientIds.append (newInt);
}


int RGServer :: WaitForReadAccess (HANDLE* HandleArray) {

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
