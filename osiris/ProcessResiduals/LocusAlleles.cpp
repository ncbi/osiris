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
*  FileName: STRChannelData.h
*  Author:   Robert Goor
*
*/
//
//	Stores locus and allele info for residual study
//


#include "LocusAlleles.h"

const double erdThreshold = 0.3;

PERSISTENT_DEFINITION (LocusAllele, _LOCUSALLELE_, "LocusAllele")
PERSISTENT_DEFINITION (LocusWithAlleles, _LOCUSWITHALLELES_, "LocusWithAlleles")

LocusAllele :: LocusAllele () : RGPersistent (), mRFU (0), mResidual (0.0), mFit (0.0) {

}



LocusAllele :: LocusAllele (int rfu, double residual, double fit) : RGPersistent (), 
	mRFU (rfu), 
	mResidual (residual), 
	mFit (fit) {

}



LocusAllele :: LocusAllele (const LocusAllele& locusAllele) : RGPersistent (locusAllele),
	mRFU (locusAllele.mRFU),
	mResidual (locusAllele.mResidual), 
	mFit (locusAllele.mFit) {

}



LocusAllele :: ~LocusAllele () {

}


void LocusAllele :: OutputAllele (RGTextOutput& outFile) {

	outFile << mRFU << "\t\t" << mResidual << "\t\t" << mFit;
}


int LocusAllele :: CompareTo (const RGPersistent* p) const {

	LocusAllele* q = (LocusAllele*) p;

	if (mRFU > q->mRFU)
		return 1;

	if (mRFU == q->mRFU)
		return 0;

	return -1;
}



unsigned LocusAllele :: HashNumber (unsigned long Base) const {

	return mRFU%Base;
}


Boolean LocusAllele :: IsEqualTo (const RGPersistent* p) const {

	LocusAllele* q = (LocusAllele*) p;

	if (mRFU == q->mRFU)
		return TRUE;

	return FALSE;
}





LocusWithAlleles :: LocusWithAlleles () : RGPersistent (), mLineNumber (0), mMaxRFU (0) {

}



LocusWithAlleles :: LocusWithAlleles (const RGString& inputLine) : RGPersistent (), mLineNumber (0), mMaxRFU (0) {

	RGStringArray tokens;
	RGStringArray delimiters;
	RGString locusString (inputLine);
	locusString.FindAndReplaceAllSubstrings ("\t\t", "\t");
	RGStringTokenizer tokenizer (locusString);
	tokenizer.AddDelimiter ("\t");

	tokenizer.Split (tokens, delimiters);
	int size = tokens.Length ();
	int rfu;
	double fit;
	double residual;
	LocusAllele* newAllele;
	int i;

	if (size >= 8) {

		for (i=0; i<8; i++) {

			if ((tokens [i].Length () == 0) || (tokens [i] == "\t"))
				return;
		}

		mLineNumber = tokens [0].ConvertToInteger ();
		mLocusName = tokens [1];

		rfu = tokens [2].ConvertToInteger ();
		residual = tokens [3].ConvertToDouble ();
		fit = tokens [4].ConvertToDouble ();
		newAllele = new LocusAllele (rfu, residual, fit);
		InsertAllele (newAllele);

		rfu = tokens [5].ConvertToInteger ();
		residual = tokens [6].ConvertToDouble ();
		fit = tokens [7].ConvertToDouble ();
		newAllele = new LocusAllele (rfu, residual, fit);
		InsertAllele (newAllele);
	}
}



LocusWithAlleles :: LocusWithAlleles (int lineNumber, const RGString& locusName) : RGPersistent (), mLineNumber (lineNumber), mLocusName (locusName), mMaxRFU (0) {

}



LocusWithAlleles :: ~LocusWithAlleles () {

	mAlleles.ClearAndDelete ();
}


void LocusWithAlleles :: OutputLocus (RGTextOutput& outFile) {

	outFile << mLineNumber << "\t";
	outFile << mLocusName << "\t\t";
	RGDListIterator it (mAlleles);
	LocusAllele* allele;

	while (allele = (LocusAllele*) it ()) {

		allele->OutputAllele (outFile);
		outFile << "\t\t";
	}

	outFile << "\n";
}


void LocusWithAlleles :: GetResidualDisplacementList (list<double>& rList) {

	RGDListIterator it (mAlleles);
	bool readFirst = false;
	LocusAllele* allele;
	double baseResidual = 0.0;
	double displacement;

	while (allele = (LocusAllele*) it ()) {

		if (readFirst) {

			displacement = baseResidual - allele->GetResidual ();
			rList.push_back (displacement);
		}

		else {

			baseResidual = allele->GetResidual ();
			readFirst = true;
		}
	}
}


void LocusWithAlleles :: GetResidualDisplacementList (list<double>& rList, RGTextOutput& outFile, RGString*& currentFileName) {

	RGDListIterator it (mAlleles);
	bool readFirst = false;
	bool erd = false;
	LocusAllele* allele;
	double baseResidual = 0.0;
	double displacement;

	while (allele = (LocusAllele*) it ()) {

		if (readFirst) {

			displacement = baseResidual - allele->GetResidual ();
			rList.push_back (displacement);

			if (fabs (displacement) > erdThreshold)
				erd = true;
		}

		else {

			baseResidual = allele->GetResidual ();
			readFirst = true;
		}
	}

	if (erd) {

		if (mFileNamePtr != currentFileName) {

			if (currentFileName != NULL)
				outFile << "\n";

			outFile << "File = " << (char*)mFileNamePtr->GetData () << "\n";
			outFile << "Line#\tLocus\t\tRFU\t\tResidual\t\tFit\t\t\tRFU\t\tResidual\t\tFit\n";
			currentFileName = mFileNamePtr;
		}

		OutputLocus (outFile);
	}
}



void LocusWithAlleles :: GetFitList (list<double>& fList) {

	RGDListIterator it (mAlleles);
	LocusAllele* allele;
	double fit = 0.0;

	while (allele = (LocusAllele*) it ()) {

		fit = allele->GetFit ();
		fList.push_back (fit);
	}
}



void LocusWithAlleles :: GetRFUList (list<int>& rfuList) {

	RGDListIterator it (mAlleles);
	LocusAllele* allele;
	int rfu = 0;

	while (allele = (LocusAllele*) it ()) {

		rfu = allele->GetRFU ();
		rfuList.push_back (rfu);
	}
}



void LocusWithAlleles :: InsertAllele (LocusAllele* newAllele) {

	int rfu = newAllele->GetRFU ();

	if (rfu > mMaxRFU) {

		mAlleles.Prepend (newAllele);
		mMaxRFU = rfu;
	}

	else
		mAlleles.Append (newAllele);
}



int LocusWithAlleles :: CompareTo (const RGPersistent* p) const {

	LocusWithAlleles* q = (LocusWithAlleles*) p;

	return mLocusName.CompareTo (&(q->mLocusName));
}



unsigned LocusWithAlleles :: HashNumber (unsigned long Base) const {

	return mLocusName.HashNumber (Base);
}



Boolean LocusWithAlleles :: IsEqualTo (const RGPersistent* p) const {

	LocusWithAlleles* q = (LocusWithAlleles*) p;

	return mLocusName.IsEqualTo (&(q->mLocusName));
}