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
*  FileName: IndividualGenotype.h
*  Author:   Robert Goor
*
*/
//
//  classes IndividualAllele, IndividualLocus and IndividualGenotype, designed to retain an individual genotype and to be compared to a CoreBioComponent.  These will be used to store positive controls.
//

#ifndef _INDIVIDUALGENOTYPE_H_
#define _INDIVIDUALGENOTYPE_H_


#include "rgpersist.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rghashtable.h"
#include "rgdefs.h"
#include "SynonymList.h"

#include "CoreBioComponent.h"
#include "Genetics.h"
#include "RGTextOutput.h"


PERSISTENT_PREDECLARATION (IndividualAllele)
PERSISTENT_PREDECLARATION (IndividualLocus)
PERSISTENT_PREDECLARATION (LocusCollection)
PERSISTENT_PREDECLARATION (IndividualGenotype)
PERSISTENT_PREDECLARATION (GenotypesForAMarkerSet)


const int _INDIVIDUALALLELE_ = 1501;
const int _INDIVIDUALLOCUS_ = 1502;
const int _INDIVIDUALGENOTYPE_ = 1503;
const int _GENOTYPESFORAMARKERSET_ = 1504;
const int _LOCUSCOLLECTION_ = 1505;

class IndividualGenotype;
class DataSignal;


class IndividualAllele : public RGPersistent {

PERSISTENT_DECLARATION (IndividualAllele)

public:
	IndividualAllele ();
	IndividualAllele (const RGString& xmlString);
	IndividualAllele (const IndividualAllele& allele);
	virtual ~IndividualAllele ();

	bool isValid () const { return mValid; }
	void SetName (const RGString& name) { mName = name; }
	void SetAssociatedPeak (DataSignal* ds);

	RGString GetName () const { return mName; }
//	bool HasBioID (int id) { return id == mBioID; }
	DataSignal* GetAssociatedPeak ();

	void WriteXML (RGTextOutput& text, const RGString& indent);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	friend class Allele;

protected:
	RGString mName;
//	int mBioID;
	bool mValid;
	DataSignal* mAssociatedPeak;
};


class IndividualLocus : public RGPersistent {

PERSISTENT_DECLARATION (IndividualLocus)

public:
	IndividualLocus ();
	IndividualLocus (const RGString& xmlString);
	IndividualLocus (const IndividualLocus& locus);
	virtual ~IndividualLocus ();

	bool isValid () const { return mValid; }
	void SetName (const RGString& name) { mName = name; }

	int GetNumberOfAlleles () const { return mAlleles.Entries (); }
	RGString GetName () const { return mName; }

	bool Matches (const IndividualLocus& locus) const;
	Boolean ContainsAllele (const IndividualAllele& allele) const;
	bool IsContainedIn (const IndividualLocus& locus) const;
	bool IsContainedIn (const RGDList& alleleList) const;
	void AppendAllelesToList (RGDList& alleleList);

	IndividualAllele* FindAllele (const RGString& name) const;
//	IndividualAllele* FindAlleleFromBioID (int id);

	void WriteXML (RGTextOutput& text, const RGString& indent);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	friend class Locus;

protected:
	RGString mName;
	RGHashTable mAlleles;
	RGDList mAllelesByName;
	bool mValid;
};


class LocusCollection : public RGPersistent {

	PERSISTENT_DECLARATION (LocusCollection)

public:
	LocusCollection ();
	LocusCollection (const RGString& xmlString);
	LocusCollection (const LocusCollection& collection);
	virtual ~LocusCollection ();

	bool isValid () const { return mValid; }
	bool AddLoci (const RGString& xmlString);

	int GetNumberOfLoci () const { return mLoci.Entries (); }
	IndividualLocus* FindLocus (const RGString& name) const;
	bool FindAllCopiesOfNamedLocus (const RGString& name, RGDList& results);
	bool FindAllAllelesForNamedLocus (const RGString& name, RGDList& results);

	bool Matches (const LocusCollection& collection) const;
	bool IsContainedIn (const LocusCollection& collection) const;

	void WriteXML (RGTextOutput& text, const RGString& indent);

//	virtual int CompareTo (const RGPersistent* p) const;
//	virtual unsigned HashNumber (unsigned long Base) const;
//	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	friend class CoreBioComponent;
	friend class IndividualGenotype;

protected:
	RGHashTable mLoci;
	RGDList mLociByName;
	bool mValid;
};


class IndividualGenotype : public RGPersistent {

	// IndividualGenotype compares equal based on genotype name; this is just a named LocusCollection

PERSISTENT_DECLARATION (IndividualGenotype)

public:
	IndividualGenotype ();
	IndividualGenotype (const RGString& xmlString);
	IndividualGenotype (const IndividualGenotype& genotype);
	virtual ~IndividualGenotype ();

	bool isValid () const { return mValid; }
	void SetName (const RGString& name) { mName = name; if (mFileNameSearchString.Length () == 0) mFileNameSearchString = name; }

	int GetNumberOfLoci () const { return mLoci->GetNumberOfLoci (); }
	RGString GetName () const { return mName; }
	RGString GetFileNameSearchCriterion () const { return mFileNameSearchString; }
	IndividualLocus* FindLocus (const RGString& name) const;
	bool Matches (const IndividualGenotype& genotype) const;
	bool IsAssignedPositiveControl () const { return mIsAssignedPositiveCtl; }
	void SetAssignedPositiveControl (bool assigned) { mIsAssignedPositiveCtl = assigned; }

	void WriteXML (RGTextOutput& text, const RGString& indent, const RGString& tag);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	friend class CoreBioComponent;

protected:
	RGString mName;
	RGString mFileNameSearchString;
	LocusCollection* mLoci;
	bool mValid;
	bool mIsAssignedPositiveCtl;
};


class GenotypesForAMarkerSet : public RGPersistent {

	// All IndividualGenotypes stored in this class object have the same marker set name...
	// Search for IndividualGenotype on the basis of genotype name only...
	// GenotypesForAMarkerSet compares equal based on marker set name

PERSISTENT_DECLARATION (GenotypesForAMarkerSet)

public:
	GenotypesForAMarkerSet ();
	GenotypesForAMarkerSet (const RGString& markerSetName);
	GenotypesForAMarkerSet (const GenotypesForAMarkerSet& gFAMS);
	GenotypesForAMarkerSet (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
	virtual ~GenotypesForAMarkerSet ();

	bool isValid () const { return mValid; }

	void SetMarkerSetName (const RGString& name) { mMarkerSetName = name; }
	bool AddGenotype (IndividualGenotype* ig);
	bool AddCollection (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);

	RGString GetMarkerSetName () const { return mMarkerSetName; }

	bool AddLabPositiveControlStringsToControlLists (SynonymList* controlIDs, SynonymList* posControlIDs);
	IndividualGenotype* FindGenotypeForFileName (const RGString& fileName);
	IndividualGenotype* FindGenotypeThatMatchesFileName (const RGString& fileName);

	bool ContainsOffLadderAllele (const RGString& locusName, const RGString& alleleName);
//	bool ContainsOffLadderAllele (const RGString& locusName, int bioID);

	bool ContainsControlTriallele (const RGString& locusName, RGDList& alleles);
	bool ContainsSampleTriallele (const RGString& locusName, RGDList& alleles);

	void WriteXML (RGTextOutput& text, const RGString& indent, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);

	virtual int CompareTo (const RGPersistent* p) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent* p) const;

	static void SetPathToStandardControlFile (const RGString& path) { PathToStandardControlFile = path; }
	static RGString GetPathToStandardControlFile () { return PathToStandardControlFile; }

protected:
	RGString mMarkerSetName;
	bool mValid;
	RGDList mGenotypes;  // list of individual genotypes, i.e., positive controls, for this marker set
	LocusCollection* mOffLadderAlleles;  // set of loci and alleles that are allowable off ladder alleles
	LocusCollection* mSampleTriAlleles;  // set of loci and tri (or more) alleles allowable for samples
	LocusCollection* mControlTriAlleles;  // set of loci and tri (or more) alleles allowable on positive controls

	static RGString PathToStandardControlFile;

	bool AddCollectionToTables (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
	bool AddCollectionToTablesFromLabSettings (const RGString& xmlString, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
};


class GenotypeSet {

public:
	GenotypeSet ();
	GenotypeSet (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
	GenotypeSet (const RGString& xmlString, bool isLabSettings);
	~GenotypeSet ();

	bool AddGenotypes (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
	bool AddGenotypes (const RGString& xmlString, bool isLabSettings);
	bool AddGenotype (IndividualGenotype* ig, const RGString& markerSetName);

	IndividualGenotype* FindGenotype (const RGString& fileName, const RGString& markerSetName);
	GenotypesForAMarkerSet* FindMarkerSetCollection (const RGString& markerSetName);
	bool FindAllGenotypes (const RGString& fileName, RGDList& genotypes);

	void WriteXML (RGTextOutput& text, const RGString& indent, const RGString& tag);

	bool isValid () const { return mValid; }

protected:
	RGDList mGenotypes;
	bool mValid;

	bool AddGenotypesToTables (const RGString& xmlString, const RGString& collectionTag, const RGString& offLadderTag, const RGString& triallelesTag, const RGString& posTriTag);
};




#endif  /*  _INDIVIDUALGENOTYPE_H_  */

