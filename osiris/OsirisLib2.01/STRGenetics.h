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
*  FileName: STRGenetics.h
*  Author:   Robert Goor
*
*/
//
//  STR Genetics classes, subclasses of those in Genetics.h
//

#ifndef _STRGENETICS_H_
#define _STRGENETICS_H_

#include "rgdlist.h"
#include "Genetics.h"

class CoordinateTransform;
class ChannelData;
class Locus;

const int _STRBASEALLELE_ = 1035;
const int _STRBASELOCUS_ = 1036;
const int _STRBASEPOPULATIONMARKERSET_ = 1037;
const int _STRBASELANESTANDARD_ = 1051;


PERSISTENT_PREDECLARATION (STRBaseAllele)
PERSISTENT_PREDECLARATION (STRBaseLocus)
PERSISTENT_PREDECLARATION (STRBasePopulationMarkerSet)
PERSISTENT_PREDECLARATION (STRBaseLaneStandard)



class STRBaseAllele : public BaseAllele {

PERSISTENT_DECLARATION (STRBaseAllele)

public:
	STRBaseAllele ();
	STRBaseAllele (const RGString& alleleName, int curve, int bp);
	STRBaseAllele (const RGString& xmlInput);
	virtual ~STRBaseAllele ();

	virtual void SetBioID (int id);
	virtual int GetBioID () const;

	virtual DataSignal* GetNewPrimarySignature (const DataSignal& ds, CoordinateTransform* trans);

	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& f);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& f) const;

protected:
	int BP;  // value in base pairs
};


class STRBaseLocus : public BaseLocus {

	PERSISTENT_DECLARATION (STRBaseLocus)
		
public:
	STRBaseLocus ();
	STRBaseLocus (const RGString& xmlInput);
	virtual ~STRBaseLocus ();

	bool GetLadderSearchRegion (size_t& startIndex, RGString& input, const RGString& familyName);

	virtual int GetMinimumBound () const;
	virtual int GetMaximumBound () const;
	virtual Boolean ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus);
	virtual RGString ReconstructAlleleName (int id, Allele* nearAllele);
	virtual RGString GetOffGridMessage () const;

	virtual void SetSampleStutterThreshold (double limit) { mSampleLocusSpecificStutterThreshold = limit; }
	virtual void SetSamplePlusStutterThreshold (double limit) { mSampleLocusSpecificPlusStutterThreshold = limit; }
	virtual void SetSampleAdenylationThreshold (double limit) { mSampleLocusSpecificAdenylationThreshold = limit; }
	virtual void SetSampleFractionalFilter (double limit) { mSampleLocusSpecificFractionalFilter = limit; }
	virtual void SetSamplePullupFractionalFilter (double limit) { mSampleLocusSpecificPullupFractionalFilter = limit; }
	virtual void SetSampleHeterozygousImbalanceThreshold (double limit) { mSampleLocusSpecificHeterozygousImbalanceThreshold = limit; }
	virtual void SetSampleMinBoundForHomozygote (double limit) { mSampleLocusSpecificMinBoundForHomozygote = limit; }

	virtual void SetLadderStutterThreshold (double limit) { mLadderLocusSpecificStutterThreshold = limit; }
	virtual void SetLadderAdenylationThreshold (double limit) { mLadderLocusSpecificAdenylationThreshold = limit; }
	virtual void SetLadderFractionalFilter (double limit) { mLadderLocusSpecificFractionalFilter = limit; }
	virtual void SetLadderPullupFractionalFilter (double limit) { mLadderLocusSpecificPullupFractionalFilter = limit; }

	virtual double GetSampleStutterThreshold () const { return mSampleLocusSpecificStutterThreshold; }
	virtual double GetSamplePlusStutterThreshold () const { return mSampleLocusSpecificPlusStutterThreshold; }
	virtual double GetSampleAdenylationThreshold () const { return mSampleLocusSpecificAdenylationThreshold; }
	virtual double GetSampleFractionalFilter () const { return mSampleLocusSpecificFractionalFilter; }
	virtual double GetSamplePullupFractionalFilter () const { return mSampleLocusSpecificPullupFractionalFilter; }
	virtual double GetSampleHeterozygousImbalanceThreshold () const { return mSampleLocusSpecificHeterozygousImbalanceThreshold; }
	virtual double GetSampleMinBoundForHomozygote () const { return mSampleLocusSpecificMinBoundForHomozygote; }

	virtual double GetLadderStutterThreshold () const { return mLadderLocusSpecificStutterThreshold; }
	virtual double GetLadderAdenylationThreshold () const { return mLadderLocusSpecificAdenylationThreshold; }
	virtual double GetLadderFractionalFilter () const { return mLadderLocusSpecificFractionalFilter; }
	virtual double GetLadderPullupFractionalFilter () const { return mLadderLocusSpecificPullupFractionalFilter; }

	virtual double GetMinimumBPILS () const { return LowerBoundGridLSBasePair; }

	virtual int GetCoreNumber () const;
	virtual bool AllowsNoExtension () const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual BaseAllele* GetNewAllele (const RGString& input);

	// Smart Message methods*************************************************************************************************************************************

	virtual Boolean ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus);

	//***********************************************************************************************************************************************************

protected:
//	RGDList CharacteristicList;
//	CoordinateTransform* BioIDToTimeMap;
	int MinimumBP;  // Minimum allele found associated with locus
	int MaximumBP;  // Maximum allele found associated with locus
	double LowerBoundGridLSIndex;  // Minimum index (or fraction) in Internal Lane Standard to bound ladder alleles
	double UpperBoundGridLSIndex;  // Maximum index (or fraction) in Internal Lane Standard to bound ladder alleles
	double MinimumGridTime;
	double MaximumGridTime;
	int CoreRepeatNumber;
	double LowerBoundGridLSBasePair;
	double UpperBoundGridLSBasePair;

	double mSampleLocusSpecificStutterThreshold;
	double mSampleLocusSpecificPlusStutterThreshold;
	double mSampleLocusSpecificAdenylationThreshold;
	double mSampleLocusSpecificFractionalFilter;
	double mSampleLocusSpecificPullupFractionalFilter;
	double mSampleLocusSpecificHeterozygousImbalanceThreshold;
	double mSampleLocusSpecificMinBoundForHomozygote;

	double mLadderLocusSpecificStutterThreshold;
	double mLadderLocusSpecificAdenylationThreshold;
	double mLadderLocusSpecificFractionalFilter;
	double mLadderLocusSpecificPullupFractionalFilter;

	bool mNoExtension;
};


class STRBasePopulationMarkerSet : public BasePopulationMarkerSet {

	PERSISTENT_DECLARATION (STRBasePopulationMarkerSet)

public:
	STRBasePopulationMarkerSet ();
	STRBasePopulationMarkerSet (const RGString& xmlString);
	~STRBasePopulationMarkerSet ();

//	virtual int AnalyzeLoci ();
//	int BuildILSSplineBPToTime (const SupplementaryData& extraData);  // ???
	
//	const DataSignal* GetCurve (const RGString& locus, const RGString& allele, int& channel);
//	double GetTimeForCorrespondingBioID (int bioID, int channel);  // ????

	virtual BaseLocus* GetNewLocus (const RGString& input);

protected:
	
};


class STRPopulationCollection : public PopulationCollection {

public:
	STRPopulationCollection (RGFile& inputFile);
	STRPopulationCollection (const RGString& inputFileName);
	STRPopulationCollection (const RGString& inputDirectoryName, const RGString& markerSetName);
	virtual ~STRPopulationCollection ();

	virtual BasePopulationMarkerSet* GetNewPopulationMarkerSet (const RGString& input);
	virtual LaneStandardCollection* GetNewLaneStandardCollection (const RGString& textInput);

protected:
	
};


class STRBaseLaneStandard : public BaseLaneStandard {

	PERSISTENT_DECLARATION (STRBaseLaneStandard)

public:
	STRBaseLaneStandard ();
	STRBaseLaneStandard (const RGString& xmlString);
	~STRBaseLaneStandard ();

	virtual int GetCharacteristicArray (const double*& array) const;  // returns array size or -1
	virtual int GetCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1
	virtual int GetUnnormalizedCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1

	virtual int GetLargeCharacteristicArray (const double*& array) const;  // returns array size or -1
	virtual int GetLargeDifferenceArray (const double*& array) const;  // returns array size or -1

	virtual double GetFirstIntervalFraction () const;
	virtual double GetSmallestIntervalFraction () const;

	virtual int GetMaxRelativeHeight () const;
	virtual double GetCorrelationAcceptanceThreshold () const;
	virtual double GetCorrelationAutoAcceptanceThreshold () const;
	virtual bool HasAltSpacingFormula () const { return (mAltSpacingFormula.Length () > 0); }
	virtual double* GetAltSpacingArray ();

	//virtual int GetLastHalfCharacteristicArray (const double*& array) const;  // returns array size or -1
	//virtual int GetLastHalfUnnormalizedCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1
	//virtual int GetLastHalfRelativeSizes (const int*& array) const;  // returns array size, or -1
	//virtual int GetLastHalfNormsLeft (const double*& array) const;  // returns array size, or -1
	//virtual int GetLastHalfNormsRight (const double*& array) const;  // returns array size, or -1

	virtual int GetNormsLeft (const double*& array) const;  // returns array size, or -1
	virtual int GetNormsRight (const double*& array) const;  // returns array size, or -1
	virtual int GetRelativeSizes (const int*& array) const;  // returns array size, or 0, or -1
	virtual int GetOmissionArray (const double*& array) const;  // returns array size, or -1

protected:
	double* mCharacteristicArray;
	double* mDifferenceArray;
	double* mUnnormalizedDifferenceArray;
	double* mNormsLeft;
	double* mNormsRight;
	int* mRelativeSizes;
	int mSize;
	int mDifferenceSize;
	int mOmissionSize;
	double* mOmissionArray;

	int mNumberOfLargePeaks;
	double* mLargeCharacteristicArray;
	double* mLargeDifferenceArray;
	int mNumberOfLargeDifferences;
	double mFirstIntervalFraction;
	double mSmallestIntervalFraction;

	int mMaxRelativeHeight;
	double mCorrelationAcceptanceThreshold;
	double mCorrelationAutoAcceptanceThreshold;
	RGString mAltSpacingFormula;
	double* mAltSpacingArray;
	double* mAltSpacingCoefficients;
	int mFormulaTerms;

	//int mLastHalfSize;
	//double* mLastHalfCharacteristicArray;
	//double* mLastHalfUnnormalizedDifferenceArray;
	//int* mLastHalfRelativeSizes;
	//int mLastHalfDifferenceSize;
	//double* mLastHalfNormsLeft;
	//double* mLastHalfNormsRight;
};


class STRLaneStandardCollection : public LaneStandardCollection {

public:
	STRLaneStandardCollection (const RGString& xmlString);
	virtual ~STRLaneStandardCollection ();

	virtual BaseLaneStandard* GetNewBaseLaneStandard (const RGString& input);

protected:

};



#endif  /*  _STRGENETICS_H_  */

