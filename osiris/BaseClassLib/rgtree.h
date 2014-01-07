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
*  FileName: rgtree.h
*  Author:   Robert Goor
*
*/
//
//   class RGTree which is persistent and which manages subtrees as well as persistent data stored in objects of
//  class RGDocument, also persistent
//

#ifndef _RGTREE_H_
#define _RGTREE_H_

#include "rgpersist.h"
#include "rgdlist.h"
#include "rgstring.h"


const int _RGTREE_ = 4;
const int _RGDOCUMENT_ = 5;


class RGTree;
class RGDirectory;
class RGSuffixList;

PERSISTENT_PREDECLARATION (RGDocument)
PERSISTENT_PREDECLARATION (RGTree)


class RGDocument : public RGPersistent {

PERSISTENT_DECLARATION (RGDocument)

public:
	RGDocument ();
	RGDocument (RGTree* parent);
	RGDocument (RGTree* parent, const RGString& name);
	RGDocument (const RGString& name);
	virtual ~RGDocument ();

	void SetName (const RGString& name) { Name = name; }
	void SetParent (RGTree* parent) { Parent = parent; }
	void BuildFullPathName (const char* delimiter, RGString& name);
	void BuildRelativePathName (const char* delimiter, RGString& name);
	void BuildRelativeContainerPath (const char* delimiter, RGString& name);

	static void SetCompareOnName (Boolean choice) { CompareOnName = choice; }

	RGTree* GetParent () { return Parent; }
	const char* GetName () const { return Name.GetData (); }

	Boolean HasName (const char* name) const;


	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	void WriteXML (RGFile& file, const RGString& tabLevel, const RGString& docLabel);
	void WriteXMLOpen (RGFile& file, const RGString& tabLevel, const RGString& docLabel);
	virtual void WriteXMLData (RGFile& file, const RGString& tabLevel);
	void WriteXMLClose (RGFile& file, const RGString& tabLevel, const RGString& docLabel);


protected:
	RGTree* Parent;
	RGString Name;

	RGDocument (const RGDocument& doc);
	static Boolean CompareOnName;
};


class RGTree : public RGPersistent {

	friend class RGTreeIterator;
	PERSISTENT_DECLARATION (RGTree)

public:
	RGTree ();
	RGTree (RGTree* parent);
	RGTree (RGTree* parent, const RGString& name);
	RGTree (const RGString& name);
	~RGTree ();

	void SetName (const RGString& name) { Name = name; }
	void SetParent (RGTree* parent) { Parent = parent; }
	void RecursivelySetParent (RGTree* parent);
	void BuildFullPathName (const char* delimiter, RGString& name);
	void BuildRelativePathName (const char* delimiter, RGString& name);
	size_t NumberOfDocuments () const { return DataList.Entries (); }
	size_t NumberOfSubtrees () const { return Subtrees.Entries (); }
	Boolean isRoot () const { return (Parent == NULL); }

	RGTree* AddSubtree (RGTree* NewTree);
	RGDocument* AddDocument (RGDocument* NewDocument);
	RGTree* InsertSubtree (RGTree* NewTree);
	RGDocument* InsertDocument (RGDocument* NewDocument);
	RGTree* RemoveSubtree (RGTree* Target);
	RGTree* RemoveSubtree (const RGString& name);
	RGDocument* RemoveDocument (RGDocument* Target);
	void ClearAndDeleteData ();
	void ClearAndDeleteAllData ();
	void ClearAndDeleteAll ();

	RGTree* CopyTreeAndSubtreesRecursively (const RGString& newName);

	RGDocument* AddDocumentFromPath (const RGString& path, RGDocument* NewDocument, Boolean overWrite);
	RGDocument* AddDocumentFromRelativePath (const RGString& path, RGDocument* NewDocument, Boolean overWrite);
	RGDocument* AddDocumentFromReducedPath (const RGString& path, RGDocument* NewDocument, Boolean overWrite);

	int BuildTreeFromDirectories (const RGString& FileSuffix, const RGDocument* DocumentType);
	int BuildTreeFromDirectoriesAndSuffixList (RGSuffixList& SuffixList, const RGDocument* DocumentType);
	int BuildTree ();
	Boolean PruneTreesWithNoData ();
	Boolean ClearDirectoryTree ();

	Boolean CreateDirectoriesRecursivelyFromSubtrees ();

	RGTree* GetParent () { return Parent; }
	const char* GetName () const { return Name.GetData (); }
	Boolean HasName (const char* name) const;
	size_t GetLevel () const;

	RGTree* FindTreeFromTreeName (const RGString& name);
	RGDocument* FindDocumentFromTreeAndDocumentName (const RGString& treeName, const RGString& docName);
	RGDocument* FindDocumentFromDocumentName (const RGString& name);
	RGDocument* FindDocumentFromFullPath (const RGString& fullpath);
	RGDocument* FindDocumentFromPathAndDocumentName (const RGString& path, const RGString& name);
	RGDocument* FindDocumentFromRelativePathAndDocumentName (const RGString& path, const RGString& name);
	RGDocument* FindDocumentFromReducedPathAndDocumentName (const RGString& path, const RGString& name);

	void ResetSubtreeList ();
	void ResetAllSubtreeLists ();
	void ResetDocumentList ();
	void ResetAllDocumentLists ();
	void ResetAllLists ();

	RGTree* NextSubtree ();
	RGDocument* NextDocument ();

	RGTree* NextDeepSubtree ();
	RGDocument* NextDeepDocument ();
	RGTree* NextTopDown ();

	static void SetNotifyOn () { Notify = TRUE; }
	static void SetNotifyOff () { Notify = FALSE; }
	static void ResetNotifyFrequency (int freq);

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	void WriteXML (RGFile& file, const RGString& tabLevel, const RGString& dirLabel, const RGString& subdirLabel,
		const RGString& docLabel);
	void WriteXMLOpen (RGFile& file, const RGString& tabLevel, const RGString& dirLabel);
	virtual void WriteXMLData (RGFile& file, const RGString& tabLevel, const RGString& subdirLabel,	const RGString& docLabel);
	void WriteXMLClose (RGFile& file, const RGString& tabLevel, const RGString& docLabel);

protected:
	RGDList Subtrees;
	RGTree* Parent;
	RGDList DataList;
	RGString Name;
	RGDListIterator* SubtreeIterator;
	RGDListIterator* DataIterator;

	Boolean BeforeFirstSubtree;
	Boolean InDeepMode;
	Boolean SentTreetop;
	Boolean SentSelf;
	RGTree* SendingTree;

	static Boolean Notify;
	static int Frequency;
	static int CurrentRepetition;

	RGTree* TreetopTest (RGTree* target);
	Boolean DiskObjectIsDirectory (const RGString& FileName, const RGString& DirectoryName, RGDirectory* dir);
	Boolean DiskObjectIsFileWithSuffix (const RGString& FileSuffix, const RGString& FileName);
	Boolean DiskObjectIsFileWithListedSuffix (RGSuffixList& SuffixList, const RGString& FileName);
	void NotifyWorking ();
	RGTree* CopyTreeAndSubtreesRecursively (RGTree* parent);

	RGTree (const RGTree& tree);
};


class RGTreeIterator { // this is a top-down directory (or subtree) iterator

public:
	RGTreeIterator (RGTree&);
	~RGTreeIterator ();

	void Reset ();
	RGTree* NextSubtree (int& level);
	RGTree* CurrentItem ();

protected:
	RGDListIterator* CurrentIterator;
	RGDListIterator* SecondaryIterator;
	RGDList* CurrentList;
	RGDList* SecondaryList;
	
	RGTree& Root;
	int CurrentLevel;
	RGTree* CurrentSubtree;
};


class RGSuffixList {

public:
	RGSuffixList ();
	~RGSuffixList ();

	void AddSuffix (const char* suffix);
	void ResetList ();
	RGString* NextSuffix ();
	void ClearAndDelete ();
	size_t Entries () const { return SuffixList.Entries (); }

protected:
	RGDList SuffixList;
	RGDListIterator SuffixIterator;
};

#endif  /*  _RGTREE_H_  */
