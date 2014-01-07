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
*  FileName: rgclient.h
*  Author:   Robert Goor
*
*/
//
//  Header file for client operations in high level IPC.  Uses shared memory and Windows Win32 Events to coordinate
// between client and server.  Implements Send/Receive/Reply paradigm for blocking/non-blocking control and message
// passing.
//
//  RGServerPort is a class that maintains communication with a particular server.  RGClient is a class that performs all
// of the client functions.  To Send to a server requires that an RGClient object work through an RGServerPort object.
// Each RGClient object maintains a list of servers with which it has communicated.
//

#ifndef _RGCLIENT_H_
#define _RGCLIENT_H_

#include <windows.h>

#include "rgstring.h"
#include "rgcommlist.h"
#include "rgtarray.h"

class RGClient;


class RGServerPort {

public:
	RGServerPort ();
	RGServerPort (const RGString& name);
	~RGServerPort ();

	int Connect (const RGString& name, RGClient* parent);
	int Connect (RGClient* parent);
	int Disconnect ();
	int Send (int len, unsigned char* data);
	HANDLE GetServerHandle () const { return ServerPidHandle; }
	unsigned long GetServerPid () const;
	Boolean isEqual (const RGServerPort* port);

	int GetLastError () const { return LastError; }
	friend Boolean operator==(const RGServerPort& sp1, const RGServerPort& sp2);

protected:
	RGString Name;

    HANDLE OKToReadHandle;   // ok for other end of port to read buffer
    HANDLE OKToWriteHandle;  // ok to write to buffer
    HANDLE ContinuationHandle;
	HANDLE ImTryingToContactYouHandle;
	HANDLE ServerPidHandle;
	HANDLE HotLineHandle;

    int BufferSize;
    HANDLE BufferHandle;
    unsigned char* BufferMapAddress;
    unsigned long* Type;
    unsigned long* Length;
	unsigned long* ServerPid;
	unsigned long* ReplyPointer;
	unsigned char* DataBegin;
	int LastError;
	Boolean FirstTime;
	unsigned long RemotePortID;
	Boolean Connected;
	int ClientId;
	const RGClient* Parent;

	int WaitForWriteAccess (HANDLE* HandleArray);
	int WaitForSendAccess (HANDLE* HandleArray);
};


class RGClient {

public:
	RGClient (const RGString& name);
	~RGClient ();

	static void ResetReplyBufferSize (int newSize);

	Boolean IsActive () const { return Initialized; }

	const char* GetName () const { return Name.GetData (); }
	unsigned long GetNameLength () const { return (unsigned long) Name.Length (); }
	int GetLastError () const { return LastError; }

	int LocateServer (const RGString& name);  // use the output of this method as server id to call Send.  This is local only
	int Send (int serverId, unsigned char* sendBuffer, unsigned char*& replyBuffer, int sendLength, int& replyMax);

	void RemoveServer (int id);
	void ClearServerPid () const;

protected:
	RGString Name;
	RGTarray<RGServerPort> ServerPortList;
	CommDataList<int> UnusedServerIds;
	int NextServerId;

    HANDLE OKToReadHandle;   // ok for this end of port to read reply buffer
	HANDLE OKToWriteHandle;  // ok for server end of port to write into reply buffer...only needed for continuation
    HANDLE ContinuationHandle;

    int BufferSize;
    HANDLE BufferHandle;
    unsigned char* BufferMapAddress;
    unsigned long* Type;
    unsigned long* Length;
	unsigned long* ServerPid;  // To notify servers what destination is intended (for TestReceive)
	unsigned long* MyPid;
	unsigned char* DataBegin;
	int LastError;
	unsigned long RemotePortID;   // ????
	Boolean Initialized;

	int WaitForReadAccess (HANDLE* HandleArray);
	int Initialize ();

	static int ReplyBufferSize;
};


#endif  /*  _RGCLIENT_H_  */
