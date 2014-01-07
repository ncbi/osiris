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
*  FileName: ByteOrder.cpp
*  Author:   Robert Goor
*
*/
#include <string.h>
#include <stdlib.h>
#include "ByteOrder.h"

bool ByteOrder::_gbSetupBigEndian = false;
bool ByteOrder::_gbLocalBigEndian = false;


void ByteOrder::setup()
{
	union
	{
	INT32 n;
		char c[4];
	} xx;
	xx.n = 1;
	_gbLocalBigEndian = !xx.c[0];
	_gbSetupBigEndian = true;
}

void ByteOrder::memcpyrv(void	*pDest,	const void *pSrc,	size_t nLen)
{
		UINT8 *ppDest = (UINT8 *)pDest;
		const UINT8 *ppSrc	=	(UINT8 *)pSrc;
		INT64 nDiff = (ppSrc - ppDest);
		(nDiff < 0) && (nDiff = -nDiff);
		bool bInPlace = !nDiff;
		ppDest +=	(nLen	-	1);
		if(bInPlace)
		{
			UINT8 c;
			UINT8 *ppDest2 = (UINT8 *)pDest;
			while(ppDest2 < ppDest)
			{
				// swap
				c = *ppDest;
				*ppDest = *ppDest2;
				*ppDest2 = c;
				// increment/decrement
				ppDest--;
				ppDest2++;
			}
		}
		else if(nDiff < nLen)
		{
			// source and destination overlap
			// this should NEVER happen
			void *psTmp = malloc(nLen);
			memcpyrv(psTmp,pSrc,nLen);
			memcpyrv(pDest,psTmp,nLen);
			free(psTmp);
		}
		else
		{
			for(;nLen > 0; nLen--)
			{
				*ppDest--	=	*ppSrc++;
			}
		}
}

template<class TYPE> TYPE ByteOrderGet(const UINT8 *p,bool bReverse)
{
	TYPE nRtn;
	if(bReverse)
	{
		ByteOrder::memcpyrv((void *)&nRtn,(const void *)p,sizeof(TYPE));
	}
	else
	{
		memcpy((void *)&nRtn,(const void *)p,sizeof(TYPE));
	}
	return nRtn;
}

INT16 ByteOrder::localInt16(const UINT8 *p,bool bBig)
{
	return ByteOrderGet<INT16>(p,bBig != LocalBigEndian());
}
INT32 ByteOrder::localInt32(const UINT8 *p,bool bBig)
{
	return ByteOrderGet<INT32>(p,bBig != LocalBigEndian());
}
UINT16 ByteOrder::localUint16(const UINT8 *p,bool bBig)
{
	return ByteOrderGet<UINT16>(p,bBig != LocalBigEndian());
}
UINT32 ByteOrder::localUint32(const UINT8 *p,bool bBig)
{
	return ByteOrderGet<UINT32>(p,bBig != LocalBigEndian());
}

