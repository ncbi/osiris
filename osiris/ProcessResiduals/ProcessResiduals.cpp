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
*  FileName: TestAnalysisDirectoryLC.cpp
*  Author:   Robert Goor
*
*/


#include "stdafx.h"
#include "LocusAlleles.h"
#include "rgfile.h"

#include "rgstring.h"
#include "RGTextOutput.h"
#include "rgdirectory.h"
#include "rgtokenizer.h"
#include "rgtokenizer.h"


#include <string>
#include <math.h>
#include <list>

using namespace std;



int _tmain(int argc, _TCHAR* argv[]) {

	//RGString fileName = "C:/Users/goorrob/Documents/GitHub/osiris/ResdiualDataFromISP/AMP-110616-03A_2_90210 disabled.txt";
	//RGString outFileName = "C:/Users/goorrob/Documents/GitHub/osiris/ResdiualDataFromISP/AMP-110616-03A_2_90210 disabled.output.txt";
	//RGString fileName = "C:/Users/goorrob/Documents/GitHub/osiris/ResdiualDataFromISP/AMP-110616-04A_2_007 disabled.txt";
	RGString outFileName;	// = "C:/Users/goorrob/Documents/GitHub/osiris/ResdiualDataFromISP/AMP-110616-04A_2_007 disabled.output.txt";
	//RGFile dataInput (fileName, "rt");
	char newCharLine [501];
	int nLoci = 0;
	RGString locusNames [20];
	RGDList* locusCollections [20];
	LocusWithAlleles* nextLocus;
	int i;
	bool foundName;
	RGString currentLocusName;
	RGString tenSecond = "_2_";
	bool useTenSecondData = false;
	RGString inputSuffix = " disabled.xls.txt";
	RGString directory = "C:/Users/goorrob/Documents/GitHub/osiris/ResdiualDataFromISP/ResidualFiles/Residual Files/output/";
	RGString currentFileBase;
	RGFile* currentInputFile;
	RGString fileNameFileName = directory + "filelist2.txt";
	RGFile fileNameFile (fileNameFileName, "rt");
	RGString outlierName;

	if (!fileNameFile.isValid ()) {

		cout << "Exiting...Could not open file with list of file names, called " << (char*)fileNameFileName.GetData () << endl;
		return -10;
	}

	//while (fileNameFile.ReadLine (newCharLine)) {

	//	cout << newCharLine << endl;
	//}

	//return 0;

	if (useTenSecondData) {

		outFileName = directory + "TenSecondSummary.txt";
		outlierName = directory + "TenSecondOutlierList.txt";
	}

	else {

		outFileName = directory + "FiveSecondSummary.txt";
		outlierName = directory + "FiveSecondOutlierList.txt";
	}

	RGTextOutput outlierFile (outlierName, FALSE);

	if (!outlierFile.FileIsValid ()) {

		cout << "Exiting...Could not open outlier file named:  " << (char*)outlierName.GetData () << "\n";
		return -1000;
	}


	//if (!dataInput.isValid ()) {

	//	cout << "Exiting...Could not open input file named " << (char*)fileName.GetData () << endl;
	//	return 100;
	//}

	//cout << "Input file opened..." << endl;

	RGTextOutput outFile (outFileName, FALSE);

	if (!outFile.FileIsValid ()) {

		cout << "Exiting...Could not open output file named " << (char*)outFileName.GetData () << endl;
		return 100;
	}

	RGString nextLine;
	int count = 0;
	Boolean status;
	RGString currentFileName;
	RGString* nextFileName;
	RGDList fileNames;

	while (fileNameFile.ReadLine (newCharLine)) {

		currentFileBase = newCharLine;
		
		if (useTenSecondData)
			currentFileBase.FindAndReplaceAllSubstrings ("_1_", "_2_");

		currentFileName = directory + currentFileBase + inputSuffix;
		nextFileName = new RGString (currentFileBase + inputSuffix);
		fileNames.Append (nextFileName);

		currentInputFile = new RGFile (currentFileName, "rt");

		if (!currentInputFile->isValid ()) {

			cout << "Exiting...Could not open input file:  " << (char*)currentFileName.GetData () << endl;
			return 5000;
		}

		cout << "Reading:  " << (char*) currentFileName.GetData () << endl;

		status = currentInputFile->ReadLine (newCharLine);
		status = currentInputFile->ReadLine (newCharLine);
	

		while (currentInputFile->ReadLine (newCharLine)) {

			nextLine = newCharLine;
			nextLocus = new LocusWithAlleles (nextLine);
			nextLocus->SetFileNamePtr (nextFileName);

			if (!nextLocus->HasAtLeastTwoAlleles ())
				delete nextLocus;

			else {

				// Find locus name and insert into corresponding list
				foundName = false;
				currentLocusName = nextLocus->GetLocusName ();
				//nextLocus->OutputLocus (outFile);

				for (i=0; i<nLoci; i++) {

					if (currentLocusName == locusNames [i]) {

						foundName = true;
						locusCollections [i]->Append (nextLocus);
						break;
					}
				}

				if (!foundName) {

					locusNames [nLoci] = currentLocusName;
					locusCollections [nLoci] = new RGDList;
					locusCollections [nLoci]->Append (nextLocus);
					nLoci++;
				}
			}

			count++;
		}

		delete currentInputFile;
	}

	cout << "Total number of loci = " << count << endl;
	RGDListIterator* it;
	list<double> displacements;
	int nDisplacements;
	double average;
	double max;
	double min;
	list<double>::iterator dispIt;
	int countDeciles;
	int deciles [11];
	double decileMax [11];
	double decileMin [11];
	int j;
	int decileNumber;
	double maxMinusMin;
	double increment;
	double sigma2;
	double sigma;
	nextFileName = NULL;

	for (i=0; i<nLoci; i++) {

		it = new RGDListIterator (*locusCollections [i]);

		while (nextLocus = (LocusWithAlleles*) (*it) ()) {

			nextLocus->GetResidualDisplacementList (displacements, outlierFile, nextFileName);
		}


		nDisplacements = displacements.size ();
		displacements.sort ();
		min = displacements.front ();
		max = displacements.back ();
		maxMinusMin = max - min;
		increment = 0.1 * maxMinusMin;
		average = 0.0;
		countDeciles = 0;
		sigma2 = 0.0;

		for (j=0; j<10; j++) {

			deciles [j] = 0;
			decileMin [j] = ((double)j) * increment + min;
			decileMax [j] = ((double)(j + 1)) * increment + min;
		}

		for (dispIt = displacements.begin (); dispIt != displacements.end (); dispIt++) {

			average += *dispIt;
			sigma2 += (*dispIt) * (*dispIt);
			decileNumber = (int) floor (10.0 * (*dispIt - min) / maxMinusMin);

			if (decileNumber == 10)
				decileNumber = 9;

			deciles [decileNumber]++;
		}

		average = average / (double)nDisplacements;
		sigma = (sigma2 / (double)nDisplacements) - average * average;

		double thirdMoment = 0.0;
		double fourthMoment = 0.0;
		double temp;
		double temp2;

		for (dispIt = displacements.begin (); dispIt != displacements.end (); dispIt++) {

			temp = *dispIt - average;
			temp2 = temp * temp;
			thirdMoment += temp * temp2;
			fourthMoment += temp2 * temp2;
		}

		thirdMoment = thirdMoment / (double)nDisplacements;
		fourthMoment = fourthMoment / (double)nDisplacements;

		double skewness = thirdMoment / (sigma * sqrt (sigma));
		double kurtosis = fourthMoment / (sigma * sigma);

		outFile << "Locus:  " << locusNames [i] << " has average RD = " << average << " with std dev = " << sqrt (sigma) << ", and with Max = " << max << " and Min = " << min << " with " << nDisplacements << " displacements\n";
		outFile << "     Skewness (Normal = 0) = " << skewness << "       Kurtosis (Normal = 3) = " << kurtosis << "\n";
		outFile << "     Ave minus 3 std dev = " << average - 3.0 * sqrt (sigma) << "       Ave plus 3 std dev = " << average + 3.0 * sqrt (sigma) << "\n";

		for (j=0; j<10; j++) {

			outFile << "\t\tDecile " << j+1 << ":  min = " << decileMin [j] << ", max = " << decileMax [j] << ", Count = " << deciles [j] << "\n";
		}

		outFile << "\n";
		delete it;
		displacements.clear ();
	}

	return 0;
}

