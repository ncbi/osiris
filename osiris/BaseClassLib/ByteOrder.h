/*
 * =====================================================================
  
                              PUBLIC DOMAIN NOTICE
                 National Center for Biotechnology Information

    This software/database is a "United States Government Work" under 
		the terms of the United States Copyright Act.  It was written as 
		part of the author's official duties as a United States Government 
		employee and thus cannot be copyrighted.  This software/database is 
		freely available to the public for use. The National Library of 
		Medicine and the U.S. Government have not placed any restriction on 
		its use or reproduction.

		Although all reasonable efforts have been taken to ensure the 
		accuracy and reliability of the software and data, the NLM and the 
		U.S. Government do not and cannot warrant the performance or results 
		that may be obtained by using this software or data. The NLM and 
		the U.S. Government disclaim all warranties, express or implied, 
		including warranties of performance, merchantability or fitness for 
		any particular purpose.

		Please cite the author in any work or product based on this material.

 * =====================================================================
 *
 * Author:  Douglas J. Hoffman
 *          Contractor
 *          National Center For Biotechnology Information
 *          National Library of Medicine
 *          National Institutes of Health
 *
 * File Description:
		ByteOrder.h - header file for class used to determine if
		this computer uses big endian numbers and provides static
		methods fot reversing bytes when needed
 */
#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

#include "rgdefs.h"

class ByteOrder
{
public:
	static bool LocalBigEndian()
	{
		if(!_gbSetupBigEndian)
		{
			setup();
		}
		return _gbLocalBigEndian;
	}
	static INT16 localInt16(const UINT8 *p, bool bBig = true);
	static INT32 localInt32(const UINT8 *p, bool bBig = true);
	static UINT16 localUint16(const UINT8 *p, bool bBig = true);
	static UINT32 localUint32(const UINT8 *p, bool bBig = true);
	static void memcpyrv(void	*pDest,	const void *pSrc,	size_t nLen);
	template<class T> static void reverse(T *p)
	{
		memcpyrv((void *)p,(const void *)p,sizeof(*p));
	}
  static void reverse(char *p,size_t n, UINT32 nLoops = 1)
	{
    for(UINT32 i = 0; i < nLoops; i++)
    {
		  memcpyrv((void *)p,(const void *)p,n);
      p += n;
    }
	}
  static void reverse(UINT8 *p,size_t n, UINT32 nLoops = 1)
	{
    reverse((char *)p,n,nLoops);
	}
private:
	ByteOrder() {}; // prevent instantiation
	static void setup();
	static bool _gbLocalBigEndian;
	static bool _gbSetupBigEndian;
};

#endif
