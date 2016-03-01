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
*  FileName: BaseGenetics.h
*  Author:   Robert Goor
*
*/
//
//  classes BaseAllele, BaseLocus and BasePopulationMarkerSet...see Genetics.h
//

#ifndef _BASEGENETICS_H_
#define _BASEGENETICS_H_

#include "rgpersist.h"
#include "rgstring.h"
#include "rgpscalar.h"
#include "rgdefs.h"
#include "rghashtable.h"
#include "rgdlist.h"
#include "SmartMessagingObject.h"


class DataSignal;
class RGFile;
class RGVInStream;
class RGVOutStream;
struct SupplementaryData;
class LaneStandard;
class Allele;
class CoordinateTransform;
class ChannelData;
class Locus;



class BaseAllele : public RGPersistent {

ABSTRACT_DECLARATION (BaseAllele)

public:
	BaseAllele ();
	BaseAllele (const RGString& alleleName, int curve);
	BaseAllele (const RGString& xmlInput);
	virtual ~BaseAllele ();

	static void SetSearchByName () { SearchByName = TRUE; }
	static void SetSearchByCurve () { SearchByName = FALSE; }

	Boolean IsValid () const { return Valid; }
	RGString GetError () const { return Msg; }

	RGString GetAlleleName () const { return AlleleName; }
	int GetCurveNumber () const { return Curve; }
	int GetRelativeHeight () const { return mRelativeHeight; }

	void SetAlleleName (const RGString& name) { AlleleName = name; }
	void SetCurve (int curve) { Curve = curve; }

	virtual void SetBioID (int id);
	virtual int GetBioID () const;

	virtual DataSignal* GetNewPrimarySignature (const DataSignal& ds, CoordinateTransform* trans);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

protected:
//	int LocusId;   // ???
	RGString AlleleName;
	int Curve;  // curve number within locus
//	int Channel;   // ???
	Boolean Valid;
	RGString Msg;
	int mRelativeHeight;

	static Boolean SearchByName;
};


class BaseLocus : public SmartMessagingObject {

	ABSTRACT_DECLARATION (BaseLocus)

public:
	BaseLocus ();
	BaseLocus (const RGString& xmlInput);
	virtual ~BaseLocus ();

	Boolean IsValid () const { return Valid; }
	RGString GetLocusName () const { return LocusName; }
	void SetLocusName (const RGString& name) { LocusName = name; }
	int GetChannel () const { return Channel; }
	void SetChannel (int channel) { Channel = channel; }
	void SetTableLink (int linkNumber);

	bool isYLinked () const { return mIsYLinked; }
	int GetMaxExpectedAlleles () const { return mMaxExpectedAlleles; }
	int GetMinExpectedAlleles () const { return mMinExpectedAlleles; }

	void SetYLinked (bool linked) { mIsYLinked = linked; }
	void SetMaxExpectedAlleles (int max) { mMaxExpectedAlleles = max; }
	void SetMinExpectedAlleles (int min) { mMinExpectedAlleles = min; }

	RGString GetError () const { return Msg; }

	void SetLocusVector (RGDList& alleleList);
	const double* GetDifferenceVector () const { return mDifferenceVector; }
	const double* GetLocusVector () const { return mLocusVector; }
	const double* GetUnnormalizedDifferenceVector () const { return mUnnormalizedDifferenceVector; }
	int GetDifferenceVectorSize () const { return mDifferenceSize; }
	int GetLocusVectorSize () const { return mLocusSize; }
	const int* GetRelativeHeights () const { return mRelativeHeights; }
	const double* GetNormsLeft () const { return mNormsLeft; }
	const double* GetNormsRight () const { return mNormsRight; }

	int SelectBestSubsetOfCharacteristics (RGDList& curveList, double& correlation);
	virtual Boolean ExtractGridSignals (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus);

	virtual RGString ReconstructAlleleName (int id, Allele* nearAllele);
	virtual RGString GetOffGridMessage () const;

	virtual int GetMinimumBound () const;
	virtual int GetMaximumBound () const;
	virtual double GetMinGridID () const;
	virtual double GetMaxGridID () const;
	virtual int GetCoreNumber () const;
	virtual bool AllowsNoExtension () const;

	virtual void SetSampleStutterThreshold (double limit) { }
	virtual void SetSamplePlusStutterThreshold (double limit) { }
	virtual void SetSampleAdenylationThreshold (double limit) { }
	virtual void SetSampleFractionalFilter (double limit) { }
	virtual void SetSamplePullupFractionalFilter (double limit) { }
	virtual void SetSampleHeterozygousImbalanceThreshold (double limit) { }
	virtual void SetSampleMinBoundForHomozygote (double limit) { }

	virtual void SetLadderStutterThreshold (double limit) { }
	virtual void SetLadderAdenylationThreshold (double limit) { }
	virtual void SetLadderFractionalFilter (double limit) { }
	virtual void SetLadderPullupFractionalFilter (double limit) { }

	virtual double GetSampleStutterThreshold () const { return 0.0; }
	virtual double GetSamplePlusStutterThreshold () const { return 0.0; }
	virtual double GetSampleAdenylationThreshold () const { return 0.0; }
	virtual double GetSampleFractionalFilter () const { return 0.0; }
	virtual double GetSamplePullupFractionalFilter () const { return 0.0; }
	virtual double GetSampleHeterozygousImbalanceThreshold () const { return 0.0; }
	virtual double GetSampleMinBoundForHomozygote () const { return 0.0; }

	virtual double GetLadderStutterThreshold () const { return 0.0; }
	virtual double GetLadderAdenylationThreshold () const { return 0.0; }
	virtual double GetLadderFractionalFilter () const { return 0.0; }
	virtual double GetLadderPullupFractionalFilter () const { return 0.0; }

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual BaseAllele* GetNewAllele (const RGString& input);

	// Smart Message methods*************************************************************************************************************************************

	virtual Boolean ExtractGridSignalsSM (RGDList& channelSignalList, const LaneStandard* ls, RGDList& locusDataSignalList, ChannelData* lsData, Locus* locus);
	virtual int GetObjectScope () const { return 5; }
	virtual void OutputDebugID (SmartMessagingComm& comm, int numHigherObjects);
	virtual int AddAllSmartMessageReporters ();
	virtual int AddAllSmartMessageReporters (SmartMessagingComm& comm, int numHigherObjects);

	virtual void InitializeMessageData ();

	bool EvaluateSmartMessagesForStage (SmartMessagingComm& comm, int numHigherObjects, int stage);
	bool SetTriggersForAllMessages (SmartMessagingComm& comm, int numHigherObjects, int stage);

	Boolean ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);
	Boolean ReportSmartNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim, Boolean reportLink);
	Boolean ReportXMLSmartNoticeObjects (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);
	Boolean ReportXMLSmartNoticeObjectsWithExport (RGTextOutput& text, RGTextOutput& tempText, const RGString& delim);

	static void CreateInitializationData (int scope);
	static void InitializeMessageMatrix (bool* matrix, int size);
	static void ClearInitializationMatrix () { delete[] InitialMatrix; InitialMatrix = NULL; }
	static int GetScope () { return 2; }

	static RGString GetILSFamilyName () { return ILSFamilyName; }
	static void SetILSFamilyName (const RGString& name) { ILSFamilyName = name; }

	//***********************************************************************************************************************************************************

protected:
	RGString LocusName;
	int Channel;
	Boolean Valid;
	double* mDifferenceVector;
	double* mLocusVector;
	int mDifferenceSize;
	int mLocusSize;
	double mMinGridID;
	double mMaxGridID;
	double* mUnnormalizedDifferenceVector;
	int* mRelativeHeights;
	double* mNormsLeft;
	double* mNormsRight;

	RGString Msg;
	RGString mTableLink;
	bool mIsYLinked;
	int mMaxExpectedAlleles;
	int mMinExpectedAlleles;

	static bool* InitialMatrix;
	static RGString ILSFamilyName;

//	RGHashTable AlleleListByName;   // items of type BaseAllele*
//	RGHashTable AlleleListByCurve;   // items of type BaseAllele*
//	RGDList AlleleList;
//	RGDListIterator* AlleleIterator;
//	RGDList CharacteristicList;
//	CoordinateTransform* BioIDToTimeMap;
};


class BasePopulationMarkerSet : public RGPersistent {

	ABSTRACT_DECLARATION (BasePopulationMarkerSet)

public:
	BasePopulationMarkerSet ();
	BasePopulationMarkerSet (const RGString& xmlString);
	virtual ~BasePopulationMarkerSet ();

	Boolean IsValid () const { return Valid; }
	RGString GetError () const { return Msg; }

	Boolean RepresentsMarkerSet (const RGString& markerSet) const;
	RGString GetMarkerSetName () const { return MarkerSetName; }
	int GetNumberOfChannels () const { return NChannels; }
	RGString GetLaneStandardName () const {return LaneStandardName; }
	int GetLaneStandardChannelNumber () const { return LSChannelNumber; }

	RGString GetFileNameSuffix () const { return FileNameSuffix; }
	RGString GetGenotypeSuffix () const { return GenotypeSuffix; }
	RGString GetDirectorySearchString () const { return DirectorySearchString; }
	const int* GetChannelMap () const { return mChannelMap; }

	void SetMarkerSetName (const RGString& name) { MarkerSetName = name; LCMarkerSetName = name; 
					LCMarkerSetName.ToLower (); }
	BaseLocus* FindLocus (const RGString& name);

//	virtual int AnalyzeLoci ();
//	int BuildILSSplineBPToTime (const SupplementaryData& extraData);  // ???
	
//	const DataSignal* GetCurve (const RGString& locus, const RGString& allele, int& channel);
//	double GetTimeForCorrespondingBioID (int bioID, int channel);  // ????

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	virtual BaseLocus* GetNewLocus (const RGString& input);

	static void SetUserLaneStandardName (const RGString& name) { UserLaneStandardName = name; }
	static RGString GetUserLaneStandardName () { return UserLaneStandardName; }

protected:
	RGString MarkerSetName;
	RGString LCMarkerSetName;
	int NChannels;
	RGString LaneStandardName;
	int LSChannelNumber;
	RGString FileNameSuffix;
	RGString GenotypeSuffix;
	RGString DirectorySearchString;
//	RGHashTable LocusList;  // contains objects of type BaseLocus*
//	RGHashTable AlleleListByName;
//	RGHashTable AlleleListByChannelCurve;
	Boolean Valid;
	RGString Msg;
	int* mChannelMap;
//	CoordinateTransform** splineBPToTime;
	static RGString UserLaneStandardName;
};



class BaseLaneStandard : public RGPersistent {

	ABSTRACT_DECLARATION (BaseLaneStandard)

public:
	BaseLaneStandard ();
	BaseLaneStandard (const RGString& xmlString);
	virtual ~BaseLaneStandard ();

	void SetName (const RGString& name) { Name = name; }

	Boolean IsValid () const { return Valid; }
	RGString GetError () const { return Msg; }
	RGString GetLaneStandardName () const { return Name; }
	int GetNumberOfCharacteristics () const { return NCharacteristics; }
	double GetMinimumCharacteristic () const { return MinCharacteristic; }
	double GetMaximumCharacteristic () const { return MaxCharacteristic; }
	int GetLaneStandardChannel () const { return mChannel; }
//	virtual const double* GetCharacteristicArray () const;
//	virtual const double* GetCharacteristicDifferenceArray () const;

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

	virtual int GetNormsLeft (const double*& array) const;  // returns array size, or -1
	virtual int GetNormsRight (const double*& array) const;  // returns array size, or -1
	virtual int GetRelativeSizes (const int*& array) const;  // returns array size, or 0, or -1
	virtual int GetOmissionArray (const double*& array) const;  // returns array size, or -1

	//virtual int GetLastHalfCharacteristicArray (const double*& array) const;  // returns array size or -1
	//virtual int GetLastHalfUnnormalizedCharacteristicDifferenceArray (const double*& array) const;  // returns array size or -1
	//virtual int GetLastHalfRelativeSizes (const int*& array) const;  // returns array size, or -1
	//virtual int GetLastHalfNormsLeft (const double*& array) const;  // returns array size, or -1
	//virtual int GetLastHalfNormsRight (const double*& array) const;  // returns array size, or -1

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

protected:
	RGString Name;
	int NCharacteristics;
	double MaxCharacteristic;
	double MinCharacteristic;
	int mChannel;
	Boolean Valid;
	RGString Msg;
};



#endif  /*  _BASEGENETICS_H_  */

