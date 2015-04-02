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

#ifndef _LOCUSALLELES_H_
#define _LOCUSALLELES_H_


#include "rgpersist.h"
#include "rgdlist.h"
#include "rgstring.h"
#include "rgtokenizer.h"
#include "rgdefs.h"
#include "RGTextOutput.h"

#include <list>

const int _LOCUSALLELE_ = 10051;
const int _LOCUSWITHALLELES_ = 10052;


PERSISTENT_PREDECLARATION (LocusAllele)
PERSISTENT_PREDECLARATION (LocusWithAlleles)

class LocusAllele : public RGPersistent {

	PERSISTENT_DECLARATION (LocusAllele)

public:
	LocusAllele ();
	LocusAllele (int rfu, double residual, double fit);
	LocusAllele (const LocusAllele& locusAllele);
	~LocusAllele ();

	void SetRFU (int rfu) { mRFU = rfu; }
	void SetResidual (double residual) { mResidual = residual; }
	void SetFit (double fit) { mFit = fit; }

	int GetRFU () const { return mRFU; }
	double GetResidual () const { return mResidual; }
	double GetFit () { return mFit; }

	void OutputAllele (RGTextOutput& outFile);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	int mRFU;
	double mResidual;
	double mFit;
};


class LocusWithAlleles : public RGPersistent {

	PERSISTENT_DECLARATION (LocusWithAlleles)

public:
	LocusWithAlleles ();
	LocusWithAlleles (const RGString& inputLine);
	LocusWithAlleles (int lineNumber, const RGString& locusName);
	~LocusWithAlleles ();

	bool HasAtLeastTwoAlleles () const { return (mAlleles.Entries () > 1); }

	void SetLocusName (const RGString& name) { mLocusName = name; }
	void SetLineNumber (int no) { mLineNumber = no; }
	void SetFileNamePtr (RGString* fileName) { mFileNamePtr = fileName; }
	void OutputLocus (RGTextOutput& outFile);

	int GetLineNumber () const { return mLineNumber; }
	RGString GetLocusName () const { return mLocusName; }
	void GetResidualDisplacementList (list<double>& rList);
	void GetResidualDisplacementList (list<double>& rList, RGTextOutput& outFile, RGString*& currentFileName);
	void GetFitList (list<double>& fList);
	void GetRFUList (list<int>& rfuList);
	void InsertAllele (LocusAllele* newAllele);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	int mLineNumber;
	RGString mLocusName;
	RGDList mAlleles;
	int mMaxRFU;
	RGString* mFileNamePtr;
};


#endif	/*  _LOCUSALLELES_H_  */


