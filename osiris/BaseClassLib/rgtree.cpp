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
*  FileName: rgtree.cpp
*  Author:   Robert Goor
*
*/
//
//   class RGTree which is persistent and which manages subtrees as well as persistent data stored in objects of
//  class RGDocument, also persistent
//


#include <ctype.h>
#include <iostream>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "rgtree.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgtokenizer.h"
#include "rgdirectory.h"


Boolean RGTree :: Notify = FALSE;
int RGTree :: Frequency = 50;
int RGTree :: CurrentRepetition = 0;

PERSISTENT_DEFINITION (RGDocument, _RGDOCUMENT_, "Document")
PERSISTENT_DEFINITION (RGTree, _RGTREE_, "Tree")

Boolean RGDocument :: CompareOnName = TRUE;

//using namespace std;

RGDocument :: RGDocument () : RGPersistent (), Parent (NULL) {

}


RGDocument :: RGDocument (RGTree* parent) : RGPersistent (), Parent (parent) {

}


RGDocument :: RGDocument (RGTree* parent, const RGString& name) : RGPersistent (), Parent (parent), Name (name) {

}


RGDocument :: RGDocument (const RGString& name) : RGPersistent (), Parent (NULL), Name (name) {

}


RGDocument :: RGDocument (const RGDocument& doc) : RGPersistent (doc), Parent (doc.Parent), Name (doc.Name) {

}


RGDocument :: ~RGDocument () {

}


void RGDocument :: BuildFullPathName (const char* delimiter, RGString& name) {

	Parent->BuildFullPathName (delimiter, name);
	name += delimiter + Name;
}

void RGDocument :: BuildRelativePathName (const char* delimiter, RGString& name) {

	if (Parent->isRoot ())
		name = Name;

	else {

		Parent->BuildRelativePathName (delimiter, name);
		name += delimiter + Name;
	}
}


void RGDocument :: BuildRelativeContainerPath (const char* delimiter, RGString& name) {

	if (Parent->isRoot ())
		name = "";

	else
		Parent->BuildRelativePathName (delimiter, name);
}


Boolean RGDocument :: HasName (const char* name) const {

	return (Name == name);
}


size_t RGDocument :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += Name.StoreSize ();
	return size;
}


int RGDocument :: CompareTo (const RGPersistent* target) const {

	RGDocument* temp = (RGDocument*) target;
	return Name.CompareTo (&(temp->Name));
}


unsigned RGDocument :: HashNumber (unsigned long Base) const {

	return Name.HashNumber (Base);
}


Boolean RGDocument :: IsEqualTo (const RGPersistent* target) const {

	RGDocument* temp = (RGDocument*) target;
	return Name.IsEqualTo (&(temp->Name));
}

void RGDocument :: RestoreAll (RGFile& f) {

	RGPersistent::RestoreAll (f);
	Name.RestoreAll (f);
}


void RGDocument :: RestoreAll (RGVInStream& vin) {

	RGPersistent::RestoreAll (vin);
	Name.RestoreAll (vin);
}


void RGDocument :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	Name.SaveAll (f);
}


void RGDocument :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	Name.SaveAll (vout);
}


void RGDocument :: WriteXML (RGFile& file, const RGString& tabLevel, const RGString& docLabel) {

	
	WriteXMLOpen (file, tabLevel, docLabel);
	WriteXMLData (file, tabLevel);
	WriteXMLClose (file, tabLevel, docLabel);
}


void RGDocument :: WriteXMLOpen (RGFile& file, const RGString& tabLevel, const RGString& docLabel) {

	RGString Line (tabLevel + "<" + docLabel + ">");
	Line.WriteTextLine (file);
}


void RGDocument :: WriteXMLData (RGFile& file, const RGString& tabLevel) {

	RGString Line (tabLevel + "\t<Name>" + Name + "</Name>");
	Line.WriteTextLine (file);
}


void RGDocument :: WriteXMLClose (RGFile& file, const RGString& tabLevel, const RGString& docLabel) {

	RGString Line (tabLevel + "</" + docLabel + ">");
	Line.WriteTextLine (file);
}


RGTree :: RGTree () : RGPersistent (), Parent (NULL) {

	DataIterator = new RGDListIterator (DataList);
	SubtreeIterator = new RGDListIterator (Subtrees);
}


RGTree :: RGTree (RGTree* parent) : RGPersistent (), Parent (parent) {

	DataIterator = new RGDListIterator (DataList);
	SubtreeIterator = new RGDListIterator (Subtrees);
}


RGTree :: RGTree (RGTree* parent, const RGString& name) : RGPersistent (), Parent (parent), Name (name) {

	DataIterator = new RGDListIterator (DataList);
	SubtreeIterator = new RGDListIterator (Subtrees);

	if (Parent == NULL)
		Name.FindAndReplaceAllSubstrings ("\\", "/");
}


RGTree :: RGTree (const RGString& name) : RGPersistent (), Parent (NULL), Name (name) {

	DataIterator = new RGDListIterator (DataList);
	SubtreeIterator = new RGDListIterator (Subtrees);
	Name.FindAndReplaceAllSubstrings ("\\", "/");
}


RGTree :: RGTree (const RGTree& tree) : RGPersistent (tree), Subtrees (tree.Subtrees), Parent (tree.Parent),DataList (tree.DataList),  Name (tree.Name) {

	DataIterator = new RGDListIterator (DataList);
	SubtreeIterator = new RGDListIterator (Subtrees);

	if (Parent == NULL)
		Name.FindAndReplaceAllSubstrings ("\\", "/");
}


RGTree :: ~RGTree () {

	delete DataIterator;
	delete SubtreeIterator;
	DataList.ClearAndDelete ();
	Subtrees.ClearAndDelete ();
}


void RGTree :: RecursivelySetParent (RGTree* parent) {

	RGTree* NextTree;
	SubtreeIterator->Reset ();
	Parent = parent;

	while (NextTree = (RGTree*)(*SubtreeIterator)()) {

		NextTree->RecursivelySetParent (this);
	}
}


void RGTree :: BuildFullPathName (const char* delimiter, RGString& name) {

	if (Parent == NULL)
		name = Name;

	else {

		Parent->BuildFullPathName (delimiter, name);
		name += delimiter + Name;
	}
}

void RGTree :: BuildRelativePathName (const char* delimiter, RGString& name) {

	if (Parent == NULL)
		name = "";

	else {

		if (Parent->isRoot ())
			name = Name;

		else {

			Parent->BuildRelativePathName (delimiter, name);
			name += delimiter + Name;
		}
	}
}


RGTree* RGTree :: AddSubtree (RGTree* NewTree) {

	if (Subtrees.Contains (NewTree))
		return NULL;
	
	Subtrees.Append (NewTree);
	NewTree->SetParent (this);
	return NewTree;
}


RGDocument* RGTree :: AddDocument (RGDocument* NewDocument) {

	RGDocument::SetCompareOnName (TRUE);

	if (DataList.Contains (NewDocument))
		return NULL;
	
	DataList.Append (NewDocument);
	NewDocument->SetParent (this);
	return NewDocument;
}


RGTree* RGTree :: InsertSubtree (RGTree* NewTree) {

	if (Subtrees.Contains (NewTree))
		return NULL;
	
	Subtrees.Insert (NewTree);
	return NewTree;
}


RGDocument* RGTree :: InsertDocument (RGDocument* NewDocument) {

	RGDocument::SetCompareOnName (TRUE);

	if (DataList.Contains (NewDocument))
		return NULL;

	DataList.Insert (NewDocument);
	return NewDocument;
}


RGTree* RGTree :: RemoveSubtree (RGTree* Target) {

	RGTree* Temp = (RGTree*) Subtrees.Remove (Target);
	return Temp;
}


RGTree* RGTree :: RemoveSubtree (const RGString& name) {

	SubtreeIterator->Reset ();
	RGTree* NextTree;

	while (NextTree = (RGTree*) (*SubtreeIterator)()) {

		if (name.CompareTo (NextTree->GetName ()) == 0) {

			NextTree = (RGTree*)SubtreeIterator->RemoveCurrentItem ();
			return NextTree;
		}
	}

	return NULL;
}


RGDocument* RGTree :: RemoveDocument (RGDocument* Target) {

	RGDocument::SetCompareOnName (TRUE);
	RGDocument* Temp = (RGDocument*) DataList.Remove (Target);
	return Temp;
}

void RGTree :: ResetSubtreeList () {

	SubtreeIterator->Reset ();
}


void RGTree :: ClearAndDeleteData () {

	DataList.ClearAndDelete ();
	DataIterator->Reset ();
}

void RGTree :: ClearAndDeleteAllData () {

	DataList.ClearAndDelete ();
	DataIterator->Reset ();
	SubtreeIterator->Reset ();
	RGTree* NextSubtree;

	while (NextSubtree = (RGTree*)(*SubtreeIterator)())
		NextSubtree->ClearAndDeleteAllData ();
}


void RGTree :: ClearAndDeleteAll () {

	ClearAndDeleteData ();

	SubtreeIterator->Reset ();
	RGTree* NextSubtree;

	while (NextSubtree = (RGTree*)(*SubtreeIterator)())
		NextSubtree->ClearAndDeleteAll ();

	Subtrees.ClearAndDelete ();
	SubtreeIterator->Reset ();
}


RGTree* RGTree :: CopyTreeAndSubtreesRecursively (const RGString& newName) {

	RGTree* root = new RGTree (newName);
	RGDListIterator it (Subtrees);
	RGTree* subtree;
	RGTree* newSubtree;

	while (subtree = (RGTree*) it()) {

		newSubtree = subtree->CopyTreeAndSubtreesRecursively (root);
		root->AddSubtree (newSubtree);
	}

	return root;
}


RGTree* RGTree :: CopyTreeAndSubtreesRecursively (RGTree* parent) {

	RGTree* root = new RGTree (parent);
	root->SetName (Name);
	RGDListIterator it (Subtrees);
	RGTree* subtree;
	RGTree* newSubtree;

	while (subtree = (RGTree*) it()) {

		newSubtree = subtree->CopyTreeAndSubtreesRecursively (root);
		root->AddSubtree (newSubtree);
	}

	return root;
}


int RGTree :: BuildTree () {

	RGDirectory* direct;
	RGString FileName;
	RGString TempName;
	RGString FullPathName;
	RGTree* NewTree;
	RGString Dot (".");
	RGString DDot ("..");
	RGDocument* DocumentType = new RGDocument;
	RGDocument* NewDoc;

	if (RGTree::Notify)
		NotifyWorking ();

	char Last = Name [Name.Length () - 1];

	if ((Last == '/') || (Last == '\\')) {

		TempName = Name.ExtractSubstring (0, Name.Length () - 2);
		Name = TempName;
	}

	BuildFullPathName ("/", FullPathName);
	direct = new RGDirectory (FullPathName);    // opendir (FullPathName.GetData ());

	if (!direct->IsValid ()) {

		cout << "     Could not open directory " << FullPathName.GetData () << endl << endl;
		delete direct;
		delete DocumentType;
		return -1;
	}

	else {
		
		while (direct->ReadNextDirectory (FileName)) {

//			cout << "Found file/directory with name " << FileName << endl;

			if ((FileName == Dot) || (FileName == DDot)) {

			}

			else if (DiskObjectIsDirectory (FileName, FullPathName, direct)) {

				NewTree = new RGTree (this, FileName);
				AddSubtree (NewTree);
			}

			else {

				NewDoc = (RGDocument*)DocumentType->GetNew ();
				NewDoc->SetParent (this);
				NewDoc->SetName (FileName);
				AddDocument (NewDoc);
			}
		}

		delete direct;
	}

	delete DocumentType;
	ResetSubtreeList ();

	while (NewTree = NextSubtree ()) {

		NewTree->BuildTree ();
	}

	return 0;
}


int RGTree :: BuildTreeFromDirectories (const RGString& FileSuffix, const RGDocument* DocumentType) {

	RGDirectory* direct;
	RGString FileName;
	RGString TempName;
	RGString FullPathName;
	RGTree* NewTree;
	RGDocument* NewDoc;
	RGString Dot (".");
	RGString DDot ("..");

	if (RGTree::Notify)
		NotifyWorking ();

	char Last = Name [Name.Length () - 1];

	if ((Last == '/') || (Last == '\\')) {

		TempName = Name.ExtractSubstring (0, Name.Length () - 2);
		Name = TempName;
	}

	BuildFullPathName ("/", FullPathName);
	direct = new RGDirectory (FullPathName);    // opendir (FullPathName.GetData ());

	if (!direct->IsValid ()) {

		cout << "     Could not open directory " << FullPathName.GetData () << endl << endl;
		delete direct;
		return -1;
	}

	else {
		
		while (direct->ReadNextDirectory (FileName)) {

//			cout << "Found file/directory with name " << FileName << endl;

			if ((FileName == Dot) || (FileName == DDot)) {

			}
			
			else if (DiskObjectIsDirectory (FileName, FullPathName, direct)) {

				NewTree = new RGTree (this, FileName);
				AddSubtree (NewTree);
			}

			else if (DiskObjectIsFileWithSuffix (FileSuffix, FileName)) {

				NewDoc = (RGDocument*)DocumentType->GetNew ();
				NewDoc->SetParent (this);
				NewDoc->SetName (FileName);
				AddDocument (NewDoc);
			}
		}

		delete direct;
	}

	ResetSubtreeList ();

	while (NewTree = NextSubtree ()) {

		NewTree->BuildTreeFromDirectories (FileSuffix, DocumentType);
	}

	return 0;
}


int RGTree :: BuildTreeFromDirectoriesAndSuffixList (RGSuffixList& SuffixList, const RGDocument* DocumentType) {

	RGDirectory* direct;
	RGString FileName;
	RGString TempName;
	RGString FullPathName;
	RGTree* NewTree;
	RGDocument* NewDoc;
	RGString Dot (".");
	RGString DDot ("..");

	if (RGTree::Notify)
		NotifyWorking ();

	char Last = Name [Name.Length () - 1];

	if ((Last == '/') || (Last == '\\')) {

		TempName = Name.ExtractSubstring (0, Name.Length () - 2);
		Name = TempName;
	}

	BuildFullPathName ("/", FullPathName);

	direct = new RGDirectory (FullPathName);    // opendir (FullPathName.GetData ());

	if (!direct->IsValid ()) {

		cout << "     Could not open directory " << FullPathName.GetData () << endl << endl;
		delete direct;
		return -1;
	}

	else {
		
		while (direct->ReadNextDirectory (FileName)) {

//			cout << "Found file/directory with name " << FileName << endl;

			if ((FileName == Dot) || (FileName == DDot)) {

			}
			
			else if (DiskObjectIsDirectory (FileName, FullPathName, direct)) {

				NewTree = new RGTree (this, FileName);
				AddSubtree (NewTree);
			}

			else if (DiskObjectIsFileWithListedSuffix (SuffixList, FileName)) {

				NewDoc = (RGDocument*)DocumentType->GetNew ();
				NewDoc->SetParent (this);
				NewDoc->SetName (FileName);
				AddDocument (NewDoc);
			}
		}

		delete direct;
	}

	ResetSubtreeList ();

	while (NewTree = NextSubtree ()) {

		NewTree->BuildTreeFromDirectoriesAndSuffixList (SuffixList, DocumentType);
	}

	return 0;
}


Boolean RGTree :: PruneTreesWithNoData () {

	// returns TRUE if has data or some subtree has data
	
	RGTree* Target;
	RGTree* NextTree;
	RGDList TemporaryList;
	RGDListIterator it (TemporaryList);
	Boolean SomeTreeHasData = FALSE;
	ResetSubtreeList ();

	while (Target = NextSubtree ()) {

		if (!Target->PruneTreesWithNoData ()) {

			TemporaryList.Append (Target);
		}
	}

	it.Reset ();

	while (Target = (RGTree*) it ()) {

		NextTree = RemoveSubtree (Target);
//		cout << "Removing tree named " << NextTree->GetName () << endl;
	}

	TemporaryList.ClearAndDelete ();

	if (Subtrees.Entries () > 0)
		return TRUE;

	if (DataList.Entries () > 0)
		return TRUE;

	return FALSE;
}


Boolean RGTree :: ClearDirectoryTree () {

	ResetAllLists ();
	RGDocument* NextDoc;
	RGTree* NextTree;
	RGString FullTestName;

	while (NextDoc = NextDeepDocument ()) {

		NextDoc->BuildFullPathName ("/", FullTestName);
#ifdef _WINDOWS
		DeleteFile (FullTestName);
#else
    unlink(FullTestName);
#endif
	}

	ResetAllLists ();

	while (NextTree = NextDeepSubtree ()) {

		NextTree->BuildFullPathName ("/", FullTestName);

		if (NextTree != this)
    {
#ifdef _WINDOWS
			RemoveDirectory (FullTestName);
#else
      rmdir(FullTestName);
#endif
    }
	}

	return TRUE;
	
	/*ResetSubtreeList ();
	RGTree* NextTree;
	RGString FullPath;
	BuildFullPathName ("/", FullPath);
	RGString FullPathSlash = FullPath + "/";
	RGString DirectoryName;
	RGString DirectoryPath;
	RGString FileName;
	RGString Dot (".");
	RGString DDot ("..");
	Boolean ReturnValue = TRUE;

	while (NextTree = NextSubtree ()) {

		NextTree->ClearDirectoryTree ();
		DirectoryName = NextTree->GetName ();
		DirectoryPath = FullPathSlash + DirectoryName;
		RemoveDirectory (DirectoryPath);
	}

	RGDirectory* direct = new RGDirectory (FullPath);    // opendir (FullPath.GetData ());

	if (direct->IsValid ()) {
		
		while (direct->ReadNextDirectory (FileName)) {

			//
			// we have deleted all the directories; what is left must be a file
			//

			if ((FileName == Dot) || (FileName == DDot)) {

			}

			else if (DiskObjectIsDirectory (FileName, FullPath, direct)) {

				DirectoryPath = FullPathSlash + FileName;
				RemoveDirectory (DirectoryPath);
			}

			else {

				DirectoryPath = FullPathSlash + FileName;
				DeleteFile (DirectoryPath);
			}
		}		
	}

	else
		ReturnValue = FALSE;

	delete direct;
	return ReturnValue;*/
}


Boolean RGTree :: CreateDirectoriesRecursivelyFromSubtrees () {

	RGString FullPath;
	BuildFullPathName ("/", FullPath);
	cout << "Attempting to create directory " << FullPath << endl;

	if (!RGDirectory::MakeDirectory (FullPath))
		return FALSE;

	ResetSubtreeList ();
	RGTree* NextLocalSubtree;

	while (NextLocalSubtree = NextSubtree ()) {

		if (!NextLocalSubtree->CreateDirectoriesRecursivelyFromSubtrees ())
			return FALSE;
	}

	return TRUE;
}


size_t RGTree :: GetLevel () const {

	if (Parent == NULL)
		return 0;

	return Parent->GetLevel () + 1;
}


Boolean RGTree :: HasName (const char* name) const {

	return (Name == name);
}


RGTree* RGTree :: FindTreeFromTreeName (const RGString& name) {

	RGTree* Next;
	RGTree* Found;

	if (Name == name)
		return this;

	SubtreeIterator->Reset ();

	while (Next = NextSubtree()) {

		Found = Next->FindTreeFromTreeName (name);

		if (Found != NULL)
			return Found;
	}

	return NULL;
}


RGDocument* RGTree :: FindDocumentFromTreeAndDocumentName (const RGString& treeName, const RGString& docName) {

	RGTree* Next;
	RGTree* Found;
	RGDocument* NextDoc;

	if (Name == treeName) {

		// search this document data list

		ResetDocumentList ();

		while (NextDoc = NextDocument ()) {

			if (NextDoc->HasName (docName))
				return NextDoc;
		}

		return NULL;
	}

	SubtreeIterator->Reset ();

	while (Next = NextSubtree()) {

		Found = Next->FindTreeFromTreeName (treeName);

		if (Found != NULL) {

			// search Found data list
			Found->ResetDocumentList ();
			
			while (NextDoc = Found->NextDocument ()) {

				if (NextDoc->HasName (docName))
					return NextDoc;
			}

			return NULL;
		}
	}
	
	return NULL;
}


RGDocument* RGTree :: FindDocumentFromDocumentName (const RGString& name) {

	RGTree* Next;
	RGDocument* Found;

	SubtreeIterator->Reset ();
	DataIterator->Reset ();

	while (Found = NextDocument()) {

		if (Found->HasName (name))
			return Found;
	}

	while (Next = NextSubtree()) {

		Found = Next->FindDocumentFromDocumentName (name);

		if (Found != NULL)
			return Found;
	}
	
	return NULL;
}


RGDocument* RGTree :: FindDocumentFromFullPath (const RGString& fullpath) {

	RGString fullPath (fullpath);
	fullPath.FindAndReplaceAllSubstrings ("\\", "/");
	size_t startPosition;
	size_t endPosition;
	size_t position;
	RGString fileName;
	RGString Path;

	if (!fullPath.FindLastSubstring ("/", startPosition, endPosition))
		return FindDocumentFromReducedPathAndDocumentName (Path, fullPath);

	size_t L = fullPath.Length ();

	if (endPosition < L - 1) {
	
		fileName = fullPath.ExtractSubstring (endPosition + 1, L - 1);

		if (startPosition > 0)
			Path = fullPath.ExtractSubstring (0, startPosition - 1);

		else
			Path = "";
	}

	else
		return NULL;

	RGString BaseFullPath;
	BuildFullPathName ("/", BaseFullPath);
	position = 0;
	Path.FindAndReplaceNextSubstring (BaseFullPath, "", position);
	position = 0;

	if ((Path.Length () > 0) && (Path [0] == '/'))
		Path.FindAndReplaceNextSubstring ("/", "", position);

	return FindDocumentFromReducedPathAndDocumentName (Path, fileName);
}



RGDocument* RGTree :: FindDocumentFromPathAndDocumentName (const RGString& path, const RGString& name) {

	RGString BaseFullPath;
	BuildFullPathName ("/", BaseFullPath);
	RGString Path (path);
	Path.FindAndReplaceAllSubstrings ("\\", "/");
	size_t position = 0;
	Path.FindAndReplaceNextSubstring (BaseFullPath, "", position);
	position = 0;

	if ((Path.Length () > 0) && (Path [0] == '/'))
		Path.FindAndReplaceNextSubstring ("/", "", position);

	return FindDocumentFromReducedPathAndDocumentName (Path, name);
}


RGDocument* RGTree :: FindDocumentFromRelativePathAndDocumentName (const RGString& path, const RGString& name) {

	RGString Path (path);
	Path.FindAndReplaceAllSubstrings ("\\", "/");
	size_t position = 0;

	if ((Path.Length () > 0) && (Path [0] == '/'))
		Path.FindAndReplaceNextSubstring ("/", "", position);

	return FindDocumentFromReducedPathAndDocumentName (Path, name);
}


RGDocument* RGTree :: FindDocumentFromReducedPathAndDocumentName (const RGString& path, const RGString& name) {

	RGDocument* NextDoc;
	RGTree* NextTree;
	
	if (path.Length () == 0) {

		//
		// This is the directory...look for the document here
		//

		ResetDocumentList ();

		while (NextDoc = NextDocument ()) {

			if (NextDoc->HasName (name))
				return NextDoc;
		}

		return NULL;
	}

	//
	// Remove next directory name and look for directory
	//

	RGString Path (path);
	RGString NextTreeName;
	size_t position;

	if (Path.FindNextSubstring (0, "/", position)) {

		NextTreeName = Path.ExtractSubstring (0, position - 1);
		position = 0;
		Path.FindAndReplaceNextSubstring (NextTreeName, "", position);
		position = 0;

		if ((Path.Length () > 0) && (Path [0] == '/'))
			Path.FindAndReplaceNextSubstring ("/", "", position);
	}

	else {
		
		NextTreeName = Path;
		Path = "";
	}

	ResetSubtreeList ();

	while (NextTree = NextSubtree()) {

		if (NextTree->HasName (NextTreeName))
			return NextTree->FindDocumentFromReducedPathAndDocumentName (Path, name);
	}

	return NULL;
}


RGDocument* RGTree :: AddDocumentFromPath (const RGString& path, RGDocument* NewDocument, Boolean overWrite) {

	RGString Path (path);
	Path.FindAndReplaceAllSubstrings ("\\", "/");
	size_t position = 0;

	if ((Path.Length () > 0) && (Path [0] == '/'))
		Path.FindAndReplaceNextSubstring ("/", "", position);

	return AddDocumentFromReducedPath (Path, NewDocument, overWrite);
}


RGDocument* RGTree :: AddDocumentFromRelativePath (const RGString& path, RGDocument* NewDocument, Boolean overWrite) {

	RGString BaseFullPath;
	BuildFullPathName ("/", BaseFullPath);
	RGString Path (path);
	Path.FindAndReplaceAllSubstrings ("\\", "/");
	size_t position = 0;
	Path.FindAndReplaceNextSubstring (BaseFullPath, "", position);
	position = 0;

	if ((Path.Length () > 0) && (Path [0] == '/'))
		Path.FindAndReplaceNextSubstring ("/", "", position);

	return AddDocumentFromReducedPath (Path, NewDocument, overWrite);
}


RGDocument* RGTree :: AddDocumentFromReducedPath (const RGString& path, RGDocument* NewDocument, Boolean overWrite) {

	RGDocument* NextDoc;
	RGTree* NextTree;
	
	if (path.Length () == 0) {

		//
		// This is the directory...add the document here
		//

		if (overWrite) {

			if (DataList.Contains (NewDocument)) {

				NextDoc = (RGDocument*)DataList.Remove (NewDocument);
				delete NextDoc;
			}
		}
		
		return AddDocument (NewDocument);
	}

	//
	// Remove next directory name and look for directory
	//

	RGString Path (path);
	RGString NextTreeName;
	size_t position;

	if (Path.FindNextSubstring (0, "/", position)) {

		NextTreeName = Path.ExtractSubstring (0, position - 1);
		position = 0;
		Path.FindAndReplaceNextSubstring (NextTreeName, "", position);
		position = 0;

		if ((Path.Length () > 0) && (Path [0] == '/'))
			Path.FindAndReplaceNextSubstring ("/", "", position);
	}

	else {
		
		NextTreeName = Path;
		Path = "";
	}

	ResetSubtreeList ();

	while (NextTree = NextSubtree()) {

		if (NextTree->HasName (NextTreeName))
			return NextTree->AddDocumentFromReducedPath (Path, NewDocument, overWrite);
	}

	//
	// Add a new subtree
	//

	NextTree = new RGTree (this, NextTreeName);
	Subtrees.Append (NextTree);
	return NextTree->AddDocumentFromReducedPath (Path, NewDocument, overWrite);
}


void RGTree :: ResetAllSubtreeLists () {

	SubtreeIterator->Reset ();
	RGTree* NextTree;

	while (NextTree = (RGTree*)(*SubtreeIterator)()) {

		NextTree->ResetAllSubtreeLists ();
	}

	SubtreeIterator->Reset ();
	BeforeFirstSubtree = TRUE;
	InDeepMode = TRUE;
	SentTreetop = FALSE;
	SentSelf = FALSE;
}


void RGTree :: ResetDocumentList () {

	DataIterator->Reset ();
}


void RGTree :: ResetAllDocumentLists () {

	DataIterator->Reset ();
	SubtreeIterator->Reset ();
	RGTree* NextTree;

	while (NextTree = (RGTree*)(*SubtreeIterator)()) {

		NextTree->ResetAllDocumentLists ();
	}

	SubtreeIterator->Reset ();
	BeforeFirstSubtree = TRUE;
	InDeepMode = TRUE;
	SentTreetop = FALSE;
	SentSelf = FALSE;
}


void RGTree :: ResetAllLists () {

	DataIterator->Reset ();
	SubtreeIterator->Reset ();
	RGTree* NextTree;

	while (NextTree = (RGTree*)(*SubtreeIterator)()) {

		NextTree->ResetAllLists ();
	}

	SubtreeIterator->Reset ();
	BeforeFirstSubtree = TRUE;
	InDeepMode = TRUE;
	SentTreetop = FALSE;
	SentSelf = FALSE;
}



RGTree* RGTree :: NextSubtree () {

	RGTree* NextTree = (RGTree*) (*SubtreeIterator)();
	return NextTree;
}


RGDocument* RGTree :: NextDocument () {

	RGDocument* NextDoc = (RGDocument*) (*DataIterator)();
	return NextDoc;
}


RGTree* RGTree :: TreetopTest (RGTree* target) {

	if (target)
		return target;

	if (!SentTreetop && (Parent == NULL)) {

		SentTreetop = TRUE;
		return this;
	}

	return NULL;
}


RGTree* RGTree :: NextDeepSubtree () {

	RGTree* CurrentTree;
	RGTree* DeepTree;
	
	if (InDeepMode) {

		if (BeforeFirstSubtree) {

			CurrentTree = (RGTree*)(*SubtreeIterator)();
			BeforeFirstSubtree = FALSE;

			if (CurrentTree == NULL) {

				//
				//  there are no subtrees
				//

				InDeepMode = FALSE;
				SubtreeIterator->Reset ();
				return TreetopTest (NULL);
			}
		}

		else
			CurrentTree = (RGTree*) SubtreeIterator->CurrentItem ();

		// CurrentTree should never be NULL at this point

		DeepTree = CurrentTree->NextDeepSubtree ();

		if (DeepTree)
			return DeepTree;

		while (DeepTree == NULL) {

			CurrentTree = (RGTree*)(*SubtreeIterator)();

			if (CurrentTree) {

				DeepTree = CurrentTree->NextDeepSubtree ();

				if (DeepTree)
					return DeepTree;
			}

			else {

				InDeepMode = FALSE;
				SubtreeIterator->Reset ();
				CurrentTree = (RGTree*)(*SubtreeIterator)();
				return TreetopTest (CurrentTree);

/*				if (CurrentTree)
					return CurrentTree;

				return NULL;  */
			}
		}
	}

	else {   // not in deep mode

		CurrentTree = (RGTree*)(*SubtreeIterator)();

		if (CurrentTree)
			return CurrentTree;
	}

	return TreetopTest (NULL);
}


RGDocument* RGTree :: NextDeepDocument () {

	RGTree* CurrentTree;
	RGDocument* DeepDoc;
	RGDocument* CurrentDoc;
	
	if (InDeepMode) {

		if (BeforeFirstSubtree) {

			CurrentTree = (RGTree*)(*SubtreeIterator)();
			BeforeFirstSubtree = FALSE;

			if (CurrentTree == NULL) {

				//
				//  there are no subtrees
				//

				InDeepMode = FALSE;
				SubtreeIterator->Reset ();
				CurrentDoc = (RGDocument*)(*DataIterator)();
				return CurrentDoc;
			}
		}

		else
			CurrentTree = (RGTree*) SubtreeIterator->CurrentItem ();

		// CurrentTree should never be NULL at this point

		DeepDoc = CurrentTree->NextDeepDocument ();

		if (DeepDoc)
			return DeepDoc;

		while (DeepDoc == NULL) {

			CurrentTree = (RGTree*)(*SubtreeIterator)();

			if (CurrentTree) {

				DeepDoc = CurrentTree->NextDeepDocument ();

				if (DeepDoc)
					return DeepDoc;
			}

			else {

				InDeepMode = FALSE;
				DataIterator->Reset ();
				CurrentDoc = (RGDocument*)(*DataIterator)();
				return CurrentDoc;  // whether NULL or not
			}
		}
	}

	else {   // not in deep mode

		CurrentDoc = (RGDocument*)(*DataIterator)();
		return CurrentDoc;  // whether NULL or not
	}

	return NULL;
}


RGTree* RGTree :: NextTopDown () {

	RGTree* NextTree;
	
	if (!SentSelf) {

		SentSelf = TRUE;
		SubtreeIterator->Reset ();
		SendingTree = NULL;
		return this;
	}

	if (SendingTree == NULL) {

		SendingTree = (RGTree*)(*SubtreeIterator)();

		if (SendingTree == NULL)
			return NULL;
	}

	NextTree = SendingTree->NextTopDown ();

	if (NextTree == NULL) {

		SendingTree = (RGTree*)(*SubtreeIterator)();

		if (SendingTree == NULL)
			return NULL;

		NextTree = SendingTree->NextTopDown ();
	}

	return NextTree;
}


size_t RGTree :: StoreSize () const {

	size_t size = RGPersistent::StoreSize ();
	size += Name.StoreSize ();
	size += Subtrees.StoreSize () + DataList.StoreSize ();
	return size;
}


int RGTree :: CompareTo (const RGPersistent* target) const {

	RGTree* Temp = (RGTree*) target;
	return Name.CompareTo (&Temp->Name);
}


unsigned RGTree :: HashNumber (unsigned long Base) const {

	return Name.HashNumber (Base);
}


Boolean RGTree :: IsEqualTo (const RGPersistent* target) const {

	RGTree* Temp = (RGTree*) target;
	return Name.IsEqualTo (&Temp->Name);
}


void RGTree :: RestoreAll (RGFile& f) {

	RGTree* NewTree;
	RGDocument* NextDoc;
	
	RGPersistent::RestoreAll (f);
	Subtrees.RestoreAll (f);
	DataList.RestoreAll (f);
	Name.RestoreAll (f);

	DataIterator->Reset ();
	SubtreeIterator->Reset ();

	while (NewTree = (RGTree*)(*SubtreeIterator)())
		NewTree->SetParent (this);

	while (NextDoc = (RGDocument*)(*DataIterator)())
		NextDoc->SetParent (this);
}


void RGTree :: RestoreAll (RGVInStream& vin) {

	RGTree* NewTree;
	RGDocument* NextDoc;
	
	RGPersistent::RestoreAll (vin);
	Subtrees.RestoreAll (vin);
	DataList.RestoreAll (vin);
	Name.RestoreAll (vin);

	DataIterator->Reset ();
	SubtreeIterator->Reset ();

	while (NewTree = (RGTree*)(*SubtreeIterator)())
		NewTree->SetParent (this);

	while (NextDoc = (RGDocument*)(*DataIterator)())
		NextDoc->SetParent (this);
}


void RGTree :: SaveAll (RGFile& f) const {

	RGPersistent::SaveAll (f);
	Subtrees.SaveAll (f);
	DataList.SaveAll (f);
	Name.SaveAll (f);
}


void RGTree :: SaveAll (RGVOutStream& vout) const {

	RGPersistent::SaveAll (vout);
	Subtrees.SaveAll (vout);
	DataList.SaveAll (vout);
	Name.SaveAll (vout);
}


void RGTree :: WriteXML (RGFile& file, const RGString& tabLevel, const RGString& dirLabel, const RGString& subdirLabel, const RGString& docLabel) {

	//
	// use three calls...open, data and close
	//

	WriteXMLOpen (file, tabLevel, dirLabel);
	WriteXMLData (file, tabLevel, subdirLabel, docLabel);
	WriteXMLClose (file, tabLevel, dirLabel);
}

void RGTree :: WriteXMLOpen (RGFile& file, const RGString& tabLevel, const RGString& dirLabel) {

	RGString Line (tabLevel + "<" + dirLabel + ">");
	Line.WriteTextLine (file);
}


void RGTree :: WriteXMLData (RGFile& file, const RGString& tabLevel, const RGString& subdirLabel, const RGString& docLabel) {

	RGTree* NextTree;
	RGDocument* NextDoc;
	RGString TabLevel1 = tabLevel + "\t";
	RGString Line (TabLevel1 + "<Name>" + Name + "</Name>");
	Line.WriteTextLine (file);
	ResetSubtreeList ();

	while (NextTree = NextSubtree ())
		NextTree->WriteXML (file, TabLevel1, subdirLabel, subdirLabel, docLabel);

	ResetDocumentList ();

	while (NextDoc = NextDocument ())
		NextDoc->WriteXML (file, TabLevel1, docLabel);
}


void RGTree :: WriteXMLClose (RGFile& file, const RGString& tabLevel, const RGString& dirLabel) {

	RGString Line (tabLevel + "</" + dirLabel + ">");
	Line.WriteTextLine (file);
}


Boolean RGTree :: DiskObjectIsDirectory (const RGString& FileName, const RGString& DirectoryName, RGDirectory* dir) {

	// returns TRUE for directory

	RGString FullPathName = DirectoryName + "/" + FileName;
	return dir->FileIsDirectory (FullPathName);
}


Boolean RGTree :: DiskObjectIsFileWithSuffix (const RGString& FileSuffix, const RGString& FileName) {

	// returns TRUE for file with suffix FileSuffix

	size_t startPosition;
	size_t endPosition;
	RGString Extension;
	size_t L = FileName.Length ();

	if (FileName.FindLastSubstring (".", startPosition, endPosition)) {

		if (endPosition >= L - 1)
			return FALSE;

		Extension = FileName.ExtractSubstring (endPosition + 1, L - 1);

		if (Extension == FileSuffix)
			return TRUE;
	}

	return FALSE;
}


Boolean RGTree :: DiskObjectIsFileWithListedSuffix (RGSuffixList& SuffixList, const RGString& FileName) {

	// returns TRUE for file with any suffix listed in SuffixList

	size_t startPosition;
	size_t endPosition;
	RGString Extension;
	size_t L = FileName.Length ();
	SuffixList.ResetList ();
	RGString* NextSuffix;

	if (FileName.FindLastSubstring (".", startPosition, endPosition)) {

		if (endPosition >= L - 1)
			return FALSE;

		Extension = FileName.ExtractSubstring (endPosition + 1, L - 1);

		while (NextSuffix = SuffixList.NextSuffix ()) {

			if (Extension == *NextSuffix)
				return TRUE;
		}	
	}

	return FALSE;
}


void RGTree :: NotifyWorking () {

	if (RGTree::CurrentRepetition >= RGTree::Frequency) {

		cout << "Working..." << endl;
		RGTree::CurrentRepetition = 0;
	}

	else
		RGTree::CurrentRepetition++;
	
}


void RGTree :: ResetNotifyFrequency (int freq) {
	
	if (freq > 0)
		Frequency = freq;
	
	else
		Frequency = 50;
}


RGTreeIterator :: RGTreeIterator (RGTree& tree) : Root (tree), CurrentLevel (0), CurrentSubtree (NULL) {

	CurrentList = new RGDList;
	SecondaryList = new RGDList;
	CurrentIterator = new RGDListIterator (*CurrentList);
	SecondaryIterator = new RGDListIterator (*SecondaryIterator);
}


RGTreeIterator :: ~RGTreeIterator () {

	delete CurrentIterator;
	delete SecondaryIterator;
	CurrentList->Clear ();
	SecondaryList->Clear ();
	delete CurrentList;
	delete SecondaryList;
}


void RGTreeIterator :: Reset () {

	RGTree* root = &Root;
	CurrentList->Clear ();
	SecondaryList->Clear ();
	CurrentList->Append (root);
	CurrentLevel = 0;
	CurrentSubtree = NULL;
	CurrentIterator->Reset ();
	SecondaryIterator->Reset ();
	Root.ResetAllLists ();
}


RGTree* RGTreeIterator :: NextSubtree (int& level) {

	RGTree* NextTree = (RGTree*) (*CurrentIterator)();
	RGTree* SecondaryTree;
	RGDList* TempList;
	RGDListIterator* TempIterator;

	if (NextTree != NULL) {

		CurrentSubtree = NextTree;
		level = CurrentLevel;
		return NextTree;
	}

	CurrentIterator->Reset ();
	SecondaryList->Clear ();

	while (NextTree = (RGTree*)(*CurrentIterator)()) {

		while (SecondaryTree = NextTree->NextSubtree ()) {

			SecondaryList->Append (SecondaryTree);
		}
	}

	if (SecondaryList->Entries () == 0)
		return NULL;

	CurrentLevel++;
	level = CurrentLevel;

	TempList = CurrentList;
	CurrentList = SecondaryList;
	SecondaryList = TempList;

	TempIterator = CurrentIterator;
	CurrentIterator = SecondaryIterator;
	SecondaryIterator = TempIterator;

	CurrentIterator->Reset ();
	CurrentSubtree = (RGTree*)(*CurrentIterator)();
	return CurrentSubtree;
}


RGTree* RGTreeIterator :: CurrentItem () {

	return CurrentSubtree;
}


RGSuffixList :: RGSuffixList () : SuffixIterator (SuffixList) {

}


RGSuffixList :: ~RGSuffixList () {

	SuffixList.ClearAndDelete ();
}


void RGSuffixList :: AddSuffix (const char* suffix) {

	RGString* NewSuffix = new RGString (suffix);
	
	if (!SuffixList.Contains (NewSuffix))
		SuffixList.Append (NewSuffix);

	else
		delete NewSuffix;
}


void RGSuffixList :: ResetList () {

	SuffixIterator.Reset ();
}


RGString* RGSuffixList :: NextSuffix () {

	return (RGString*)SuffixIterator();
}


void RGSuffixList :: ClearAndDelete () {

	SuffixList.ClearAndDelete ();
}
