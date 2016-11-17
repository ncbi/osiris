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
*  FileName: ParameterServer.h
*  Author:   Robert Goor
*
*/
//
//  Maintains connection to all Osiris-defined parameters and prints them out on request
//

//
//   Required Parameters:
//       ParametricCurve::FitTolerance gives minimum inner product for matching standard deviation; default = .9995;
//       Gaussian::NumberOfSteps gives number of numerical integration steps for Gaussian integrals; default = 50;
//       NormalizedGaussian::SigmaTolerance gives distance between two standard deviation estimates for match; default = 1.5e-4;
//       Gaussian::SigmaWidth gives number of standard deviations on each side of mean for standard Gaussian and NormalizedGaussian; default = 3.5;
//       NormalizedGaussian::MaximumIterations gives the max number of iterations of Brent's algorithm for fitting standard deviation; default = 5;
//		 DoubleGaussian::DefaultSigmaRatio gives default value of ratio of sigmas for 2 Gaussians; can be overridden; default = 4.0;
//       DataSignal::SignalSpacing gives value of spacing between signal observations; default = 1.0;
//

//
//    Required Parameters:
//       TracePrequalification::windowWidth gives number of samples of width of square wave for convolution; default = 20
//       TracePrequalification::noiseThreshold gives threshold on post-convolution peaks to consider them noise; default = 400.0;
//

#ifndef _PARAMETERSERVER_H_
#define _PARAMETERSERVER_H_

#include "rgdefs.h"
#include "rgstring.h"
#include "IndividualGenotype.h"
#include "SynonymList.h"
#include "RGLogBook.h"
#include "SmartMessage.h"
#include "BaseGenetics.h"
//#include "rgparray.h"

#include <list>

using namespace std;

class RGTextOutput;
class RGString;
class Locus;

enum ThresholdIdEnum {FRACTIONOFMAXPEAK, PULLUPFRACTIONALFILTER, STUTTERTHRESHOLD, 
						ADENYLATIONTHRESHOLD, HETEROZYGOUSIMBALANCELIMIT, MINBOUNDFORHOMOZYGOTE};

struct RFULimitsStruct {

	double minRFU;
	double maxRFU;
	double fractionOfMaxPeak;
	double pullupFractionOfMaxPeak;
	double stutterThreshold;
	double plusStutterThreshold;
	double adenylationThreshold;
};

struct labNonRFULimitsStruct {

	double heterozygousImbalanceLimit;
	double minBoundForHomozygote;
	int maxNumberPullupsPerSample;
	int maxNumberStutterPerSample;
	int maxNumberSpikesPerSample;
	int maxNumberAdenylationPerSample;
	int maxNumberOLAllelesPerSample;
	double maxResidualForAlleleCall;
	int minBPSForArtifacts;
	double alleleRFUOverloadThreshold;
	int maxNumberExcessiveResidualsPerSample;
};


struct channelThreshold {

	channelThreshold () : mChannel (0), mThreshold (0.0) {}
	channelThreshold (int c, double t) : mChannel (c), mThreshold (t) {}

	int mChannel;
	double mThreshold;
};

struct locusSpecificLimitsStruct {

	locusSpecificLimitsStruct ();
	locusSpecificLimitsStruct (const locusSpecificLimitsStruct& limits);
	~locusSpecificLimitsStruct ();

	void CopyPositiveThresholdsFrom (const locusSpecificLimitsStruct& limits);
	void Reset ();

	RGString locusName;
	double fractionOfMaxPeak;
	double pullupFractionalFilter;
	double stutterThreshold;
	double plusStutterThreshold;
	double adenylationThreshold;
	double heterozygousImbalanceLimit;
	double minBoundForHomozygote;
};


struct locusSpecificNonStandardStutterStruct {

	locusSpecificNonStandardStutterStruct ();
	locusSpecificNonStandardStutterStruct (const locusSpecificNonStandardStutterStruct& limits);
	~locusSpecificNonStandardStutterStruct ();

	void SetNonStandardStutterThreshold (int bp, double threshold);
	void Reset () { mPositiveNonStandardStutter.ResetArray (); mNegativeNonStandardStutter.ResetArray (); }

	RGString locusName;
	RGPArray mPositiveNonStandardStutter;
	RGPArray mNegativeNonStandardStutter;
};


class ParameterServer {

public:
	ParameterServer ();
	ParameterServer (const RGString& xmlString);
	~ParameterServer ();

	void* operator new (size_t);
	void operator delete (void*);

	bool isValid () const { return mValid; }
	bool AddGenotypeCollection (const RGString& xmlString, bool isLabSettings);

	int AddLadderLocusSpecificThreshold (const RGString& locusName, ThresholdIdEnum thresholdID, double value);
	int AddSampleLocusSpecificThreshold (const RGString& locusName, ThresholdIdEnum thresholdID, double value);

	int AddLadderLocusSpecificThreshold (const locusSpecificLimitsStruct& limits);
	int AddSampleLocusSpecificThreshold (const locusSpecificLimitsStruct& limits);
	int AddSampleLocusSpecificNonStandardStutterCollection (const locusSpecificNonStandardStutterStruct& nsLocusStutter);

	int SetSampleLocusSpecificThresholds (Locus* locus, locusSpecificLimitsStruct* limits);
	int SetLadderLocusSpecificThresholds (Locus* locus, locusSpecificLimitsStruct* limits);
	int SetAllLocusSpecificThresholds (PopulationCollection* collection);

	void SetMarkerSetName (const RGString& markerSetName) { *mMarkerSetName = markerSetName; }

	void SetParametricFitTolerance (double fit);
	double GetParametricFitTolerance () const;

	void SetNumberOfIntegrationSteps (int steps);
	int GetNumberOfIntegrationSteps () const;

	void SetGaussianSigmaTolerance (double tol);
	double GetGaussianSigmaTolerance () const;

	void SetGaussianSigmaWidth (double nSigmas);
	double GetGaussianSigmaWidth () const;

	void SetMaximumSearchIterations (int iterations);
	int GetMaximumSearchIterations () const;

	void SetDoubleGaussianSigmaRatio (double ratio);
	double GetDoubleGaussianSigmaRatio () const;

	void SetInterSampleSpacing (double spacing);
	double GetInterSampleSpacing () const;

	void SetSquareWaveWindowWidth (int width);
	int GetSquareWaveWindowWidth () const;

	void SetConvolutionNoiseThreshold (double threshold);
	double GetConvolutionNoiseThreshold () const;

	void SetChannelSpecificThresholds (list<channelThreshold*>* analysis, list<channelThreshold*>* detection) { mAnalysisThresholds = analysis; mDetectionThresholds = detection; }

	void Report (RGTextOutput& text, const RGString& indent);
	void ReportIfChanged (RGTextOutput& text, const RGString& indent);

	void WriteSettingsToDotOAR (RGLogBook& outputFile);

	GenotypeSet* GetGenotypeCollection () { return mSet; }
	RGString GetStandardPositiveControlName () const { return *mStandardPositiveControlName; }

	RGString BuildChannelThresholdOverridesForOAR ();
	RGString BuildChannelThresholdOverridesForPLT ();

	bool ReadAlgorithmParameters (const RGString& xmlString);
	bool ReadCurveFitParameters (const RGString& xmlString);
//	bool ReadLaneStdSearchParameters (const RGString& xmlString);
	bool ReadLadderLocusSearchParameters (const RGString& xmlString);
	bool ReadSearchSpacingParameters  (const RGString& xmlString, double& minMultiple, double& maxMultiple);
	bool ReadRFULimits (const RGString& xmlString, RFULimitsStruct& rfuLimits);
	bool ReadLadderLabLimits (const RGString& xmlString, RFULimitsStruct& rfuLimits);
	bool ReadSampleLabLimits (const RGString& xmlString, RFULimitsStruct& rfuLimits);
	bool ReadLabNonRFULimits (size_t startOffset, size_t& endOffset, const RGString& xmlString, labNonRFULimitsStruct& nonRFULimits);
	//int ReadSampleTypeSpecifications (size_t startOffset, size_t& endOffset, const RGString& xmlString);
	//int ReadSampleTypeSpec (const RGString& xmlString);
	bool SetAllSmartMessageThresholds ();

	bool LadderDoesTargetStringEqualASynonymCaseIndep (const RGString& target);
	bool LadderDoesTargetStringContainASynonymCaseIndep (const RGString& target);

	bool ControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target);
	bool ControlDoesTargetStringContainASynonymCaseIndep (const RGString& target);
	bool AddLabPositiveControlsToControlStrings (GenotypesForAMarkerSet* genotypes);

	bool PosControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target);
	bool PosControlDoesTargetStringContainASynonymCaseIndep (const RGString& target);

	bool NegControlDoesTargetStringEqualASynonymCaseIndep (const RGString& target);
	bool NegControlDoesTargetStringContainASynonymCaseIndep (const RGString& target);

	bool DoesTargetStringEqualMixtureCriteriaCaseIndep (const RGString& target);
	bool DoesTargetStringContainMixtureCriteriaCaseIndep (const RGString& target);

	int GetMaxPullups () const { return mMaxPullups; }
	int GetMaxStutter () const { return mMaxStutter; }
	int GetMaxSpikes () const { return mMaxSpikes; }
	int GetMaxAdenylation () const { return mMaxAdenylations; }
	int GetMaxOLAlleles () const { return mMaxOLAlleles; }
	int GetMaxExcessiveResiduals () const { return mMaxExcessiveResiduals; }

	static void Kill() {

		if(OneAndOnlySelf != NULL) {

			ReferenceCount = 1; // problem with reference count
			delete OneAndOnlySelf;
			OneAndOnlySelf = NULL;
		}
  }

  static void UnescapeXML (RGString& target);

protected:
	static int ReferenceCount;
	static ParameterServer* OneAndOnlySelf;

	GenotypeSet* mSet;
	bool mValid;

	SynonymList* LadderIDs;
	SynonymList* ControlIDs;
	SynonymList* PositiveControlIDs;
	SynonymList* NegativeControlIDs;
	SynonymList* mSingleSourceSampleIDs;
	SynonymList* mMixtureIDs;
	RGString* mStandardPositiveControlName;
	RGString* mMarkerSetName;

	int mMaxPullups;
	int mMaxStutter;
	int mMaxSpikes;
	int mMaxAdenylations;
	int mMaxOLAlleles;
	int mMaxExcessiveResiduals;

	Boolean Changed;

	RGString* mStandardSettingsString;
	RGString* mLabSettingsString;

	RGDList* mSmartMessageThresholds;
	list<locusSpecificLimitsStruct*>* mSampleLocusSpecificThresholds;
	list<locusSpecificNonStandardStutterStruct*>* mSampleNonStandardStutterThresholds;
	list<locusSpecificLimitsStruct*>* mLadderLocusSpecificThresholds;
	list<channelThreshold*>* mAnalysisThresholds;
	list<channelThreshold*>* mDetectionThresholds;

	int mNumberOfChannels;

	bool ReadFileNameStrings (const RGString& xmlString);
};


class ParameterServerKill
{
public:
  ParameterServerKill() {;}
  ~ParameterServerKill()
  {
    ParameterServer::Kill();
  }
};


#endif  /*  _PARAMETERSERVER_H_   */

