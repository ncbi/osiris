//
//	This app builds the overall standard control file from a tab-delimited text file located in the osiris/Documents folder
//

#include "rgfile.h"
#include "rgstring.h"
#include "rgtokenizer.h"
#include "rgtptrdlist.h"
#include "LocusData.h"
#include "RGTextOutput.h"
#include <iostream>
#include <tchar.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {

	RGString FullPath ("..");
	RGString InputFileDirectory ("Documentation/StandardPositiveControlList.txt");
	RGString OutputFileDirectory ("OsirisXML/LadderSpecifications/StandardPositiveControls.xml");

	RGString fullPathName = FullPath + "/" + InputFileDirectory;
	RGString fullOutputPathName = FullPath + "/" + OutputFileDirectory;
	RGFile inputList (fullPathName, "rt");
	RGTextOutput outputList (fullOutputPathName, FALSE);
	char oneLine [1000];

	if (!inputList.isValid ()) {

		cout << "Could not access input standard control list named " << (char*)fullPathName.GetData () << endl;
		cout << "Exiting..." << endl;
		return 50;
	}

	if (!outputList.FileIsValid ()) {

		cout << "Could not open output standard control list named " << (char*) fullOutputPathName.GetData () << endl;
		cout << "Exiting..." << endl;
		return 55;
	}

	if (!inputList.ReadLine (oneLine)) {

		cout << "Could not read first line of input file.  Exiting..." << endl;
		return 75;
	}

	RGStringArray tokens;
	RGStringArray stdControls;
	RGStringArray delimiters;
	RGString lineString = oneLine;
	RGStringTokenizer oneLineTokens (lineString);
	oneLineTokens.AddDelimiter ("\t");

	RGString locusString;
	RGString alleleString;

	oneLineTokens.Split (stdControls, delimiters);
	int n = stdControls.Length ();

	if (n < 6) {

		cout << "Not enough columns.  n = " << n << ".  Exiting..." << endl;
		return 100;
	}

	cout << "There are " << n - 1 << " named standard controls" << endl;
	cout << "They are:  ";

	int i;

	for (i=1; i<n; i++) {

		cout << (char*)stdControls [i].GetData ();

		if (i < n-1)
			cout << ", ";
	}

	cout << endl;
	RGString locusName;
	LocusData* nextLocusData;
	RGTPtrDList<LocusData>** controlAlleleList = new RGTPtrDList<LocusData>* [n];

	for (i=0; i<n; i++)
		controlAlleleList [i] = new RGTPtrDList<LocusData>;

	while (true) {

		if (!inputList.ReadLine (oneLine))
			break;

		lineString = oneLine;
		oneLineTokens.ResetTokenizer ();
		oneLineTokens.Split (tokens, delimiters);

		//if (tokens.Length () < n)
		//	break;

		locusName = tokens [0];

		for (i=1; i<n; i++) {

			if (tokens [i].Length () == 0)
				continue;

			nextLocusData = new LocusData (locusName, tokens [i]);
			controlAlleleList [i]->append (nextLocusData);
		}
	}
	
	Endl endLine;
	outputList << "\t\t\t<PositiveControls>" << endLine;
	RGString locusAlleleList;

	for (i=1; i<n; i++) {

		outputList << "\t\t\t\t<PositiveControl>" << endLine;
		outputList << "\t\t\t\t\t<Name>" << stdControls [i] << "</Name>" << endLine;
		outputList << "\t\t\t\t\t<Loci>" << endLine;

		while (controlAlleleList [i]->entries () > 0) {

			nextLocusData = controlAlleleList [i]->RemoveFirst ();
			locusAlleleList = nextLocusData->FormXMLString ();
			outputList << locusAlleleList;
			delete nextLocusData;
		}

		outputList << "\t\t\t\t\t</Loci>" << endLine;
		outputList << "\t\t\t\t</PositiveControl>" << endLine;
	}

	outputList << "\t\t\t</PositiveControls>" << endLine;

	for (i=0; i<n; i++)
		delete controlAlleleList [i];

	delete [] controlAlleleList;
	return 0;
}