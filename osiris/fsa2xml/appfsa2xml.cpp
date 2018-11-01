
// fsa2xml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <stdio.h>

#include "RGmemCheck.h"

//MEMCHECK(g_x,6881);

#include "fsaFileInput.h"
#include "xmlwriter.h"
#include "rgfile.h"

using namespace std;

#ifndef _WIN32
#define _tmain main
#define _TCHAR char
#endif

static void Pause()
{
	char s[128];
	gets_s(s,sizeof(s)-1);
}
int _tmain(int argc, _TCHAR* argv[])
{
  MEMDIFF(x);
// 	char *ps = strdup("ABCDEFG - ignore this");
#ifdef _DEBUG
  if(!fsaFileInput::TESTMEMSTR())
  {
    cerr << "function fsaFileInput::MEMSTR() failed test." << endl;
  }
#endif
	if(argc < 3)
	{
		cerr << "Usage: " << argv[0] << " <infilename> <outfilename>" << endl;
		//Pause();
		return 1;
	}

	fsaFileInput fsa(argv[1],true);
//	fsaFileIO fsa ("E:/Users/goorrob/My Documents/ABI Info/DataFileConverter/20030707BCOF_B02_2002-011420_016.fsa");

	if(fsa.Error())
	{
		cerr << "Cannot open " << argv[1] << endl;
		Pause();
		return 2;
	}

	RGFile file (argv [2], "wt");
//	RGFile file ("E:/Users/goorrob/My Documents/ABI Info/DataFileConverter/Test.xml");

	if (!file.isValid ()) {

		cerr << "Output file name invalid" << endl;
		return 5;
	}
  if(fsa.Warning())
  {
    cerr << "File has problems, continuing" << endl;
  }

	fsa.WriteXML (file);
	file.Flush ();
	file.Close ();

	if (argc > 3) {

		RGFile notify (argv [3], "a+t");

		if (!notify.isValid ()) {

			cerr << "Cannot open " << argv [3] << " to notify about " << argv [2] << endl;
			return 6;
		}

		RGString Notification = "     <xml>Wrote         ";
		Notification += argv [2];
		Notification += "</xml>";
		Notification.WriteTextLine (notify);
		notify.Close ();
	}

	return 0;

}

