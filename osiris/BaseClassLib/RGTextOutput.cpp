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
*  FileName: RGTextOutput.cpp
*  Author:   Robert Goor
*
*/
//
//  class RGTextOutput automates the outputting of text strings and numbers, simultaneously writing to a file and echoing to a console
//

#include "RGTextOutput.h"
#include <iostream>


using namespace std;

int RGTextOutput::Resolution = 10;


Endl :: Endl () : NumEndlines (1) {

}


Endl :: Endl (int i) : NumEndlines (i) {

}

CLevel :: CLevel () : mCLevel (0) {

}

CLevel :: CLevel (int cLevel) : mCLevel (cLevel) {

}

PLevel :: PLevel () {

}


RGTextOutput :: RGTextOutput (const RGString& fileName, Boolean echo) : Echo (echo), CreatedFile (TRUE) {

	file = new RGFile (fileName, "wt");
}


RGTextOutput :: RGTextOutput (RGFile* ofile, Boolean echo) : Echo (echo), CreatedFile (FALSE) {

	file = ofile;
}


RGTextOutput :: ~RGTextOutput () {

	file->Flush ();

	if (CreatedFile) {

		file->Close ();
		delete file;
	}
}


Boolean RGTextOutput :: FileIsValid () const {

	return file->isValid ();
}


Boolean RGTextOutput :: Write (const RGString& s) {

	file->Write (s.GetData ());

	if (Echo)
		cout << s;

	return TRUE;
}


Boolean RGTextOutput :: Write (const char* s) {

	file->Write (s);

	if (Echo)
		cout << s;

	return TRUE;
}


void RGTextOutput :: SetOutputLevel (int level) {

}


void RGTextOutput :: ResetOutputLevel () {

}


Boolean RGTextOutput :: Write (int level, const RGString& s) {

	Write (s);
	return TRUE;
}


Boolean RGTextOutput :: Write (int level, const char* s) {

	Write (s);
	return TRUE;
}


RGTextOutput& RGTextOutput :: operator<<(const RGString& s) {

	Write (s);
	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(const char* s) {

	Write (s);
	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(int i) {

	GetString ((long)i);
	file->Write (buffer);

	if (Echo)
		cout << i;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(double d) {

	GetString (d);
	file->Write (buffer);

	if (Echo)
		cout << d;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(unsigned long i) {

	GetString ((long)i);
	file->Write (buffer);

	if (Echo)
		cout << i;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(char c) {

	file->Write (c);

	if (Echo)
		cout << c;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(long l) {

	GetString (l);
	file->Write (buffer);

	if (Echo)
		cout << l;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(short s) {

	GetString ((long)s);
	file->Write (buffer);

	if (Echo)
		cout << s;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(unsigned char c) {

	GetString ((long)c);
	file->Write (buffer);

	if (Echo)
		cout << c;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(unsigned int i) {

	GetString ((long)i);
	file->Write (buffer);

	if (Echo)
		cout << i;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(unsigned short s) {

	GetString ((long)s);
	file->Write (buffer);

	if (Echo)
		cout << s;

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(const Endl& endLine) {

	int NumLines = endLine.NumEndlines;
	int i;

	if (Echo) {

		for (i=0; i<NumLines; i++) {

			file->Write ("\n", 1);
			cout << endl;
		}
	}

	else {

		for (i=0; i<NumLines; i++)
			file->Write ("\n", 1);
	}

#ifdef _DEBUG
	file->Flush ();
#endif

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(const CLevel& clevel) {

	return *this;
}


RGTextOutput& RGTextOutput :: operator<<(const PLevel& plevel) {

	return *this;
}


Boolean RGTextOutput :: TestCurrentLevel () const {

	return TRUE;
}



void RGTextOutput :: EndLine () {

	file->Write ("\n", 1);

	if (Echo)
		cout << endl;
}


void RGTextOutput :: GetString (long l) {
	_ltoa (l, buffer, 10);
}


void RGTextOutput :: GetString (double d) {

	gcvt (d, RGTextOutput::Resolution, buffer);
	size_t l = strlen (buffer);

	if ((l > 0) && (buffer [l-1] == '.'))
		buffer [l-1] = '\0';
}

