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

*  FileName: CStandardSettings.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_STANDARD_SETTINGS_H__
#define __C_STANDARD_SETTINGS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"


class CCurveFitParameters : public nwxXmlPersist
{
public:
  CCurveFitParameters()
  {
    RegisterAll(true);
  }
  virtual ~CCurveFitParameters(){;}


  virtual void RegisterAll(bool = false)
  {
    RegisterDouble("NoiseThreshold",            &m_dNoiseThreshold);
    RegisterInt(   "WindowWidth",               &m_nWindowWidth);
    RegisterDouble("MinFitForNormalPeak",       &m_dMinFitForNormalPeak);
    RegisterDouble("TriggerFitForArtifact",     &m_dTriggerFitForArtifact);
    RegisterDouble("AbsoluteMinimumFit",        &m_dAbsoluteMinimumFit);
    RegisterBool(  "TestForNegSecondaryContent",&m_bTestForNegSecondaryContent);
    RegisterDouble("ParametricFitTolerance",    &m_dParametricFitTolerance);
    RegisterInt(   "NumberOfIntegrationSteps",  &m_nNrIntegrationSteps);
    RegisterDouble("SigmaTolerance",            &m_dSigmaTolerance);
    RegisterDouble("SigmaWidth",                &m_dSigmaWidth);
    RegisterInt(   "MaximumIterations",         &m_nMaxIterations);
    RegisterDouble("SigmaRatio",                &m_dSigmaRatio);
    RegisterDouble("InterSampleSpacing",        &m_dInterSampleSpacing);
    RegisterDouble("SignatureSigma",            &m_dSignatureSigma);
    RegisterInt(   "BlobDegree",                &m_nBlobDegree);
    RegisterDouble("MinDistanceBetweenPeaks",   &m_dMinDistBtwnPeaks);
    RegisterDouble("PeakFractionForEndPtTest",  &m_dPeakFractionForEndPtTest);
    RegisterDouble("PeakLevelForEndPtTest",     &m_dPeakLevelForEndPtTest);
    RegisterDouble("LowTailHeightThreshold",    &m_dLowTailHeightThreshold);
    RegisterDouble("LowTailSlopeThreshold",     &m_dLowTailSlopeThreshold);
    RegisterInt("MinSamplesForSlopeRegression", &m_nMinSamplesForSlopeRegression);
  }


  double m_dLowTailHeightThreshold;
  double m_dLowTailSlopeThreshold;
  double m_dAbsoluteMinimumFit;
  double m_dNoiseThreshold;
  double m_dMinFitForNormalPeak;
  double m_dTriggerFitForArtifact;
  double m_dParametricFitTolerance;
  double m_dSigmaTolerance;
  double m_dSigmaWidth;
  double m_dSigmaRatio;
  double m_dInterSampleSpacing;
  double m_dSignatureSigma;
  double m_dMinDistBtwnPeaks;
  double m_dPeakFractionForEndPtTest;
  double m_dPeakLevelForEndPtTest;
  int m_nWindowWidth;
  int m_nNrIntegrationSteps;
  int m_nMaxIterations;
  int m_nBlobDegree;
  int m_nMinSamplesForSlopeRegression;
  bool m_bTestForNegSecondaryContent;
};


class CControlSearchBounds  : public nwxXmlPersist
{
public:
  CControlSearchBounds()
  {
    RegisterAll(true);
  }
  virtual ~CControlSearchBounds() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterDouble("MaxSpacingMultiple",&m_dMax);
    RegisterDouble("MinSpacingMultiple",&m_dMin);
  }
  double m_dMax;
  double m_dMin;
};

class CLaneStandardSearch : public nwxXmlPersist
{
public:
  class CILSSpecificThreshold : public nwxXmlPersist
  {
  public:
    CILSSpecificThreshold()
    {
      RegisterAll(true);
    }
    virtual ~CILSSpecificThreshold() {;}
    virtual void RegisterAll(bool = false)
    {
      RegisterWxString("Synonym",&m_sSynonym);
      RegisterDouble("CorrelationWiithIdeal",&m_dCorr);
    }
    wxString m_sSynonym;
    double m_dCorr;
  };
  class CCorrelationThresholds : public TnwxXmlPersistVector<CILSSpecificThreshold>
  {
  public:
    CCorrelationThresholds() : 
        TnwxXmlPersistVector<CILSSpecificThreshold>(
          "ILSSpecificThreshold" ) {;}
    virtual ~CCorrelationThresholds() {;}
  };

  //  end of contained classes

  CLaneStandardSearch()
  {
    RegisterAll(true);
  }
  virtual ~CLaneStandardSearch() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterDouble("AcceptanceFactorOfMaxPeak", &m_dAcceptanceFactorOfMaxPeak);
    RegisterDouble("EndTestFactor", &m_dEndTestFactor);
    RegisterDouble("SkipFractionOfInitialPeaks", &m_dSkipFractionOfInitialPeaks);
    RegisterDouble("MinCorrelationForRecursiveSearch", &m_dMinCorrelationForRecursiveSearch);
    RegisterInt("MaxStartPtsForPartialSearch", &m_nMaxStartPtsForPartialSearch);
    RegisterDouble("BeginAnalysisProportion", &m_dBeginAnalysisProportion);
    Register("LaneStandardSearchSpacing", &m_LaneStandardSearchSpacing);
    Register("CorrelationThresholds", &m_CorrelationThresholds);
  }

  CControlSearchBounds m_LaneStandardSearchSpacing;
  CCorrelationThresholds m_CorrelationThresholds;
  double m_dAcceptanceFactorOfMaxPeak;
  double m_dEndTestFactor;
  double m_dSkipFractionOfInitialPeaks;
  double m_dMinCorrelationForRecursiveSearch;
  double m_dBeginAnalysisProportion;
  int m_nMaxStartPtsForPartialSearch;

};

class CLadderLocusSearch : public nwxXmlPersist
{
public:
  CLadderLocusSearch()
  {
    RegisterAll(true);
  }
  virtual ~CLadderLocusSearch() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterDouble("LadderLociMinimumCorrelationWithIdeal",&m_dLadderLociMinimumCorrelation);
    Register("LadderLocusSearchSpacing",&m_LadderLocusSearchSpacing);
    RegisterInt("MaxStartPtsForPartialSearch",&m_nMaxStartPtsForPartialSearch);
  }
  CControlSearchBounds m_LadderLocusSearchSpacing;
  double m_dLadderLociMinimumCorrelation;
  int m_nMaxStartPtsForPartialSearch;
};


class CAlgorithmParameters : public nwxXmlPersist
{
public:
  CAlgorithmParameters()
  {
    RegisterAll(true);
  }
  virtual ~CAlgorithmParameters() {;}
  virtual void RegisterAll(bool = false)
  {
    Register("CurveFitParameters",&m_curveFit);
    Register("LaneStandardSearch",&m_laneStd);
    Register("LadderLocusSearch",&m_ladderLocus);
  }
  CCurveFitParameters m_curveFit;
  CLaneStandardSearch m_laneStd;
  CLadderLocusSearch m_ladderLocus;
};

class CStandardParameters : public nwxXmlPersist
{
public:
  CStandardParameters() 
  {
    RegisterAll(true);
  }
  virtual ~CStandardParameters() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterInt("MsgDoubleResolution", &m_nMsgDoubleResolution);
    RegisterDouble("MinSigmaForDyeBlob", &m_dMinSigmaForDyeBlob);
    RegisterDouble("SecondaryContentLimitFraction", &m_dSecondaryContentLimitFraction);
    RegisterDouble("MinFractionExpectedPeakWidth", &m_dMinFractionExpectedPeakWidth);
    RegisterDouble("MaxMultipleExpectedPeakWidth", &m_dMaxMultipleExpectedPeakWidth);
    RegisterDouble("WidthMatchFraction", &m_dWidthMatchFraction);
    RegisterDouble("MaxSpikeWidth", &m_dMaxSpikeWidth);
    RegisterDouble("CraterSigmaMultiple", &m_dCraterSigmaMultiple);
  }
  double m_dMinSigmaForDyeBlob;
  double m_dSecondaryContentLimitFraction;
  double m_dMinFractionExpectedPeakWidth;
  double m_dMaxMultipleExpectedPeakWidth;
  double m_dWidthMatchFraction;
  double m_dMaxSpikeWidth;
  double m_dCraterSigmaMultiple;
  int m_nMsgDoubleResolution;
};

////
class CAllele : public nwxXmlPersist
{
public:
  CAllele()
  {
    RegisterAll(true);
  }
  virtual ~CAllele() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Name",&m_sName);
    RegisterInt("BioID",&m_nBioID);
  }
  wxString m_sName;
  int m_nBioID;
};


class CSSLocusType : public nwxXmlPersist
{
public:
  CSSLocusType()
  {
    RegisterAll(true);
  }
  virtual ~CSSLocusType() {;}

  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Name",&m_sName);
    Register("Allele",&m_io,&m_vpAlleles);
  }
  wxString m_sName;
  vector<CAllele *> m_vpAlleles;
  TnwxXmlIOPersistVector<CAllele> m_io;
};
class CSSLocusCollection : public TnwxXmlPersistVector<CSSLocusType>
{
public:
  CSSLocusCollection() : TnwxXmlPersistVector<CSSLocusType>("Locus") {;}
  virtual ~CSSLocusCollection() {;}
};

class CPositiveControl : public nwxXmlPersist
{
public:
  CPositiveControl()
  {
    RegisterAll(true);
  }
  virtual ~CPositiveControl() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Name",&m_sName);
    Register("Loci",&m_loci);
  }
  wxString m_sName;
  CSSLocusCollection m_loci;
};


class CStdMarkerSetCollection : public nwxXmlPersist
{
public:
  CStdMarkerSetCollection() : m_PositiveControls("PositiveControl")
  {
    RegisterAll(true);
  }
  virtual ~CStdMarkerSetCollection() {;}
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("MarkerSetName",&m_sMarkerSetName);
    Register("PositiveControls",&m_PositiveControls);
    Register("StandardOffLadderAlleles",&m_StandardOffLadderAlleles);
    Register("StandardTrialleles",&m_StandardTrialleles);
    Register("StdPosCtrlTrialleles",&m_StdPosCtrlTrialleles);
  }

  wxString m_sMarkerSetName;
  TnwxXmlPersistVector<CPositiveControl>  m_PositiveControls;
  CSSLocusCollection m_StandardOffLadderAlleles;
  CSSLocusCollection m_StandardTrialleles;
  CSSLocusCollection m_StdPosCtrlTrialleles;

};



class CStandardSettings : public nwxXmlPersist
{
public:
  CStandardSettings() : m_markerSet("StdMarkerSetCollection")
  {
    RegisterAll(true);
  }
  virtual ~CStandardSettings() {;}
  virtual void RegisterAll(bool = false)
  {
    Register("AlgorithmParameters",&m_algorithm);
    Register("StandardParameters",&m_standard);
    Register("StdMarkerSetSpecifications",&m_markerSet);
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sRootNodeStd;
  }
  CAlgorithmParameters m_algorithm;
  CStandardParameters m_standard;
  TnwxXmlPersistVector<CStdMarkerSetCollection> m_markerSet;

private:
  static const wxString g_sRootNodeStd;
};



#endif
