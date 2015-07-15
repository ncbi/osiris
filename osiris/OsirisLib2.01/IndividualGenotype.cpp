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
*  FileName: IndividualGenotype.cpp
*  Author:   Robert Goor
*
*/
//
//  classes IndividualAllele, IndividualLocus and IndividualGenotype, designed to retain an individual genotype and to be compared to a CoreBioComponent.  These will be used to store positive controls and, later, sample data.
//


#include "IndividualGenotype.h"
#include "DataSignal.h"
#include "rgtokenizer.h"

RGString GenotypesForAMarkerSet::PathToStandardControlFile;


PERSISTENT_DEFINITION (IndividualAllele, _INDIVIDUALALLELE_, "IndividualAllele")
PERSISTENT_DEFINITION (IndividualLocus, _INDIVIDUALLOCUS_, "IndividualLocus")
PERSISTENT_DEFINITION (LocusCollection, _LOCUSCOLLECTION_, "LocusCollection")
PERSISTENT_DEFINITION (IndividualGenotype, _INDIVIDUALGENOTYPE_, "IndividualGenotype")
PERSISTENT_DEFINITION (GenotypesForAMarkerSet, _GENOTYPESFORAMARKERSET_, "GenotypesForAMarkerSet")


IndividualAllele :: IndividualAllele () : RGPersistent (), mValid (false), mAssociatedPeak (NULL) {

}



IndividualAllele :: IndividualAllele (const RGString& xmlString) : RGPersistent (), mValid (true), mAssociatedPeak (NULL) {

	RGString XMLString (xmlString);
	RGBracketStringSearch nameSearch ("<Name>", "</Name>", XMLString);
//	RGBracketStringSearch bioIDSearch ("<BioID>", "</BioID>", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;
//	RGString numberString;

	if (!nameSearch.FindNextBracketedString (startOffset, endOffset, mName))
		mName = xmlString;

/*	if (!bioIDSearch.FindNextBracketedString (startOffset, endOffset, numberString))
		mValid = false;

	mBioID = numberString.ConvertToInteger ();*/
}



IndividualAllele :: IndividualAllele (const IndividualAllele& allele) : RGPersistent (), mName (allele.mName), 
mValid (allele.mValid), mAssociatedPeak (allele.mAssociatedPeak) {

}



IndividualAllele :: ~IndividualAllele () {

}


void IndividualAllele :: SetAssociatedPeak (DataSignal* ds) {

	mAssociatedPeak = ds;
}


DataSignal* IndividualAllele :: GetAssociatedPeak () {

	return mAssociatedPeak;
}


void IndividualAllele :: WriteXML (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	RGString indent2 = indent + "\t";
	text << indent << "<Allele>" << endLine;
	text << indent2 << "<Name>" << mName << "</Name>" << endLine;
//	text << indent2 << "<BioID>" << mBioID << "</BioID>" << endLine;
	text << indent << "</Allele>" << endLine;

	// for the future...
	/*
	text << indent << "<Allele>" << mName << "</Allele>" << endLine;
	*/
}

int IndividualAllele :: CompareTo (const RGPersistent* p) const {

	IndividualAllele* q = (IndividualAllele*) p;
	return mName.CompareTo (&(q->mName));
}


unsigned IndividualAllele :: HashNumber (unsigned long Base) const {

	return mName.HashNumber (Base);
}


Boolean IndividualAllele :: IsEqualTo (const RGPersistent* p) const {

	IndividualAllele* q = (IndividualAllele*) p;
	return mName.IsEqualTo (&(q->mName));
}



IndividualLocus :: IndividualLocus () : RGPersistent (), mAlleles (5), mValid (false) {

}


IndividualLocus :: IndividualLocus (const RGString& xmlString) : RGPersistent (), mAlleles (5), mValid (true) {

	RGString XMLString (xmlString);
	RGBracketStringSearch nameSearch ("<Name>", "</Name>", XMLString);
	RGBracketStringSearch alleleSearch ("<Allele>", "</Allele>", XMLString);
	size_t startOffset = 0;
	size_t endOffset;
	IndividualAllele* newAllele;
	RGString alleleString;

	if (!nameSearch.FindNextBracketedString (startOffset, endOffset, mName))
		mValid = false;

	startOffset = endOffset;

	while (alleleSearch.FindNextBracketedString (startOffset, endOffset, alleleString)) {

		startOffset = endOffset;
		newAllele = new IndividualAllele (alleleString);

		if (!newAllele->isValid ())
			mValid = false;

		else {

			mAlleles.Insert (newAllele);
			mAllelesByName.Append (newAllele);
		}
	}

	if (mAlleles.Entries () == 0)
		mValid = false;
}


IndividualLocus :: IndividualLocus (const IndividualLocus& locus) : RGPersistent (), mAlleles (5),
mAllelesByName (locus.mAllelesByName), mValid (locus.mValid) {

	IndividualAllele* nextAllele;
	RGDListIterator it (mAllelesByName);

	while (nextAllele = (IndividualAllele*) it ())
		mAlleles.Insert (nextAllele);
}


IndividualLocus :: ~IndividualLocus () {

	mAllelesByName.Clear ();
	mAlleles.ClearAndDelete ();
}


bool IndividualLocus :: Matches (const IndividualLocus& locus) const {

	if (!locus.IsContainedIn (*this))
		return false;

	if (!IsContainedIn (locus))
		return false;

	return true;
}


Boolean IndividualLocus :: ContainsAllele (const IndividualAllele& allele) const {

	return mAlleles.Contains (&allele);
}


bool IndividualLocus :: IsContainedIn (const IndividualLocus& locus) const {

	RGDList temp (mAllelesByName);
	IndividualAllele* nextAllele;

	while (nextAllele = (IndividualAllele*) temp.GetFirst ()) {

		if (!locus.ContainsAllele (*nextAllele)) {

			temp.ClearAndDelete ();
			delete nextAllele;
			return false;
		}

		delete nextAllele;
	}

	return true;
}


bool IndividualLocus :: IsContainedIn (const RGDList& alleleList) const {

	RGDList temp (mAllelesByName);
	IndividualAllele* nextAllele;

	while (nextAllele = (IndividualAllele*) temp.GetFirst ()) {

		if (!alleleList.Contains (nextAllele)) {

			temp.ClearAndDelete ();
			delete nextAllele;
			return false;
		}

		delete nextAllele;
	}

	return true;
}


void IndividualLocus :: AppendAllelesToList (RGDList& alleleList) {

	RGDListIterator it (mAllelesByName);
	IndividualAllele* nextAllele;

	while (nextAllele = (IndividualAllele*) it ())
		alleleList.Append (nextAllele);
}


IndividualAllele* IndividualLocus :: FindAllele (const RGString& name) const {

	IndividualAllele target;
	target.SetName (name);
	return (IndividualAllele*) mAlleles.Find (&target);
}


/*
IndividualAllele* IndividualLocus :: FindAlleleFromBioID (int id) {

	RGDListIterator it (mAllelesByName);
	IndividualAllele* nextAllele;

	while (nextAllele = (IndividualAllele*) it ()) {

		if (nextAllele->HasBioID (id))
			return nextAllele;
	}

	return NULL;
}
*/


void IndividualLocus :: WriteXML (RGTextOutput& text, const RGString& indent) {

	Endl endLine;
	RGString indent2 = indent + "\t";
	text << indent << "<Locus>" << endLine;
	text << indent2 << "<Name>" << mName << "</Name>" << endLine;

	IndividualAllele* nextAllele;
	RGDListIterator it (mAllelesByName);

	while (nextAllele = (IndividualAllele*) it ())
		nextAllele->WriteXML (text, indent2);

	text << indent << "</Locus>" << endLine;
}


int IndividualLocus :: CompareTo (const RGPersistent* p) const {

	IndividualLocus* q = (IndividualLocus*) p;
	return mName.CompareTo (&(q->mName));
}


unsigned IndividualLocus :: HashNumber (unsigned long Base) const {

	return mName.HashNumber (Base);
}


Boolean IndividualLocus :: IsEqualTo (const RGPersistent* p) const {

	IndividualLocus* q = (IndividualLocus*) p;
	return mName.IsEqualTo (&(q->mName));
}



LocusCollection :: LocusCollection () : RGPersistent (), mLoci (31), mValid (false) {

}



LocusCollection :: LocusCollection (const RGString& xmlString) : RGPersistent (), mLoci (31), mValid (true) {

	RGString XMLString (xmlString);
	RGBracketStringSearch locusSearch ("<Locus>", "</Locus>", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;
	IndividualLocus* newLocus;
	RGString locusString;

	while (locusSearch.FindNextBracketedString (startOffset, endOffset, locusString)) {

		startOffset = endOffset;
		newLocus = new IndividualLocus (locusString);

		if (!newLocus->isValid ())
			mValid = false;

		else {

			mLoci.Insert (newLocus);
			mLociByName.Append (newLocus);
		}
	}

	if (mLoci.Entries () == 0)
		mValid = false;
}



LocusCollection :: LocusCollection (const LocusCollection& collection) : RGPersistent (), mLoci (31), mLociByName (collection.mLociByName), 
mValid (collection.mValid) {

	IndividualLocus* nextLocus;
	RGDListIterator it (mLociByName);

	while (nextLocus = (IndividualLocus*) it ())
		mLoci.Insert (nextLocus);
}



LocusCollection :: ~LocusCollection () {

	mLociByName.Clear ();
	mLoci.ClearAndDelete ();
}


bool LocusCollection :: AddLoci (const RGString& xmlString) {

	RGString XMLString (xmlString);
	RGBracketStringSearch locusSearch ("<Locus>", "</Locus>", XMLString);
	size_t startOffset = 0;
	size_t endOffset = 0;
	IndividualLocus* newLocus;
	RGString locusString;
	bool returnValue = true;

	while (locusSearch.FindNextBracketedString (startOffset, endOffset, locusString)) {

		startOffset = endOffset;
		newLocus = new IndividualLocus (locusString);

		if (!newLocus->isValid ())
			returnValue = false;

		else {

			mLoci.Insert (newLocus);
			mLociByName.Append (newLocus);
		}
	}

	return returnValue;
}


IndividualLocus* LocusCollection :: FindLocus (const RGString& name) const {

	IndividualLocus target;
	target.SetName (name);
	return (IndividualLocus*) mLoci.Find (&target);
}



bool LocusCollection :: FindAllCopiesOfNamedLocus (const RGString& name, RGDList& results) {

	RGDListIterator it (mLociByName);
	IndividualLocus* nextLocus;

	while (nextLocus = (IndividualLocus*) it ()) {

		if (name == nextLocus->GetName ())
			results.Append (nextLocus);
	}

	if (results.Entries () > 0)
		return true;

	return false;
}


bool LocusCollection :: FindAllAllelesForNamedLocus (const RGString& name, RGDList& results) {

	RGDList temp;

	if (!FindAllCopiesOfNamedLocus (name, temp))
		return false;

	IndividualLocus* nextLocus;

	while (nextLocus = (IndividualLocus*) temp.GetFirst ())
		nextLocus->AppendAllelesToList (results); 
	
	if (results.Entries () == 0)
		return false;

	return true;
}



bool LocusCollection :: Matches (const LocusCollection& collection) const {

	if ((IsContainedIn (collection)) && (collection.IsContainedIn (*this)))
		return true;

	return false;
}



bool LocusCollection :: IsContainedIn (const LocusCollection& collection) const {

	RGDList tempLocusList (mLociByName);
	IndividualLocus* nextLocus;
	IndividualLocus* foundLocus;

	while (nextLocus = (IndividualLocus*) tempLocusList.GetFirst ()) {

		foundLocus = collection.FindLocus (nextLocus->GetName ());

		if ((foundLocus == NULL) || (!nextLocus->Matches (*foundLocus))) {

			delete nextLocus;
			tempLocusList.ClearAndDelete ();
			return false;
		}

		delete nextLocus;
	}

	return true;
}



void LocusCollection :: WriteXML (RGTextOutput& text, const RGString& indent) {

	IndividualLocus* nextLocus;
	RGDListIterator it (mLociByName);

	while (nextLocus = (IndividualLocus*) it ())
		nextLocus->WriteXML (text, indent);
}



IndividualGenotype :: IndividualGenotype () : RGPersistent (), mValid (false) {

	mLoci = new LocusCollection;
}


IndividualGenotype :: IndividualGenotype (const RGString& xmlString) : RGPersistent (), mValid (true) {

	RGString XMLString (xmlString);
	RGBracketStringSearch nameSearch ("<Name>", "</Name>", XMLString);
	RGBracketStringSearch fileCriterionSearch ("<FileNameString>", "</FileNameString>", XMLString);
	RGBracketStringSearch locusSearch ("<Loci>", "</Loci>", XMLString);
	size_t startOffset = 0;
	size_t endOffset = startOffset;
	RGString locusString;

	if (!nameSearch.FindNextBracketedString (startOffset, endOffset, mName))
		mValid = false;

	startOffset = endOffset;

	if (fileCriterionSearch.FindNextBracketedString (startOffset, endOffset, mFileNameSearchString))
		startOffset = endOffset;

	else
		mFileNameSearchString = mName;

	if (!locusSearch.FindNextBracketedString (startOffset, endOffset, locusString))
		mValid = false;

	else {

		mLoci = new LocusCollection (locusString);

		if (!mLoci->isValid ())
			mValid = false;
	}
}



IndividualGenotype :: IndividualGenotype (const IndividualGenotype& genotype) : RGPersistent (), mName (genotype.mName),
mFileNameSearchString (genotype.mFileNameSearchString), mValid (genotype.mValid) {

	mLoci = new LocusCollection (*genotype.mLoci);
}



IndividualGenotype :: ~IndividualGenotype () {

	delete mLoci;
}


IndividualLocus* IndividualGenotype :: FindLocus (const RGString& name) const {

	return mLoci->FindLocus (name);
}


bool IndividualGenotype :: Matches (const IndividualGenotype& genotype) const {

	return mLoci->Matches (*genotype.mLoci);
}



void IndividualGenotype :: WriteXML (RGTextOutput& text, const RGString& indent, const RGString& tag) {

	Endl endLine;
	RGString indent2 = indent + "\t";
	RGString indent3 = indent2 + "\t";
	text << indent.GetData () << "<" << tag << ">" << endLine;
	text << indent2 << "<Name>" << mName << "</Name>" << endLine;
	text << indent2 << "<Loci>" << endLine;
	mLoci->WriteXML (text, indent3);
	text << indent2 << "</Loci>" << endLine;
	text << indent.GetData () << "</" << tag << ">" << endLine;
}


int IndividualGenotype :: CompareTo (const RGPersistent* p) const {

	IndividualGenotype* q = (IndividualGenotype*) p;
	return mName.CompareTo (&(q->mName));
}


unsigned IndividualGenotype :: HashNumber (unsigned long Base) const {

	return mName.HashNumber (Base);
}



Boolean IndividualGenotype :: IsEqualTo (const RGPersistent* p) const {

	IndividualGenotype* q = (IndividualGenotype*) p;
	return mName.IsEqualTo (&(q->mName));
}


GenotypesForAMarkerSet :: GenotypesForAMarkerSet () : RGPersistent (),
mValid (true), mOffLadderAlleles (NULL), mSampleTriAlleles (NULL), mControlTriAlleles (NULL) {

}


GenotypesForAMarkerSet :: GenotypesForAMarkerSet (const RGString& markerSetName) : RGPersistent (),
mMarkerSetName (markerSetName), mValid (true), mOffLadderAlleles (NULL), mSampleTriAlleles (NULL), mControlTriAlleles (NULL) {

}



GenotypesForAMarkerSet :: GenotypesForAMarkerSet (const GenotypesForAMarkerSet& gFAMS) : RGPersistent (),
mMarkerSetName (gFAMS.mMarkerSetName), mValid (true),  mGenotypes (gFAMS.mGenotypes), mOffLadderAlleles (NULL), mSampleTriAlleles (NULL), mControlTriAlleles (NULL) {

	if (gFAMS.mOffLadderAlleles != NULL)
		mOffLadderAlleles = new LocusCollection (*gFAMS.mOffLadderAlleles);

	if (gFAMS.mSampleTriAlleles != NULL)
		mSampleTriAlleles = new LocusCollection (*gFAMS.mSampleTriAlleles);

	if (gFAMS.mControlTriAlleles != NULL)
		mControlTriAlleles = new LocusCollection (*gFAMS.mControlTriAlleles);
}


GenotypesForAMarkerSet :: GenotypesForAMarkerSet (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) :
mValid (true), mOffLadderAlleles (NULL), mSampleTriAlleles (NULL), mControlTriAlleles (NULL) {

	mValid = AddCollectionToTables (xmlString, offLadderTag, triallelesTag, posTriTag);
}



GenotypesForAMarkerSet :: ~GenotypesForAMarkerSet () {

	mGenotypes.ClearAndDelete ();
	delete mOffLadderAlleles;
	delete mSampleTriAlleles;
	delete mControlTriAlleles;
}


bool GenotypesForAMarkerSet :: AddGenotype (IndividualGenotype* ig) {

	if (ig == NULL)
		return false;
	
	mGenotypes.Insert (ig);
	return true;
}


bool GenotypesForAMarkerSet :: AddCollection (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	return AddCollectionToTablesFromLabSettings (xmlString, offLadderTag, triallelesTag, posTriTag);
}



IndividualGenotype* GenotypesForAMarkerSet :: FindGenotypeForFileName (const RGString& fileName) {

	RGDListIterator it (mGenotypes);
	IndividualGenotype* genotype;
	RGString name;
	size_t position = 0;

	while (genotype = (IndividualGenotype*) it ()) {

//		name = genotype->GetName ();
		name = genotype->GetFileNameSearchCriterion ();

		if (fileName.FindSubstringCaseIndependent (name, position))
			return genotype;
	}

	return NULL;
}



IndividualGenotype* GenotypesForAMarkerSet :: FindGenotypeThatMatchesFileName (const RGString& fileName) {

	RGDListIterator it (mGenotypes);
	IndividualGenotype* genotype;
	RGString name;
	size_t position = 0;

	while (genotype = (IndividualGenotype*) it ()) {

		name = genotype->GetName ();

		if (fileName == name)
			return genotype;
	}

	return NULL;
}


bool GenotypesForAMarkerSet :: ContainsOffLadderAllele (const RGString& locusName, const RGString& alleleName) {

	RGDList alleleList;
	bool returnValue;

	if (mOffLadderAlleles == NULL)
		return false;

	if (!mOffLadderAlleles->FindAllAllelesForNamedLocus (locusName, alleleList)) {

		alleleList.Clear ();
		return false;
	}

	IndividualAllele target;
	target.SetName (alleleName);

	if (alleleList.Contains (&target))
		returnValue = true;

	else
		returnValue = false;

	alleleList.Clear ();
	return returnValue;
}


/*
bool GenotypesForAMarkerSet :: ContainsOffLadderAllele (const RGString& locusName, int bioID) {

	RGDList alleleList;

	if (mOffLadderAlleles == NULL)
		return false;

	if (!mOffLadderAlleles->FindAllAllelesForNamedLocus (locusName, alleleList)) {

		alleleList.Clear ();
		return false;
	}

	IndividualAllele* nextAllele;

	while (nextAllele = (IndividualAllele*) alleleList.GetFirst ()) {

		if (nextAllele->HasBioID (bioID)) {

			alleleList.Clear ();
			return true;
		}
	}

	return false;
}
*/


bool GenotypesForAMarkerSet :: ContainsControlTriallele (const RGString& locusName, RGDList& alleles) {

	RGDList locusCollection;

	if (mControlTriAlleles == NULL)
		return false;

	if (!mControlTriAlleles->FindAllCopiesOfNamedLocus (locusName, locusCollection))
		return false;

	RGDListIterator itCollection (locusCollection);
	IndividualLocus* nextLocus;

	while (nextLocus = (IndividualLocus*) itCollection ()) {

		if (nextLocus->IsContainedIn (alleles)) {

			locusCollection.Clear ();
			return true;
		}
	}
	
	locusCollection.Clear ();
	return false;
}



bool GenotypesForAMarkerSet :: ContainsSampleTriallele (const RGString& locusName, RGDList& alleles) {

	RGDList locusCollection;

	if (mSampleTriAlleles == NULL)
		return false;

	if (!mSampleTriAlleles->FindAllCopiesOfNamedLocus (locusName, locusCollection))
		return false;

	RGDListIterator itCollection (locusCollection);
	IndividualLocus* nextLocus;

	while (nextLocus = (IndividualLocus*) itCollection ()) {

		if (nextLocus->IsContainedIn (alleles)) {

			locusCollection.Clear ();
			return true;
		}
	}
	
	locusCollection.Clear ();
	return false;
}



void GenotypesForAMarkerSet :: WriteXML (RGTextOutput& text, const RGString& indent, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	RGDListIterator it (mGenotypes);
	IndividualGenotype* nextGenotype;

	Endl endLine;
	RGString indent2 = indent + "\t";
	text << indent << "<MarkerSetName>" << mMarkerSetName << "</MarkerSetName>" << endLine;
	text << indent << "<PositiveControls>" << endLine;

	while (nextGenotype = (IndividualGenotype*) it ())
		nextGenotype->WriteXML (text, indent2, "PositiveControl");

	text << indent << "</PositiveControls>" << endLine;

	text << indent << "<" << offLadderTag << ">" << endLine;
	mOffLadderAlleles->WriteXML (text, indent2);
	text << indent << "</" << offLadderTag << ">" << endLine;

	text << indent << "<" << triallelesTag << ">" << endLine;
	mSampleTriAlleles->WriteXML (text, indent2);
	text << indent << "</" << triallelesTag << ">" << endLine;

	text << indent << "<" << posTriTag << ">" << endLine;
	mControlTriAlleles->WriteXML (text, indent2);
	text << indent << "</" << posTriTag << ">" << endLine;
}



int GenotypesForAMarkerSet :: CompareTo (const RGPersistent* p) const {

	GenotypesForAMarkerSet* q = (GenotypesForAMarkerSet*) p;
	return mMarkerSetName.CompareTo (&(q->mMarkerSetName));
}


unsigned GenotypesForAMarkerSet :: HashNumber (unsigned long Base) const {

	return mMarkerSetName.HashNumber (Base);
}


Boolean GenotypesForAMarkerSet :: IsEqualTo (const RGPersistent* p) const {

	GenotypesForAMarkerSet* q = (GenotypesForAMarkerSet*) p;
	return mMarkerSetName.IsEqualTo (&(q->mMarkerSetName));
}


bool GenotypesForAMarkerSet :: AddCollectionToTables (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	RGString XMLString (xmlString);
	RGString positiveControlString;
	RGString pControlsString;
	RGString fullPathControlFileName = GenotypesForAMarkerSet::PathToStandardControlFile + "/LadderSpecifications/StandardPositiveControls.xml";

	RGFile* allPositiveControls = new RGFile (fullPathControlFileName, "rt");

	if (!allPositiveControls->isValid ()) {

		cout << "Could not find positive controls file:  " << (char*)fullPathControlFileName.GetData () << endl;
		return false;
	}

	RGString posXMLString;
	posXMLString.ReadTextFile (*allPositiveControls);
	delete allPositiveControls;

	RGXMLTagSearch nameSearch ("MarkerSetName", XMLString);
	RGXMLTagSearch positiveControlsSearch ("PositiveControls", posXMLString);
	RGXMLTagSearch oldPositiveControlsSearch ("PositiveControls", XMLString);
	RGXMLTagSearch posCtrlSearch ("PositiveControl", pControlsString);
	RGXMLTagSearch offLadderSearch (offLadderTag, XMLString);
	RGXMLTagSearch trialleleSearch (triallelesTag, XMLString);
	RGXMLTagSearch posTriSearch (posTriTag, XMLString);

	size_t startOffset = 0;
	size_t endOffset = 0;
	RGString collectionString;
	
	RGString offLadderString;
	RGString triString;
	RGString posTriString;
	size_t startPControls = 0;
	size_t endPControls = 0;
	bool returnValue = true;
	RGString markerSetName;
	IndividualGenotype* nextGenotype;

	if (!nameSearch.FindNextTag (startOffset, endOffset, markerSetName))
		return false;

	if (mMarkerSetName.Length () == 0)
		mMarkerSetName = markerSetName;

	else if (mMarkerSetName != markerSetName)
		return false;

	startOffset = endOffset;
	size_t startOffsetPos = 0;
	size_t endOffsetPos = 0;
	RGString temp;

	if (oldPositiveControlsSearch.FindNextTag (startOffset, endOffset, temp))
		startOffset = endOffset;

	if (positiveControlsSearch.FindNextTag (startOffsetPos, endOffsetPos, pControlsString)) {

		posCtrlSearch.ResetSearch ();
//		cout << "Found positive controls in file" << endl;

		while (posCtrlSearch.FindNextTag (startPControls, endPControls, positiveControlString)) {

//			cout << "Found positive control..." << endl;
			startPControls = endPControls;
			nextGenotype = new IndividualGenotype (positiveControlString);

			if (!nextGenotype->isValid ()) {

				returnValue = false;
				delete nextGenotype;
			}

			else {

				if (!mGenotypes.Contains (nextGenotype)) {
					mGenotypes.Insert (nextGenotype);
//					cout << "Inserted genotype in list..." << endl;
				}

				else {

					returnValue = false;
					delete nextGenotype;
				}
			}
		}
	}

	startOffset = endOffset;

	if (offLadderSearch.FindNextTag (startOffset, endOffset, offLadderString)) {

		if (mOffLadderAlleles == NULL) {

			mOffLadderAlleles = new LocusCollection (offLadderString);

			if (!mOffLadderAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mOffLadderAlleles->AddLoci (offLadderString))
				returnValue = false;
		}
	}

	startOffset = endOffset;

	if (trialleleSearch.FindNextTag (startOffset, endOffset, triString)) {

		if (mSampleTriAlleles == NULL) {

			mSampleTriAlleles = new LocusCollection (triString);

			if (!mSampleTriAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mSampleTriAlleles->AddLoci (triString))
				returnValue = false;
		}
	}

	startOffset = endOffset;

	if (posTriSearch.FindNextTag (startOffset, endOffset, posTriString)) {

		if (mControlTriAlleles == NULL) {

			mControlTriAlleles = new LocusCollection (posTriString);

			if (!mControlTriAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mControlTriAlleles->AddLoci (posTriString))
				returnValue = false;
		}
	}

	return returnValue;
}



bool GenotypesForAMarkerSet :: AddCollectionToTablesFromLabSettings (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	RGString XMLString (xmlString);
	RGString positiveControlString;
	RGString pControlsString;

	RGXMLTagSearch nameSearch ("MarkerSetName", XMLString);
	RGXMLTagSearch positiveControlsSearch ("PositiveControls", XMLString);
	RGXMLTagSearch posCtrlSearch ("PositiveControl", pControlsString);
	RGXMLTagSearch offLadderSearch (offLadderTag, XMLString);
	RGXMLTagSearch trialleleSearch (triallelesTag, XMLString);
	RGXMLTagSearch posTriSearch (posTriTag, XMLString);

	size_t startOffset = 0;
	size_t endOffset = 0;
	RGString collectionString;
	
	RGString offLadderString;
	RGString triString;
	RGString posTriString;
	size_t startPControls = 0;
	size_t endPControls = 0;
	bool returnValue = true;
	RGString markerSetName;
	IndividualGenotype* nextGenotype;

	if (!nameSearch.FindNextTag (startOffset, endOffset, markerSetName))
		return false;

	if (mMarkerSetName.Length () == 0)
		mMarkerSetName = markerSetName;

	else if (mMarkerSetName != markerSetName)
		return false;

	startOffset = endOffset;

	if (positiveControlsSearch.FindNextTag (startOffset, endOffset, pControlsString)) {

		posCtrlSearch.ResetSearch ();
		startOffset = endOffset;
		cout << "Found positive controls in lab settings file" << endl;

		while (posCtrlSearch.FindNextTag (startPControls, endPControls, positiveControlString)) {

			cout << "Found positive control..." << endl;
			startPControls = endPControls;
			nextGenotype = new IndividualGenotype (positiveControlString);

			if (!nextGenotype->isValid ()) {

				returnValue = false;
				delete nextGenotype;
			}

			else {

				if (!mGenotypes.Contains (nextGenotype)) {
					mGenotypes.Insert (nextGenotype);
					cout << "Inserted genotype in list..." << endl;
				}

				else {

					//returnValue = false;
					cout << "\nWARNING***Found duplicate positive control named " << nextGenotype->GetName ().GetData () << ".  Ignoring...\n" << endl;
					delete nextGenotype;
				}
			}
		}
	}

	startOffset = endOffset;

	if (offLadderSearch.FindNextTag (startOffset, endOffset, offLadderString)) {

		if (mOffLadderAlleles == NULL) {

			mOffLadderAlleles = new LocusCollection (offLadderString);

			if (!mOffLadderAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mOffLadderAlleles->AddLoci (offLadderString))
				returnValue = false;
		}
	}

	startOffset = endOffset;

	if (trialleleSearch.FindNextTag (startOffset, endOffset, triString)) {

		if (mSampleTriAlleles == NULL) {

			mSampleTriAlleles = new LocusCollection (triString);

			if (!mSampleTriAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mSampleTriAlleles->AddLoci (triString))
				returnValue = false;
		}
	}

	startOffset = endOffset;

	if (posTriSearch.FindNextTag (startOffset, endOffset, posTriString)) {

		if (mControlTriAlleles == NULL) {

			mControlTriAlleles = new LocusCollection (posTriString);

			if (!mControlTriAlleles->isValid ())
				returnValue = false;
		}

		else {

			if (!mControlTriAlleles->AddLoci (posTriString))
				returnValue = false;
		}
	}

	return returnValue;
}



GenotypeSet :: GenotypeSet () : mValid (true) {

}


GenotypeSet :: GenotypeSet (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) : 
mValid (true) {

	mValid = AddGenotypesToTables (xmlString, collectionTag, offLadderTag, triallelesTag, posTriTag);
}


GenotypeSet :: GenotypeSet (const RGString& xmlString, bool isLabSettings) : mValid (true) {

	if (isLabSettings)
		mValid = AddGenotypesToTables (xmlString, "LabMarkerSetCollection", "OffLadderAlleles", "SampleAcceptedTrialleles", "PosCtlAcceptedTrialleles");

	else
		mValid = AddGenotypesToTables (xmlString, "StdMarkerSetCollection", "StandardOffLadderAlleles", "StandardTrialleles", "StdPosCtlTrialleles");
}


GenotypeSet :: ~GenotypeSet () {

	mGenotypes.ClearAndDelete ();
}



bool GenotypeSet :: AddGenotypes (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	return AddGenotypesToTables (xmlString, collectionTag, offLadderTag, triallelesTag, posTriTag);
}


bool GenotypeSet :: AddGenotypes (const RGString& xmlString, bool isLabSettings) {

	if (isLabSettings)
		return AddGenotypesToTables (xmlString, "LabMarkerSetCollection", "OffLadderAlleles", "SampleAcceptedTrialleles", "PosCtlAcceptedTrialleles");

	return AddGenotypesToTables (xmlString, "StdMarkerSetCollection", "StandardOffLadderAlleles", "StandardTrialleles", "StdPosCtlTrialleles");
}



bool GenotypeSet :: AddGenotype (IndividualGenotype* ig, const RGString& markerSetName) {

	if (ig == NULL)
		return false;
	
	GenotypesForAMarkerSet* genotypeContainer = FindMarkerSetCollection (markerSetName);

	if (genotypeContainer == NULL) {

		genotypeContainer = new GenotypesForAMarkerSet (markerSetName);
		mGenotypes.Append (genotypeContainer);
	}

	if (!genotypeContainer->AddGenotype (ig))
		return false;

	return true;
}



IndividualGenotype* GenotypeSet :: FindGenotype (const RGString& fileName, const RGString& markerSetName) {

	GenotypesForAMarkerSet* genotypeContainer = FindMarkerSetCollection (markerSetName);

	if (genotypeContainer == NULL)
		return NULL;

	return genotypeContainer->FindGenotypeForFileName (fileName);
}



GenotypesForAMarkerSet* GenotypeSet :: FindMarkerSetCollection (const RGString& markerSetName) {

	GenotypesForAMarkerSet target;
	target.SetMarkerSetName (markerSetName);
	return (GenotypesForAMarkerSet*) mGenotypes.Find (&target);
}


bool GenotypeSet :: FindAllGenotypes (const RGString& fileName, RGDList& genotypes) {

	RGDListIterator it (mGenotypes);
	GenotypesForAMarkerSet* nextSet;
	IndividualGenotype* ig;
	int startEntries = genotypes.Entries ();
	int endEntries;

	while (nextSet = (GenotypesForAMarkerSet*) it ()) {

		ig = nextSet->FindGenotypeForFileName (fileName);

		if (ig != NULL)
			genotypes.Append (ig);
	}

	endEntries = genotypes.Entries ();

	if (endEntries > startEntries)
		return true;

	return false;
}


void GenotypeSet :: WriteXML (RGTextOutput& text, const RGString& indent, const RGString& tag) {

	RGDListIterator it (mGenotypes);
	GenotypesForAMarkerSet* nextGenotype;
	Endl endLine;

	text << indent << "<MarkerSetCollection>" << endLine;
	RGString indent2 = indent + "\t";

	while (nextGenotype = (GenotypesForAMarkerSet*) it ())
		nextGenotype->WriteXML (text, indent2, "OffLadderAlleles", "SampleTriAlleles", "ControlTriAlleles");

	text << indent << "</MarkerSetCollection>" << endLine;
}



bool GenotypeSet :: AddGenotypesToTables (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag) {

	RGString XMLString (xmlString);

	RGXMLTagSearch collectionSearch (collectionTag, XMLString);

	size_t startOffset = 0;
	size_t endOffset;
	RGString collectionString;
	bool returnValue = true;
	size_t nameStart = 0;
	size_t nameEnd;
	RGString name;
	GenotypesForAMarkerSet* newCollection;
	GenotypesForAMarkerSet* foundCollection;

	RGXMLTagSearch nameSearch ("MarkerSetName", collectionString);

	while (collectionSearch.FindNextTag (startOffset, endOffset, collectionString)) {

		nameSearch.ResetSearch ();
		startOffset = endOffset;

		if (!nameSearch.FindNextTag (nameStart, nameEnd, name)) {

			returnValue = false;
			continue;
		}

		foundCollection = FindMarkerSetCollection (name);

		if (foundCollection == NULL) {

			newCollection = new GenotypesForAMarkerSet (collectionString, offLadderTag, triallelesTag, posTriTag);

			if (!newCollection->isValid ()) {

				returnValue = false;
				delete newCollection;
			}

			else
				mGenotypes.Append (newCollection);
		}

		else {

			if (!foundCollection->AddCollection (collectionString, offLadderTag, triallelesTag, posTriTag))
				returnValue = false;
		}
	}

	return returnValue;
}


