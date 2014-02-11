//
// GenerateSmartNotices.cpp : This program uses prototype files to generate
// the .h and .cpp files for STRSmartNotices.
//

//#include "stdafx.h"
#include "rgfile.h"
#include "rgstring.h"
#include "rgtokenizer.h"
#include <iostream>
#include <tchar.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[]) {

	RGString FullPath ("C:/Users/goorrob/Documents/DEV/internal/osiris");
	RGString InputFileDirectory ("Documentation/SmartNoticePrototypes");
	RGString OutputFileDirectory ("OsirisLib2.01");

	RGString StartDotHName ("STRSmartNoticePrototype.h.begin.txt");
	RGString StartDotCPPName ("STRSmartNoticePrototype.cpp.begin.txt");
	RGString StaticName ("STRSmartNoticePrototypeStatic.txt");
	RGString DeclarationName ("STRSmartNoticePrototype.h.txt");
	RGString SourceName ("STRSmartNoticePrototype.cpp.txt");
	RGString PredeclarationName ("STRSmartNoticePrototypePersistentPredecl.txt");
	RGString PersistentDefinitionName ("STRSmartNoticePrototypePersistentDef.txt");
	
	RGString DotHName ("STRSmartNotices.h");
	RGString DotCPPName ("STRSmartNotices.cpp");

	RGString FullInputPath = FullPath + "/" + InputFileDirectory + "/";
	RGString FullOutputPath = FullPath + "/" + OutputFileDirectory + "/";

	RGFile StartDotH (FullInputPath + StartDotHName, "rt");
	RGFile StartDotCPP (FullInputPath + StartDotCPPName, "rt");
	RGFile Static (FullInputPath + StaticName, "rt");
	RGFile Declaration (FullInputPath + DeclarationName, "rt");
	RGFile Source (FullInputPath + SourceName, "rt");
	RGFile Predeclaration (FullInputPath + PredeclarationName, "rt");
	RGFile PersistentDefinition (FullInputPath + PersistentDefinitionName, "rt");

	RGFile DotH (FullOutputPath + DotHName, "wt");
	RGFile DotCPP (FullOutputPath + DotCPPName, "wt");

	if (!StartDotH.isValid ()) {

		cout << "Could not open 'startDotH'...ending" << endl;
		return -5;
	}

	if (!StartDotCPP.isValid ()) {

		cout << "Could not open 'startDotCPP'...ending" << endl;
		return -5;
	}

	if (!Static.isValid ()) {

		cout << "Could not open 'Static'...ending" << endl;
		return -5;
	}

	if (!Declaration.isValid ()) {

		cout << "Could not open 'Declaration'...ending" << endl;
		return -5;
	}

	if (!Source.isValid ()) {

		cout << "Could not open 'Source'...ending" << endl;
		return -5;
	}

	if (!Predeclaration.isValid ()) {

		cout << "Could not open 'Predeclaration'...ending" << endl;
		return -5;
	}

	if (!PersistentDefinition.isValid ()) {

		cout << "Could not open 'PersistentDefinition'...ending" << endl;
		return -5;
	}

	if (!DotH.isValid ()) {

		cout << "Could not open 'DotH'...ending" << endl;
		return -5;
	}

	if (!DotCPP.isValid ()) {

		cout << "Could not open 'DotCPP'...ending" << endl;
		return -5;
	}

	RGString StartDotHString;
	RGString StartDotCPPString;
	RGString StaticString;
	RGString DeclarationString;
	RGString SourceString;
	RGString PredeclarationString;
	RGString PersistentDefinitionString;

	StartDotHString.ReadTextFile (StartDotH);
	StartDotCPPString.ReadTextFile (StartDotCPP);
	StaticString.ReadTextFile (Static);
	DeclarationString.ReadTextFile (Declaration);
	SourceString.ReadTextFile (Source);
	PredeclarationString.ReadTextFile (Predeclaration);
	PersistentDefinitionString.ReadTextFile (PersistentDefinition);

	RGString Working1;
	RGString Working2;

	DotH.Write (StartDotHString.GetData ());
	DotCPP.Write (StartDotCPPString.GetData ());

	RGFile Names (FullPath + "/MessageBook/RawSmartNoticeInput.txt", "rt");

	if (!Names.isValid ()) {

		cout << "Could not open file of names...ending" << endl;
		return -5;
	}

	RGString NameLine;
	RGString* Name;
	RGString* Index;
	RGString* ID;

	RGDList NameList;
	RGDList IndexList;
	RGDList IDList;

	RGDListIterator NameIterator (NameList);
	RGDListIterator IndexIterator (IndexList);
	RGDListIterator IDIterator (IDList);
	
	RGStringTokenizer tokenizer (NameLine);
	RGStringArray tokens (5);
	RGStringArray delims (5);
	tokenizer.AddDelimiter (' ');
	tokenizer.AddDelimiter ('\t');
	tokenizer.AddDelimiter ("\t\t");

	while (true) {

		NameLine.ReadTextLine (Names);

		if (NameLine.Length () <= 2)
			break;

		tokenizer.ResetTokenizer ();
		tokenizer.Split (tokens, delims);
		Name = new RGString (tokens [0]);
		Index = new RGString (tokens [1]);
		ID = new RGString (tokens [2]);

		NameList.Append (Name);
		IndexList.Append (Index);
		IDList.Append (ID);
	}

	NameIterator.Reset ();
	IndexIterator.Reset ();
	IDIterator.Reset ();

	while (Name = (RGString*) NameIterator ()) {

		Index = (RGString*) IndexIterator ();
		ID = (RGString*) IDIterator ();

		cout << "Name = " << *Name << ", Index = " << *Index << ", ID = " << *ID << endl;
	}

	NameIterator.Reset ();
	IndexIterator.Reset ();
	IDIterator.Reset ();
	
	while (Name = (RGString*) NameIterator ()) {

		Working1 = PredeclarationString;
		Working1.FindAndReplaceAllSubstrings ("%ClassName%", *Name);
		DotH.Write (Working1);
	}

	NameIterator.Reset ();

	while (Name = (RGString*) NameIterator ()) {

		Working1 = DeclarationString;
		Working1.FindAndReplaceAllSubstrings ("%ClassName%", *Name);
		DotH.Write (Working1);
	}

	Working1 = "#endif /* _STRSMARTNOTICES_H_ */\n\n";
	DotH.Write (Working1);
	NameIterator.Reset ();

	while (Name = (RGString*) NameIterator ()) {

		Index = (RGString*) IndexIterator ();
		Working1 = StaticString;
		Working1.FindAndReplaceAllSubstrings ("%ClassName%", *Name);
		Working1.FindAndReplaceAllSubstrings ("%MessageNumber%", *Index);
		DotCPP.Write (Working1);
	}

	NameIterator.Reset ();
	IndexIterator.Reset ();

	while (Name = (RGString*) NameIterator ()) {

		ID = (RGString*) IDIterator ();
		Working1 = PersistentDefinitionString;
		Working1.FindAndReplaceAllSubstrings ("%ClassName%", *Name);
		Working1.FindAndReplaceAllSubstrings ("%ClassID%", *ID);
		Working1.FindAndReplaceAllSubstrings ("%ClassStringID%", *Name);
		DotCPP.Write (Working1);
	}

	Working1 = "\n\n\n";
	DotCPP.Write (Working1);
	NameIterator.Reset ();
	IDIterator.Reset ();

	while (Name = (RGString*) NameIterator ()) {

		Working1 = SourceString;
		Working1.FindAndReplaceAllSubstrings ("%ClassName%", *Name);
		DotCPP.Write (Working1);
	}
	
	return 0;
}

