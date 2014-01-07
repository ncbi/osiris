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
*  FileName: rgserver.h
*  Author:   Robert Goor
*
*/
//
//  Header file for server operations in high level IPC.  Uses shared memory and Windows Win32 Events to coordinate
// between client and server.  Implements Send/Receive/Reply paradigm for blocking/non-blocking control and message
// passing.
//
//  RGClientPort is a class that maintains communication with a particular client.  RGServer is a class that performs all
// of the server functions.  To Receive from and Reply to a client requires that an RGServer object work through an RGClientPort object.
// Each RGServer object maintains a list of clients with which it has communicated.
//

#ifndef _RGSERVER_H_
#define _RGSERVER_H_

#include <windows.h>

#include "rgstring.h"
#include "rgcommlist.h"
#include "rgtarray.h"

class RGServer;


class RGClientPort {

public:
	RGClientPort ();
	RGClientPort (const RGString& name);
	~RGClientPort ();

	int Connect (const RGString& name, RGServer* parent);
	int Connect (RGServer* parent);
	int Disconnect ();
	int Reply (int len, unsigned char* data);
	HANDLE GetClientHandle () const { return ClientPidHandle; }
	unsigned long GetClientPid () const;
	unsigned long GetPidOfBlockingServer () const;
	Boolean isEqual (const RGClientPort* port);

	int GetLastError () const { return LastError; }
	int WaitForReadAccessFromClient (HANDLE specificHandle);  // when receiving from a specific client
	void TriggerHotLine ();

	friend Boolean operator==(const RGClientPort& sp1, const RGClientPort& sp2);

protected:
	RGString Name;

    HANDLE OKToReadHandle;   // ok for other end of port to read buffer
    HANDLE OKToWriteHandle;  // ok to write to buffer
    HANDLE ContinuationHandle;
	HANDLE ClientPidHandle;
	HANDLE HotLineHandle;

    int BufferSize;
    HANDLE BufferHandle;
    unsigned char* BufferMapAddress;
    unsigned long* Type;
    unsigned long* Length;
	unsigned long* ServerPid;
	unsigned long* ClientPid;  // needed for Receive from specific client and for Reply with continuation
	unsigned char* DataBegin;
	int LastError;
	Boolean FirstTime;
	unsigned long RemotePortID;
	Boolean Connected;
	int ServerId;
	const RGServer* Parent;

	int WaitForWriteAccess (HANDLE* HandleArray);
};


class RGServer {

public:
	RGServer (const RGString& name);
	~RGServer ();

	static void ResetReceiveBufferSize (int newSize);

	Boolean IsActive () const { return Initialized; }

	const char* GetName () const { return Name.GetData (); }
	unsigned long GetNameLength () const { return (unsigned long) Name.Length (); }
	int GetLastError () const { return LastError; }

	int Receive (int clientId, unsigned char*& receiveBuffer, int& receiveLength);
	int TestReceive (int clientId, unsigned char*& receiveBuffer, int& receiveLength);
	int Reply (int clientId, unsigned char* replyBuffer, int replyLength);

	void RemoveClient (int id);

protected:
	RGString Name;
	RGTarray<RGClientPort> ClientPortList;
	CommDataList<int> UnusedClientIds;
	int NextClientId;

    HANDLE OKToReadHandle;   // ok for this end of port to read receive buffer
	HANDLE OKToWriteHandle;  // ok for client end of port to write into receive buffer
    HANDLE ContinuationHandle;
	HANDLE ImTryingToContactYouHandle;  // semaphore released for every client trying to send

    int BufferSize;
    HANDLE BufferHandle;
    unsigned char* BufferMapAddress;
    unsigned long* Type;
    unsigned long* Length;
	unsigned long* ReplyPointer;
	unsigned long* ServerPid;  // To notify servers what destination is intended (for TestReceive)
	unsigned char* DataBegin;
	int LastError;
	unsigned long RemotePortID;   // ????
	Boolean Initialized;
	unsigned long MyOwnPid;

	int WaitForReadAccess (HANDLE* HandleArray);
	int RegisterName ();

	static int ReceiveBufferSize;
};


#endif  /*  _RGSERVER_H_  */
