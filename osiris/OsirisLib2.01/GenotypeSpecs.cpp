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
*  FileName: GenotypeSpecs.cpp
*  Author:   Robert Goor
*
*/
//
// class GenotypeSpecs reads an xml file with STR Genotype data and returns one marker at a time
//

#include "GenotypeSpecs.h"
#include "rgfile.h"

int GenotypeSpecs::KitTestLength = 6;

/*
GenotypeSpecs :: GenotypeSpecs (const RGString& fileName) : NumberOfChannels (4), Person (NULL), Marker (NULL), MarkerName (NULL), 
Allele1 (NULL), Allele2 (NULL), Amplitude1 (NULL), Amplitude2 (NULL), Allele (NULL), Amplitude (NULL), AlleleName (NULL), AlleleSize (NULL), 
Artifacts (NULL), ArtifactChannel (NULL), ChannelNumber (NULL), ArtifactCurve (NULL), CurveBP (NULL), KitSearch (NULL), KitNameSearch (NULL), Control (NULL), 
ControlType (NULL), ID (NULL), CurveHeight (NULL), CurveType (NULL), CurveWidth (NULL), Valid (TRUE), startPerson (0), startControl (0), startMarker (0), startArtifact (0),
startChannel (0), CurrentChannel (0), startArtifactCurve (0), PositiveCtlNumber (1), NegativeCtlNumber (1), FoundAllCtls (FALSE) {

	size_t endPos;
	RGString runTimeTag = "RunTime";
	RGString runDateTag = "RunDate";
	RGString timeStartTag;
	RGString timeEndTag;
	RGString dateStartTag;
	RGString dateEndTag;
	RGString analysisTimeTag = "AnalysisTime";
	RGString analysisDateTag = "AnalysisDate";
	ChannelPct = new RGBracketStringSearch* [NumberOfChannels + 1];

	for (int i=0; i<=NumberOfChannels; i++)
		ChannelPct [i] = NULL;
	
	RGFile inputFile (fileName, "rt");

	if (!inputFile.isValid ())
		Valid = FALSE;

	else {

		Specifications.ReadTextFile (inputFile);
		Person = new RGBracketStringSearch ("<person>", "</person>", Specifications);
		Control = new RGBracketStringSearch ("<control>", "</control>", Specifications);
		ControlType = new RGBracketStringSearch ("<type>", "</type>", PersonString);
		ID = new RGBracketStringSearch ("<Id>", "</Id>", PersonString);
		Marker = new RGBracketStringSearch ("<marker>", "</marker>", PersonString);
		MarkerName = new RGBracketStringSearch ("<name>", "</name>", MarkerString);
		Allele1 = new RGBracketStringSearch ("<allele1>", "</allele1>", MarkerString);
		Allele2 = new RGBracketStringSearch ("<allele2>", "</allele2>", MarkerString);
		Amplitude1 = new RGBracketStringSearch ("<amplitude1>", "</amplitude1>", MarkerString);
		Amplitude2 = new RGBracketStringSearch ("<amplitude2>", "</amplitude2>", MarkerString);

		Allele = new RGBracketStringSearch ("<allele>", "</allele>", MarkerString);
		Amplitude = new RGBracketStringSearch ("<amp>", "</amp>", AlleleString);
		AlleleName = new RGBracketStringSearch ("<name>", "</name>", AlleleString);
		AlleleSize = new RGBracketStringSearch ("<bps>", "</bps>", AlleleString);

		Artifacts = new RGBracketStringSearch ("<artifacts>", "</artifacts>", PersonString);
		KitSearch = new RGBracketStringSearch ("<kit>", "</kit>", ArtifactString);
		KitNameSearch = new RGBracketStringSearch ("<name>", "</name>", KitString);
		ArtifactChannel = new RGBracketStringSearch ("<channel>", "</channel>", KitString);
		ChannelNumber = new RGBracketStringSearch ("<channelNumber>", "</channelNumber>", ChannelString);
		ArtifactCurve = new RGBracketStringSearch ("<curve>", "</curve>", ChannelString);
		CurveBP = new RGBracketStringSearch ("<bp>", "</bp>", CurveString);
		CurveHeight = new RGBracketStringSearch ("<height>", "</height>", CurveString);
		CurveType = new RGBracketStringSearch ("<type>", "</type>", CurveString);
		CurveWidth = new RGBracketStringSearch ("<width>", "</width>", CurveString);

		timeStartTag = "<" + runTimeTag + ">";
		timeEndTag = "</" + runTimeTag + ">";
		dateStartTag = "<" + runDateTag + ">";
		dateEndTag = "</" + runDateTag +">";
		RGBracketStringSearch CurveTime (timeStartTag, timeEndTag, Specifications);
		RGBracketStringSearch CurveDate (dateStartTag, dateEndTag, Specifications);
		CurveTime.FindNextBracketedString (0, endPos, Time);
		CurveDate.FindNextBracketedString (0, endPos, Date);
		Date.FindAndReplaceAllSubstrings ("-", ":");

		ChannelPct [1] = new RGBracketStringSearch ("<ch1pct>", "</ch1pct>", CurveString);
		ChannelPct [2] = new RGBracketStringSearch ("<ch2pct>", "</ch2pct>", CurveString);
		ChannelPct [3] = new RGBracketStringSearch ("<ch3pct>", "</ch3pct>", CurveString);
		ChannelPct [4] = new RGBracketStringSearch ("<ch4pct>", "</ch4pct>", CurveString);

		RGString analysisTimeStartTag = "<" + analysisTimeTag + ">";
		RGString analysisTimeEndTag = "</" + analysisTimeTag + ">";
		RGString analysisDateStartTag = "<" + analysisDateTag + ">";
		RGString analysisDateEndTag = "</" + analysisDateTag + ">";
		RGBracketStringSearch analysisTimeSearch (analysisTimeStartTag, analysisTimeEndTag, Specifications);
		RGBracketStringSearch analysisDateSearch (analysisDateStartTag, analysisDateEndTag, Specifications);
		RGBracketStringSearch gelExpirationDateSearch ("<GelExpDate>", "</GelExpDate>", Specifications);
		analysisTimeSearch.FindNextBracketedString (0, endPos, AnalysisTime);
		analysisDateSearch.FindNextBracketedString (0, endPos, AnalysisDate);
		gelExpirationDateSearch.FindNextBracketedString (0, endPos, GelExpirationDate);
	}
}*/



GenotypeSpecs :: GenotypeSpecs (const RGString& fileName, int nChannels) : 
NumberOfChannels (nChannels), TotalCount (0), Person (NULL), Marker (NULL), MarkerName (NULL), 
Allele1 (NULL), Allele2 (NULL), Amplitude1 (NULL), Amplitude2 (NULL), Allele (NULL), Amplitude (NULL), AlleleName (NULL), AlleleSize (NULL), 
Artifacts (NULL), ArtifactChannel (NULL), ChannelNumber (NULL), ArtifactCurve (NULL), CurveBP (NULL), KitSearch (NULL), Control (NULL), 
ControlType (NULL), ID (NULL), CurveHeight (NULL), CurveType (NULL), CurveWidth (NULL), Valid (TRUE), startPerson (0), startControl (0), startMarker (0), startArtifact (0),
startChannel (0), CurrentChannel (0), startArtifactCurve (0), PositiveCtlNumber (1), NegativeCtlNumber (1), FoundAllCtls (FALSE) {

	RGString lPct;
	RGString rPct;
	size_t endPos;
	RGString runTimeTag = "RunTime";
	RGString runDateTag = "RunDate";
	RGString timeStartTag;
	RGString timeEndTag;
	RGString dateStartTag;
	RGString dateEndTag;
	RGString analysisTimeTag = "AnalysisTime";
	RGString analysisDateTag = "AnalysisDate";
	ChannelPct = new RGBracketStringSearch* [NumberOfChannels + 1];
  int i;
	for (i=0; i<=NumberOfChannels; i++)
		ChannelPct [i] = NULL;
	
	RGFile inputFile (fileName, "rt");

	if (!inputFile.isValid ())
		Valid = FALSE;

	else {

		Specifications.ReadTextFile (inputFile);
		Person = new RGBracketStringSearch ("<person>", "</person>", Specifications);
		Control = new RGBracketStringSearch ("<control>", "</control>", Specifications);
		ControlType = new RGBracketStringSearch ("<type>", "</type>", PersonString);
		ID = new RGBracketStringSearch ("<Id>", "</Id>", PersonString);
		Marker = new RGBracketStringSearch ("<marker>", "</marker>", PersonString);
		MarkerName = new RGBracketStringSearch ("<name>", "</name>", MarkerString);
		Allele1 = new RGBracketStringSearch ("<allele1>", "</allele1>", MarkerString);
		Allele2 = new RGBracketStringSearch ("<allele2>", "</allele2>", MarkerString);
		Amplitude1 = new RGBracketStringSearch ("<amplitude1>", "</amplitude1>", MarkerString);
		Amplitude2 = new RGBracketStringSearch ("<amplitude2>", "</amplitude2>", MarkerString);

		Allele = new RGBracketStringSearch ("<allele>", "</allele>", MarkerString);
		Amplitude = new RGBracketStringSearch ("<amp>", "</amp>", AlleleString);
		AlleleName = new RGBracketStringSearch ("<name>", "</name>", AlleleString);
		AlleleSize = new RGBracketStringSearch ("<bps>", "</bps>", AlleleString);

		Artifacts = new RGBracketStringSearch ("<artifacts>", "</artifacts>", PersonString);
		KitSearch = new RGBracketStringSearch ("<kit>", "</kit>", Specifications);
//		KitNameSearch = new RGBracketStringSearch ("<name>", "</name>", KitString);
		ArtifactChannel = new RGBracketStringSearch ("<channel>", "</channel>", ArtifactString);
		ChannelNumber = new RGBracketStringSearch ("<channelNumber>", "</channelNumber>", ChannelString);
		ArtifactCurve = new RGBracketStringSearch ("<curve>", "</curve>", ChannelString);
		CurveBP = new RGBracketStringSearch ("<bp>", "</bp>", CurveString);
		CurveHeight = new RGBracketStringSearch ("<height>", "</height>", CurveString);
		CurveType = new RGBracketStringSearch ("<type>", "</type>", CurveString);
		CurveWidth = new RGBracketStringSearch ("<width>", "</width>", CurveString);

		timeStartTag = "<" + runTimeTag + ">";
		timeEndTag = "</" + runTimeTag + ">";
		dateStartTag = "<" + runDateTag + ">";
		dateEndTag = "</" + runDateTag +">";
		RGBracketStringSearch CurveTime (timeStartTag, timeEndTag, Specifications);
		RGBracketStringSearch CurveDate (dateStartTag, dateEndTag, Specifications);
		CurveTime.FindNextBracketedString (0, endPos, Time);
		CurveDate.FindNextBracketedString (0, endPos, Date);
		Date.FindAndReplaceAllSubstrings ("-", ":");

		for (i=1; i<=NumberOfChannels; i++) {

			BuildChannelPct (i, lPct, rPct);
			ChannelPct [i] = new RGBracketStringSearch (lPct, rPct, CurveString);
		}

		RGString analysisTimeStartTag = "<" + analysisTimeTag + ">";
		RGString analysisTimeEndTag = "</" + analysisTimeTag + ">";
		RGString analysisDateStartTag = "<" + analysisDateTag + ">";
		RGString analysisDateEndTag = "</" + analysisDateTag + ">";
		RGBracketStringSearch analysisTimeSearch (analysisTimeStartTag, analysisTimeEndTag, Specifications);
		RGBracketStringSearch analysisDateSearch (analysisDateStartTag, analysisDateEndTag, Specifications);
		RGBracketStringSearch gelExpirationDateSearch ("<GelExpDate>", "</GelExpDate>", Specifications);
		analysisTimeSearch.FindNextBracketedString (0, endPos, AnalysisTime);
		analysisDateSearch.FindNextBracketedString (0, endPos, AnalysisDate);
		gelExpirationDateSearch.FindNextBracketedString (0, endPos, GelExpirationDate);
		KitSearch->FindNextBracketedString (0, endPos, KitName);
		KitName.ToLower ();
		RGBracketStringSearch TCount ("<count>", "</count>", Specifications);
		RGString CountString;
		TCount.FindNextBracketedString (0, endPos, CountString);
		TotalCount = CountString.ConvertToInteger ();
	}
}


GenotypeSpecs :: GenotypeSpecs (const RGString& fileName, PopulationCollection* collection) : 
TotalCount (0), Person (NULL), Marker (NULL), MarkerName (NULL), 
Allele1 (NULL), Allele2 (NULL), Amplitude1 (NULL), Amplitude2 (NULL), Allele (NULL), Amplitude (NULL), AlleleName (NULL), AlleleSize (NULL), 
Artifacts (NULL), ArtifactChannel (NULL), ChannelNumber (NULL), ArtifactCurve (NULL), CurveBP (NULL), KitSearch (NULL), Control (NULL), 
ControlType (NULL), ID (NULL), CurveHeight (NULL), CurveType (NULL), CurveWidth (NULL), Valid (TRUE), startPerson (0), startControl (0), startMarker (0), startArtifact (0),
startChannel (0), CurrentChannel (0), startArtifactCurve (0), PositiveCtlNumber (1), NegativeCtlNumber (1), FoundAllCtls (FALSE) {

	RGString lPct;
	RGString rPct;
	size_t endPos;
	RGString runTimeTag = "RunTime";
	RGString runDateTag = "RunDate";
	RGString timeStartTag;
	RGString timeEndTag;
	RGString dateStartTag;
	RGString dateEndTag;
	RGString analysisTimeTag = "AnalysisTime";
	RGString analysisDateTag = "AnalysisDate";
	int i;
	
	RGFile inputFile (fileName, "rt");

	if (!inputFile.isValid ()) {

		Valid = FALSE;
		ChannelPct = new RGBracketStringSearch* [2];
		ChannelPct [0] = ChannelPct [1] = NULL;
	}

	else {

		Specifications.ReadTextFile (inputFile);
		KitSearch = new RGBracketStringSearch ("<kit>", "</kit>", Specifications);
		KitSearch->FindNextBracketedString (0, endPos, KitName);
		PopulationMarkerSet* set = collection->GetNamedPopulationMarkerSet (KitName);

		if (set == NULL) {

			Valid = FALSE;
			ChannelPct = new RGBracketStringSearch* [2];
			ChannelPct [0] = ChannelPct [1] = NULL;
		}

		else {

			NumberOfChannels = set->GetNumberOfChannels ();
			delete set;
			ChannelPct = new RGBracketStringSearch* [NumberOfChannels + 1];

			for (i=0; i<=NumberOfChannels; i++)
				ChannelPct [i] = NULL;

			Person = new RGBracketStringSearch ("<person>", "</person>", Specifications);
			Control = new RGBracketStringSearch ("<control>", "</control>", Specifications);
			ControlType = new RGBracketStringSearch ("<type>", "</type>", PersonString);
			ID = new RGBracketStringSearch ("<Id>", "</Id>", PersonString);
			Marker = new RGBracketStringSearch ("<marker>", "</marker>", PersonString);
			MarkerName = new RGBracketStringSearch ("<name>", "</name>", MarkerString);
			Allele1 = new RGBracketStringSearch ("<allele1>", "</allele1>", MarkerString);
			Allele2 = new RGBracketStringSearch ("<allele2>", "</allele2>", MarkerString);
			Amplitude1 = new RGBracketStringSearch ("<amplitude1>", "</amplitude1>", MarkerString);
			Amplitude2 = new RGBracketStringSearch ("<amplitude2>", "</amplitude2>", MarkerString);

			Allele = new RGBracketStringSearch ("<allele>", "</allele>", MarkerString);
			Amplitude = new RGBracketStringSearch ("<amp>", "</amp>", AlleleString);
			AlleleName = new RGBracketStringSearch ("<name>", "</name>", AlleleString);
			AlleleSize = new RGBracketStringSearch ("<bps>", "</bps>", AlleleString);

			Artifacts = new RGBracketStringSearch ("<artifacts>", "</artifacts>", PersonString);
			
			ArtifactChannel = new RGBracketStringSearch ("<channel>", "</channel>", ArtifactString);
			ChannelNumber = new RGBracketStringSearch ("<channelNumber>", "</channelNumber>", ChannelString);
			ArtifactCurve = new RGBracketStringSearch ("<curve>", "</curve>", ChannelString);
			CurveBP = new RGBracketStringSearch ("<bp>", "</bp>", CurveString);
			CurveHeight = new RGBracketStringSearch ("<height>", "</height>", CurveString);
			CurveType = new RGBracketStringSearch ("<type>", "</type>", CurveString);
			CurveWidth = new RGBracketStringSearch ("<width>", "</width>", CurveString);

			timeStartTag = "<" + runTimeTag + ">";
			timeEndTag = "</" + runTimeTag + ">";
			dateStartTag = "<" + runDateTag + ">";
			dateEndTag = "</" + runDateTag +">";
			RGBracketStringSearch CurveTime (timeStartTag, timeEndTag, Specifications);
			RGBracketStringSearch CurveDate (dateStartTag, dateEndTag, Specifications);
			CurveTime.FindNextBracketedString (0, endPos, Time);
			CurveDate.FindNextBracketedString (0, endPos, Date);
			Date.FindAndReplaceAllSubstrings ("-", ":");

			for (i=1; i<=NumberOfChannels; i++) {

				BuildChannelPct (i, lPct, rPct);
				ChannelPct [i] = new RGBracketStringSearch (lPct, rPct, CurveString);
			}

			RGString analysisTimeStartTag = "<" + analysisTimeTag + ">";
			RGString analysisTimeEndTag = "</" + analysisTimeTag + ">";
			RGString analysisDateStartTag = "<" + analysisDateTag + ">";
			RGString analysisDateEndTag = "</" + analysisDateTag + ">";
			RGBracketStringSearch analysisTimeSearch (analysisTimeStartTag, analysisTimeEndTag, Specifications);
			RGBracketStringSearch analysisDateSearch (analysisDateStartTag, analysisDateEndTag, Specifications);
			RGBracketStringSearch gelExpirationDateSearch ("<GelExpDate>", "</GelExpDate>", Specifications);
			analysisTimeSearch.FindNextBracketedString (0, endPos, AnalysisTime);
			analysisDateSearch.FindNextBracketedString (0, endPos, AnalysisDate);
			gelExpirationDateSearch.FindNextBracketedString (0, endPos, GelExpirationDate);
			KitName.ToLower ();
			RGBracketStringSearch TCount ("<count>", "</count>", Specifications);
			RGString CountString;
			TCount.FindNextBracketedString (0, endPos, CountString);
			TotalCount = CountString.ConvertToInteger ();
		}
	}
}



GenotypeSpecs :: ~GenotypeSpecs () {

	delete Person;
	delete Marker;
	delete MarkerName;
	delete Allele1;
	delete Allele2;
	delete Amplitude1;
	delete Amplitude2;

	delete Allele;
	delete Amplitude;
	delete AlleleName;
	delete AlleleSize;

	delete Artifacts;
	delete ArtifactChannel;
	delete ChannelNumber;
	delete ArtifactCurve;
	delete CurveBP;
	delete KitSearch;
	delete Control;
	delete ControlType;
	delete ID;
	delete CurveHeight;
	delete CurveType;
	delete CurveWidth;
//	delete CurveTime;

/*	delete Blob;
	delete BlobHeight;
	delete BlobWidth;
	delete BlobChannel;
	delete BlobTime;*/

//	delete Spike;
//	delete SpikeTime;
//	delete SpikeHeight;
//	delete SpikeWidth;

//	delete Pullup;
//	delete PullupTime;
//	delete PullupHeight;

	for (int i=1; i<=NumberOfChannels; i++)
		delete ChannelPct [i];

	delete[] ChannelPct;
}


Boolean GenotypeSpecs :: GetNextPerson (RGString& name) {

	size_t endPosition;
	Boolean returnValue;
	Boolean status;
	RGString type;
	RGString id;
	RGString ctlNum;

	if (!FoundAllCtls) {

		returnValue = Control->FindNextBracketedString (startControl, endPosition, PersonString);
		startControl = endPosition;

		if (!returnValue)
			FoundAllCtls = TRUE;

		else {

			ControlType->ResetSearch ();
			Artifacts->ResetSearch ();
			status = ControlType->FindNextBracketedString (0, endPosition, type);

			if (!status)
				return FALSE;

			if (type == "-") {

				ctlNum.Convert (NegativeCtlNumber, 10);
				NegativeCtlNumber++;
			}

			else {

				ctlNum.Convert (PositiveCtlNumber, 10);
				PositiveCtlNumber++;
			}

			name = "CTL" + type + ctlNum;
		}
	}

	if (FoundAllCtls) {

		returnValue = Person->FindNextBracketedString (startPerson, endPosition, PersonString);
		startPerson = endPosition;

		if (!returnValue)
			return returnValue;

		ID->ResetSearch ();
		status = ID->FindNextBracketedString (0, endPosition, id);

		if (!status)
			return FALSE;

		name = "Person" + id;
	}
	
	Marker->ResetSearch ();
	Artifacts->ResetSearch ();
	startArtifact = 0;
	RGString ChannelNo;
	RGString kit;
	RGString subKit;
	status = Artifacts->FindNextBracketedString (startArtifact, endPosition, ArtifactString);

	if (!status)
		ArtifactString = "";

//	KitSearch->ResetSearch ();
//	size_t startKit = 0;

/*	while (TRUE) {

		status = KitSearch->FindNextBracketedString (startKit, endPosition, KitString);
		startKit = endPosition;

		if (!status) {

			KitString = "";
			break;
		}

		KitNameSearch->ResetSearch ();
		status = KitNameSearch->FindNextBracketedString (0, endPosition, kit);

		if (!status) {

			KitString = "";
			break;
		}

		subKit = kit.ExtractSubstring (0, GenotypeSpecs::KitTestLength - 1);
		subKit.ToLower ();

		if (subKit == KitName)
			break;
	}*/

	ArtifactChannel->ResetSearch ();
//	Spike->ResetSearch ();
//	Pullup->ResetSearch ();
//	Blob->ResetSearch ();

	startMarker = 0;
	startAllele = 0;
//	startBlob = 0;
//	startSpike = 0;
//	startPullup = 0;
	startArtifactCurve = 0;
	startChannel = 0;

	status = ArtifactChannel->FindNextBracketedString (startChannel, endPosition, ChannelString);
	startChannel = endPosition;

	if (!status)
		ChannelString = "";

	ChannelNumber->ResetSearch ();
	ArtifactCurve->ResetSearch ();
	status = ChannelNumber->FindNextBracketedString (0, endPosition, ChannelNo);

	if (!status)
		CurrentChannel = 0;

	else
		CurrentChannel = ChannelNo.ConvertToInteger ();

	return returnValue;
}



Boolean GenotypeSpecs :: GetNextMarker (XMLMarkerData& mData) {

	size_t endPosition;
	RGString Amp;
	
	if (!Marker->FindNextBracketedString (startMarker, endPosition, MarkerString))
		return FALSE;

	startMarker = endPosition;
	MarkerName->ResetSearch ();
	Allele1->ResetSearch ();
	Allele2->ResetSearch ();
	Amplitude1->ResetSearch ();
	Amplitude2->ResetSearch ();

	if (!MarkerName->FindNextBracketedString (0, endPosition, mData.Name))
		return FALSE;

	if (!Allele1->FindNextBracketedString (endPosition, endPosition, mData.Allele1))
		return FALSE;

	if (!Amplitude1->FindNextBracketedString (endPosition, endPosition, Amp))
		return FALSE;

	mData.Amplitude1 = Amp.ConvertToInteger ();

	if (!Allele2->FindNextBracketedString (endPosition, endPosition, mData.Allele2))
		return FALSE;

	if (!Amplitude2->FindNextBracketedString (endPosition, endPosition, Amp))
		return FALSE;

	mData.Amplitude2 = Amp.ConvertToInteger ();
	return TRUE;
}


Boolean GenotypeSpecs :: GetNextArtifactCurve (XMLArtifactData& mData) {

	size_t endPosition;
	RGString Amp;
	RGString BP;
	RGString ChannelNo;
	RGString Type;
	RGString Width;
	RGString Pct;
	
	if (!ArtifactCurve->FindNextBracketedString (startArtifactCurve, endPosition, CurveString)) {

		if (!ArtifactChannel->FindNextBracketedString (startChannel, endPosition, ChannelString))
			return FALSE;

		startChannel = endPosition;
		ChannelNumber->ResetSearch ();
		ArtifactCurve->ResetSearch ();
		startArtifactCurve = 0;
		
		if (!ChannelNumber->FindNextBracketedString (0, endPosition, ChannelNo)) {

			CurrentChannel = 0;
			return FALSE;
		}

		else
			CurrentChannel = ChannelNo.ConvertToInteger ();

		if (!ArtifactCurve->FindNextBracketedString (startArtifactCurve, endPosition, CurveString))
			return FALSE;
	}

	startArtifactCurve = endPosition;
	CurveBP->ResetSearch ();
	CurveHeight->ResetSearch ();
	CurveType->ResetSearch ();
	CurveWidth->ResetSearch ();
//	CurveTime->ResetSearch ();
  int i;
	for (i=1; i<=NumberOfChannels; i++)
		ChannelPct [i]->ResetSearch ();

	if (!CurveType->FindNextBracketedString (0, endPosition, Type))
		return FALSE;

	Type.ToLower ();

	if (Type.GetCharacter (0) == 'b') {

		// this is a blob
		mData.type = "blob";

		if (!CurveWidth->FindNextBracketedString (0, endPosition, Width))
			return FALSE;

		mData.Width = Width.ConvertToDouble ();
	}

	else if (Type.GetCharacter (0) == 'c') {

		// this is a straight artifact
		mData.type = "curve";
	}

	else {

		for (i=1; i<=NumberOfChannels; i++) {

			mData.ChPct [i] = 0.0;

			if (ChannelPct[i]->FindNextBracketedString (0, endPosition, Pct))
				mData.ChPct [i] = Pct.ConvertToDouble ();
		}
		
		if (Type.GetCharacter (0) == 's') {

			// this is a spike
			mData.type = "spike";

			if (!CurveWidth->FindNextBracketedString (0, endPosition, Width))
				return FALSE;

			mData.Width = Width.ConvertToDouble ();
		}

		else if (Type.GetCharacter (0) == 'p') {

			//this is a pullup
			mData.type = "pullup";
		}

		else
			mData.type = "?";
	}

	if (!CurveHeight->FindNextBracketedString (0, endPosition, Amp))
		return FALSE;

	mData.Height = Amp.ConvertToDouble ();

	if (!CurveBP->FindNextBracketedString (0, endPosition, BP))
		return FALSE;

	mData.BP = BP.ConvertToDouble ();

	mData.channel = CurrentChannel;
	return TRUE;
}


Boolean GenotypeSpecs :: GetNextMarker (RGString& markerName) {

	size_t endPosition;
	RGString Amp;
	
	if (!Marker->FindNextBracketedString (startMarker, endPosition, MarkerString))
		return FALSE;

	startMarker = endPosition;
	MarkerName->ResetSearch ();
	Allele->ResetSearch ();
	Amplitude->ResetSearch ();
	startAllele = 0;

	if (!MarkerName->FindNextBracketedString (0, endPosition, markerName))
		return FALSE;

/*	if (!Allele1->FindNextBracketedString (endPosition, endPosition, mData.Allele1))
		return FALSE;

	if (!Amplitude1->FindNextBracketedString (endPosition, endPosition, Amp))
		return FALSE;

	mData.Amplitude1 = Amp.ConvertToInteger ();

	if (!Allele2->FindNextBracketedString (endPosition, endPosition, mData.Allele2))
		return FALSE;

	if (!Amplitude2->FindNextBracketedString (endPosition, endPosition, Amp))
		return FALSE;

	mData.Amplitude2 = Amp.ConvertToInteger ();*/
	return TRUE;
}


Boolean GenotypeSpecs :: GetNextAllele (RGString& alleleName, int& size, int& amplitude) {

	RGString Amp;
	size_t endPosition;

	if (!Allele->FindNextBracketedString (startAllele, endPosition, AlleleString))
		return FALSE;

	startAllele = endPosition;

	AlleleName->ResetSearch ();
	AlleleSize->ResetSearch ();
	Amplitude->ResetSearch ();

	if (!AlleleName->FindNextBracketedString (0, endPosition, alleleName))
		return FALSE;

	if (!AlleleSize->FindNextBracketedString (0, endPosition, Amp))
		return FALSE;

	size = Amp.ConvertToInteger ();

	if (!Amplitude->FindNextBracketedString (0, endPosition, Amp))
		return FALSE;

	amplitude = Amp.ConvertToInteger ();
	return TRUE;
}


void GenotypeSpecs :: BuildChannelPct (int i, RGString& left, RGString& right) {

	RGString channelNumber;
	channelNumber.Convert (i, 10);
	RGString Result = "ch" + channelNumber + "pct";
	left = "<" + Result + ">";
	right = "</" + Result + ">";
}

