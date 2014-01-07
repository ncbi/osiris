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
*  FileName: DoubleArray.h
*  Author:   Robert Goor
*
*/
//
//  Struct to manage an array of doubles with a name
//

#ifndef _DOUBLEARRAY_H_
#define _DOUBLEARRAY_H_

#include "rgstring.h"

struct DoubleArray {

	DoubleArray (int N) : Size (N) { data = new double [N]; }
	DoubleArray (int N, const double* array) : Size (N) {
		
		data = new double [N];
		for (int i=0; i<N; i++)
			data [i] = array [i];
	}

	~DoubleArray () { delete[] data; }
	friend operator==(const DoubleArray& a, const DoubleArray& b) {

		if (a.Size != b.Size)
			return FALSE;

		for (int i=0; i<a.Size; i++) {

			if (a.data [i] != b.data [i])
				return FALSE;
		}

		return TRUE;
	}

int Size;
double* data;
RGString name;
};


#endif  /*  _DOUBLEARRAY_H_  */

