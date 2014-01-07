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
*  FileName: GenotypeSpecs.h
*  Author:   Robert Goor
*
*/
//
// class GenotypeSpecs reads an xml file with STR Genotype data and returns one marker at a time
//

#ifndef _GENOTYPESPECS_H_
#define _GENOTYPESPECS_H_

#include "rgstring.h"
#include "rgtokenizer.h"
#include "rgdefs.h"
#include "Genetics.h"

struct XMLMarkerData {

	RGString Name;
	RGString Allele1;
	RGString Allele2;
	int Amplitude1;
	int Amplitude2;
};


struct XMLArtifactData {

	RGString type;
	double BP;
	double Height;
	double Width;  // This is a width multiple!
	double ChPct [5];
	int channel;
};



class GenotypeSpecs {

public:
//	GenotypeSpecs (const RGString& fileName);
	GenotypeSpecs (const RGString& fileName, int nChannels);
	GenotypeSpecs (const RGString& fileName, PopulationCollection* collection);
	~GenotypeSpecs ();

	static void SetKitTestLength (int length) { KitTestLength = length; }
	static int GetKitTestLength () { return KitTestLength; }

	RGString GetKitName () const { return KitName; }

	RGString GetTime () const { return Time; }
	RGString GetDate () const { return Date; }
	RGString GetAnalysisDate () const { return AnalysisDate; }
	RGString GetAnalysisTime () const { return AnalysisTime; }
	RGString GetGelExpirationDate () const { return GelExpirationDate; }
	int GetTotalCount () const { return TotalCount; }

	Boolean IsValid () const { return Valid; }
	Boolean GetNextPerson (RGString& name);
	Boolean GetNextMarker (XMLMarkerData& mData);
	Boolean GetNextArtifactCurve (XMLArtifactData& mData);

	Boolean GetNextMarker (RGString& markerName);
	Boolean GetNextAllele (RGString& alleleName, int& size, int& amplitude);

protected:
	RGString Specifications;
	RGString PersonString;
	RGString MarkerString;
	RGString AlleleString;

	RGString KitName;
	int NumberOfChannels;
	int TotalCount;

	RGString ArtifactString;
	RGString ChannelString;
	RGString CurveString;
//	RGString BlobString;
	RGString SpikeString;
	RGString PullupString;
//	RGString KitString;
	RGString Time;
	RGString Date;
	RGString AnalysisTime;
	RGString AnalysisDate;
	RGString GelExpirationDate;

	RGBracketStringSearch* Person;
	RGBracketStringSearch* Marker;
	RGBracketStringSearch* MarkerName;
	RGBracketStringSearch* Allele1;
	RGBracketStringSearch* Allele2;
	RGBracketStringSearch* Amplitude1;
	RGBracketStringSearch* Amplitude2;

	RGBracketStringSearch* Allele;
	RGBracketStringSearch* Amplitude;
	RGBracketStringSearch* AlleleName;
	RGBracketStringSearch* AlleleSize;

	RGBracketStringSearch* Artifacts;
	RGBracketStringSearch* ArtifactChannel;
	RGBracketStringSearch* ChannelNumber;
	RGBracketStringSearch* ArtifactCurve;
	RGBracketStringSearch* CurveBP;
	RGBracketStringSearch* KitSearch;
//	RGBracketStringSearch* KitNameSearch;
	RGBracketStringSearch* Control;
	RGBracketStringSearch* ControlType;
	RGBracketStringSearch* ID;
	RGBracketStringSearch* CurveHeight;
	RGBracketStringSearch* CurveType;
	RGBracketStringSearch* CurveWidth;
//	RGBracketStringSearch* CurveTime;

/*	RGBracketStringSearch* Blob;
	RGBracketStringSearch* BlobHeight;
	RGBracketStringSearch* BlobWidth;
	RGBracketStringSearch* BlobChannel;
	RGBracketStringSearch* BlobTime;*/

//	RGBracketStringSearch* Spike;
//	RGBracketStringSearch* SpikeTime;
//	RGBracketStringSearch* SpikeHeight;
//	RGBracketStringSearch* SpikeWidth;

//	RGBracketStringSearch* Pullup;
//	RGBracketStringSearch* PullupTime;
//	RGBracketStringSearch* PullupHeight;
	RGBracketStringSearch** ChannelPct;

	Boolean Valid;

	size_t startPerson;
	size_t startControl;
	size_t startMarker;
	size_t startArtifact;
	size_t startAllele;
//	size_t startBlob;
//	size_t startSpike;
//	size_t startPullup;
	size_t startChannel;
	
	int CurrentChannel;
	size_t startArtifactCurve;
	int PositiveCtlNumber;
	int NegativeCtlNumber;
	Boolean FoundAllCtls;

	static int KitTestLength;

	void BuildChannelPct (int i, RGString& left, RGString& right);
};


#endif  /*  _GENOTYPESPECS_H_  */


