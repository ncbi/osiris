
%{



#include <stdio.h>
#include <string.h>
//#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "rgdefs.h"
#include "rgstring.h"
#include "rgdlist.h"
#include "rgxmlschema.h"

using namespace std;

#define yyparse RGXMLSchemaSummary::YYPARSE

extern FILE *yyin, *yyout;

#define nl '\n'  



int yylex();
int yyerror(const char *s);

RGDList NameStack;
RGDList NumberStack;
RGDList ElementNameStack;
int ElementDepth;
int ListDepth;
int SequenceDepth;


char buffer[80]; // used for constant type strings

RGString ErrorMsg;
int status;
RGString* NewString;
RGString* NewElementName;
RGString* ParentElementName;
RGString* ReferenceElementName;
RGXMLAttributeList* CurrentAttributes;
RGXMLAttributeList* LastAttribute;
RGXMLAttributeList* NextAttribute;
RGString IncludePath;
RGXMLType* NewType;
RGXMLType* ListElement;
Boolean Success;
RGString ErrorString;

RGString TempName;
RGString TempType;

%}

/* 
 * Define the tokens produced by yylex()
 */


%union {
  
  unsigned int   ivalue;
  double		 dvalue;
}

%debug

%token <ivalue> SEQUENCE ENDSEQUENCE BEGINELEMENT END QUOTE STRINGNAME EQUALS NAME TYPE MINOCCURS MAXOCCURS
%token <ivalue> UNBOUNDED REFERENCE ABSTRACT SUBSTITUTIONGROUP INTEGER COMPLEXSEQUENCE BEGINKEY ENDKEY SELECTOR
%token <ivalue> XPATH FIELD KEYREFERENCE ENDKEYREFERENCE INCLUDE SCHEMALOCATION ENDELEMENT REFERTOKEY
%token <ivalue> ENDCOMPLEXTYPE COMPLEXTYPE RIGHTBRACKET RGEOF BASE EXTENSION ENDEXTENSION SIMPLETYPE ENDSIMPLETYPE
%token <ivalue> RESTRICTION ENDRESTRICTION VALUE ENDRESTRICTION MININCLUSIVE MAXINCLUSIVE MINEXCLUSIVE MAXEXCLUSIVE
%token <ivalue> ENUMERATION LENGTH MAXLENGTH MINLENGTH TOTALDIGITS FRACTIONDIGITS WHITESPACE PRESERVE REPLACE
%token <ivalue> COLLAPSE

%type <ivalue> nameAttribute typeAttribute minAttribute maxAttribute refAttribute absAttribute baseAttribute
%type <ivalue> subsAttribute locAttribute xpathAttribute reftoKeyAttribute include sequence elements
%type <ivalue> listAttribute endoffile attributesEnd attributesRightBracket list element complexDef complexDefNoName


/*
*  Begin specification of the parser.
 */
%%

declarations:	declaration
	|	declarations declaration
	;
	
declaration:	element
	|			include
	|			complexDef
	|			list
	|			endoffile
	;
	
endoffile:		RGEOF {
					
				//	cout << "Found end of file" << endl;
					return 0;
				}
	;
	
list:			COMPLEXTYPE attributesRightBracket COMPLEXSEQUENCE minAttribute listAttributeBracket
			element ENDSEQUENCE ENDCOMPLEXTYPE {

			//	cout << "Found list with min attribute directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
								
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete or missing";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement); // This function makes a copy of 'ListElement'; clean up after
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			}
			
	|			COMPLEXTYPE attributesRightBracket COMPLEXSEQUENCE listAttributeBracket
			element ENDSEQUENCE ENDCOMPLEXTYPE {

			//	cout << "Found list directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
								
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete or missing";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement); // This function makes a copy of 'ListElement'; clean up after
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			}
			
	|			COMPLEXTYPE attributesRightBracket EXTENSION attributesRightBracket COMPLEXSEQUENCE minAttribute listAttributeBracket
			element ENDSEQUENCE ENDEXTENSION ENDCOMPLEXTYPE {

			//	cout << "Found list with min attribute directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list with extension:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
				NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute->SetBaseFromAttributeList (NextAttribute);
				delete NextAttribute;
								
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement);
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			}
			
	|			COMPLEXTYPE attributesRightBracket EXTENSION attributesRightBracket COMPLEXSEQUENCE listAttributeBracket
			element ENDSEQUENCE ENDEXTENSION ENDCOMPLEXTYPE {

			//	cout << "Found list directive" << endl;
				ListDepth--;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex list with extension:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  nesting of unbounded list declarations not allowed";
					yyerror (ErrorString);
					return 0;
				}
				
				if (ListDepth < 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  unmatched unbounded list identifiers";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLComplexList ();
				NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				LastAttribute->SetBaseFromAttributeList (NextAttribute);
				delete NextAttribute;
								
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  must not have nested elements with unbounded lists";
					yyerror (ErrorString);
					return 0;
				}
				
				ListElement = (RGXMLType*)ListElementStack.GetFirst ();
				
				if (ListElement == NULL) {
				
					ErrorString = "****error in building complex list named " + *NewElementName;
					ErrorString += ":  could not find list element declaration";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType->SetListElement (ListElement);
				delete ListElement;
				
				delete NewElementName;
				ResultList->Append (NewType);
			}
	;
	
	
element:	BEGINELEMENT attributesEnd {

			//	cout << "Found element with attributes only" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building simple element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				NewType = new RGXMLElementType ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?
				
				if (ElementDepth > 0) {
				
					ParentElementName = (RGString*) ElementNameStack.First ();  // this should be parent element's name
					
					if (ParentElementName == NULL) {
					
						ErrorString = "****error in building element named " + *NewElementName;
						ErrorString += ":  parent name is missing";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->SetContainerName (*ParentElementName);
					
					if (!Success) {
					
						ErrorString = "****error in setting parent name:  " + *ParentElementName;
						yyerror (ErrorString);
						return 0;
					}
				}
				
				delete NewElementName;
				
				if (SequenceDepth > 0)				
					SequencedElementsStack.Append (NewType);
				
				else if (ListDepth > 0)
					ListElementStack.Prepend (NewType);
				
				else
					ResultList->Append (NewType);
			}
			
	|		BEGINELEMENT END {
	
			//	cout << "Found extremely simple element, with no info" << endl;
				ElementDepth--;  // there is nothing to do with this; it contains no information
			}
			
	|		BEGINELEMENT attributesRightBracket keydefinitions ENDELEMENT {
				
			//	cout << "Found element with attributes and keys and/or key references" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				// ignore key data for now
				
				NewType = new RGXMLElementType ();
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?
				
				if (ElementDepth > 0) {
				
					ParentElementName = (RGString*) ElementNameStack.First ();  // this should be previous element's name
					Success = NewType->SetContainerName (*ParentElementName);
					
					if (!Success) {
					
						ErrorString = "****error in setting parent name:  " + *ParentElementName;
						yyerror (ErrorString);
						return 0;
					}
				}
				
				delete NewElementName;
				
				if (SequenceDepth > 0)				
					SequencedElementsStack.Append (NewType);
				
				else if (ListDepth > 0)
					ListElementStack.Prepend (NewType);
				
				else
					ResultList->Append (NewType);
			}
			
	|		BEGINELEMENT attributesRightBracket complexDef keydefinitions ENDELEMENT {

				// How can a complex type with name appear as a sub-element of a named element?  Don't think so...
				cout << "***Found complex element, possibly including attributes, keys, key references and subelements" << endl;
			}
				
	|		BEGINELEMENT attributesRightBracket complexDef ENDELEMENT {
	
				// How can a complex type with name appear as a sub-element of a named element?  Don't think so...
				cout << "****Found complex element, possibly including attributes, keys, and subelements:  NO REFS" << endl;
	}
	
	|		BEGINELEMENT attributesRightBracket complexDefNoName keydefinitions ENDELEMENT {

				// the complexDefNoName is, essentially, a sequence, so we treat it like one; ignore key definitions for now
				
			//	cout << "Found complex element, possibly including attributes, keys, key references and subelements" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				//
				// There should be no parent for this kind of element
				//
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex element with keys:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				SequenceDepth--;
				NewType = new RGXMLElementType ();
				
				//
				// complexDefNoName has no name or other attributes, so get name from element attributes
				//
				
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  must not have nested elements with sequences";
					yyerror (ErrorString);
					return 0;
				}
				
				Success = NewType->AddSubElementList (SequencedElementsStack);
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  could not transfer element sequence stack";
					yyerror (ErrorString);
					return 0;
				}
				
				delete NewElementName;
				ResultList->Append (NewType);
			}
				
	|		BEGINELEMENT attributesRightBracket complexDefNoName ENDELEMENT {
	
				// the complexDefNoName is, essentially, a sequence, so we treat it like one; ignore key definitions for now
				
			//	cout << "Found complex element, possibly including attributes, keys, and subelements:  NO REFS" << endl;
				NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
				
				if (NewElementName == NULL) {
				
					ErrorString = "****error in building complex element:  name not in name stack";
					yyerror (ErrorString);
					return 0;
				}
				
				SequenceDepth--;
				NewType = new RGXMLElementType ();
				
				//
				// complexDefNoName has no name or other attributes, so get name from element attributes
				//
				
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				Success = NewType->SetAttributes (LastAttribute);
				delete LastAttribute;
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  attributes incomplete";
					yyerror (ErrorString);
					return 0;
				}
				
				ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
				
				if (ElementDepth > 0) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  must not have nested elements with sequences";
					yyerror (ErrorString);
					return 0;
				}
				
				Success = NewType->AddSubElementList (SequencedElementsStack);
				
				if (!Success) {
				
					ErrorString = "****error in building element named " + *NewElementName;
					ErrorString += ":  could not transfer element sequence stack";
					yyerror (ErrorString);
					return 0;
				}
				
				delete NewElementName;
				ResultList->Append (NewType);
	}
	;
	
include:	INCLUDE attributesEnd {

			//	cout << "Found include directive" << endl;
				LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
				
				if (LastAttribute == NULL) {
				
					yyerror ("****Warning****No attributes for include statement");
				}
				
				else {
				
					LastAttribute->GetIncludePath (IncludePath);
					delete LastAttribute;
					NewString = new RGString (IncludePath);
					LocalSchemasNeeded.Append (NewString);
				}
			}
	;
	
	
complexDef:		COMPLEXTYPE attributesRightBracket sequence ENDCOMPLEXTYPE {

				//	cout << "Found complex type definition, possibly including sequence" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					SequenceDepth--;
					NewType = new RGXMLComplexType ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					Success = NewType->SetAttributes (LastAttribute);
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
					
					if (ElementDepth > 0) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  named complex type may not have a parent element";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->AddSubElementList (SequencedElementsStack);
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  could not transfer element sequence stack";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ResultList->Append (NewType);
				}
				
	|			COMPLEXTYPE attributesRightBracket EXTENSION attributesRightBracket sequence ENDEXTENSION ENDCOMPLEXTYPE {
	
				//	cout << "Found complex type definition with attributes, extension base and sequence" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					SequenceDepth--;
					NewType = new RGXMLComplexType ();
					NextAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					
					if ((NextAttribute == NULL) || (LastAttribute == NULL)) {
					
						ErrorString = "****error in building complex type with extension named " + *NewElementName;
						ErrorString += ":  could not get both attribute lists";
						yyerror (ErrorString);
						return 0;
					}
					
					LastAttribute->SetBaseFromAttributeList (NextAttribute);
					delete NextAttribute;
					Success = NewType->SetAttributes (LastAttribute);
					
					LastAttribute->GetNameAndBase (TempName, TempType);
				//	cout << "Found complex type with EXTENSION:  Name = " << TempName << " and Base = " << TempType << endl;
					
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					ElementDepth--;  // this removes the depth level for this element...any left?  Shouldn't be...
					
					if (ElementDepth > 0) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  must not have a parent element";
						yyerror (ErrorString);
						return 0;
					}
					
					Success = NewType->AddSubElementList (SequencedElementsStack);
					
					if (!Success) {
					
						ErrorString = "****error in building complex type named " + *NewElementName;
						ErrorString += ":  could not transfer element sequence stack";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ResultList->Append (NewType);
				}
				
	|			COMPLEXTYPE attributesEnd {
	
				//	cout << "Found 'simple' complex type:  attributes only" << endl;
					NewElementName = (RGString*) ElementNameStack.GetFirst ();  // this should be the name of this element
					
					if (NewElementName == NULL) {
					
						ErrorString = "****error in building complex type with attributes only:  name not in name stack";
						yyerror (ErrorString);
						return 0;
					}
					
					NewType = new RGXMLComplexType ();
					LastAttribute = (RGXMLAttributeList*)AttributeStack.GetFirst ();
					Success = NewType->SetAttributes (LastAttribute);
					delete LastAttribute;
					
					if (!Success) {
					
						ErrorString = "****error in building complex element named " + *NewElementName;
						ErrorString += ":  attributes incomplete";
						yyerror (ErrorString);
						return 0;
					}
					
					delete NewElementName;
					ElementDepth--;  // this removes the depth level for this element...any left?
					ResultList->Append (NewType);
	}
	;
	
complexDefNoName:	COMPLEXTYPE RIGHTBRACKET sequence ENDCOMPLEXTYPE {
	
				//	cout << "Found complex type definition with sequence but no attributes" << endl;
					ElementDepth--;
				}
	; 

attributes:	 
	|	attributes attribute 
	;
	


attribute:		nameAttribute	
	|			typeAttribute	
	|			minAttribute		
	|			maxAttribute
	|			listAttribute
	|			refAttribute
	|			absAttribute
	|			subsAttribute
	|			locAttribute
	|			xpathAttribute
	|			reftoKeyAttribute
	|			baseAttribute
	;
	
nameAttribute:	NAME EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming name attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
					
				//	cout << "Found name attribute:  " << *NewString << endl;
					NewElementName = new RGString (*NewString);
					ElementNameStack.Prepend (NewElementName);
					
					if (!CurrentAttributes->AddAttribute (RGNAME, NewString)) {
						
						yyerror("****error in name attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
typeAttribute:	TYPE EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming type attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
					
				//	cout << "Found type attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGTYPE, NewString)) {
						
						yyerror("****error in type attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
minAttribute:	MINOCCURS EQUALS QUOTE INTEGER QUOTE {

					NewString = (RGString*) NumberStack.GetFirst ();
				//	cout << "Found 'min occurs' attribute:  " << *NewString << endl;
					delete NewString;  // ignore it
				}
	;
	
maxAttribute:	MAXOCCURS EQUALS QUOTE INTEGER QUOTE {

					NewString = (RGString*) NumberStack.GetFirst ();
				//	cout << "Found 'max occurs' attribute:  " << *NewString << endl;
					delete NewString;
				}
	;
	
listAttribute:	MAXOCCURS EQUALS QUOTE UNBOUNDED QUOTE {

					ListDepth++;
					
					if (ListDepth > 1) {
					
						yyerror("****error in list attribute:  no nested unbounded list declarations allowed");
						return 0;
					}
					
				//	cout << "Found 'max occurs' attribute:  unbounded" << endl;
				}
	;
	
refAttribute:	REFERENCE EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming reference attribute:  no string data found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found ref attribute:  " << *NewString << endl;
					
					NewElementName = new RGString (*NewString);
					ElementNameStack.Prepend (NewElementName);
					
					if (!CurrentAttributes->AddAttribute (RGREFERENCE, NewString)) {
						
						yyerror("****error in reference attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
absAttribute:	ABSTRACT EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming abstract attribute:  no string data found";
						yyerror (ErrorString);
						return 0;
					}
					
					NewString->ToLower ();
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found abstract attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGABSTRACT, NewString)) {
						
						yyerror("****error in abstract attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
subsAttribute:	SUBSTITUTIONGROUP EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming substitution group attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found substitution group attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGSUBSTITUTIONGROUP, NewString)) {
						
						yyerror("****error in substitution group attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
locAttribute:	SCHEMALOCATION EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming location attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found schema location attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGINCLUDEPATH, NewString)) {
						
						yyerror("****error in schema location attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
xpathAttribute:	XPATH EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming xpath attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found xpath attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGXPATH, NewString)) {
						
						yyerror("****error in xpath attribute:  require a value");
						return 0;
					}
				}
	;
	
reftoKeyAttribute:  REFERTOKEY EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming key reference attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found key reference attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGKEYREFERENCE, NewString)) {
						
						yyerror("****error in key reference attribute:  require exactly one value");
						return 0;
					}
				}
	;
	
baseAttribute:  BASE EQUALS QUOTE STRINGNAME QUOTE {

					NewString = (RGString*) NameStack.GetFirst ();
					
					if (NewString == NULL) {
					
						ErrorString = "****error in forming base attribute:  no string name found";
						yyerror (ErrorString);
						return 0;
					}
					
					CurrentAttributes = GetCurrentAttributeList ();
				//	cout << "Found base attribute:  " << *NewString << endl;
					
					if (!CurrentAttributes->AddAttribute (RGBASE, NewString)) {
						
						yyerror("****error in base attribute:  require exactly one value");
						return 0;
					}
					
				//	CurrentAttributes->AddAttribute (RGNAME, NewString);
				}
	;
	
attributesEnd:		attributes END {

	//	cout << "Found attributes plus End directive" << endl;
		SaveCurrentAttributes ();
	}
	;
	
attributesRightBracket:	attributes RIGHTBRACKET {

	//	cout << "Found attributes plus right bracket" << endl;
		SaveCurrentAttributes ();
	}
	;
	
sequence:	SEQUENCE elements ENDSEQUENCE {

			//	cout << "Found sequence" << endl;
				
				if (SequenceDepth > 1) {
				
					ErrorString = "****error in sequence for element named " + *(RGString*)ElementNameStack.First ();
					ErrorString += ":  sequence within sequence not allowed";
					yyerror (ErrorString);
					return 0;
				}
			}
	;
	
elements:	element
	|		elements element {
				
			//	cout << "Found multiple elements..." << endl;
	}
	;
	
key:		BEGINKEY attributesRightBracket selector field ENDKEY {

			//	cout << "Found key directive" << endl;
			}
	;
	
keyreference:	KEYREFERENCE attributesRightBracket selector field ENDKEYREFERENCE {

			//		cout << "Found key reference directive" << endl;
				}
	;
	
keydefinition:	key { cout << "Found a key" << endl; }
	|			keyreference { cout << "Found a key reference" << endl; }
	;
	
keydefinitions:	
	|			keydefinitions keydefinition
	;
	
selector:	SELECTOR attributesEnd {

			//	cout << "Found selector directive" << endl;
			}
	;
	
field:		FIELD attributesEnd {

			//	cout << "Found field directive" << endl;
			}
	;
	
listAttributeBracket:	listAttribute RIGHTBRACKET {

	//	cout << "Found a listAttribute plus right bracket" << endl;
	}
	;


    
%%
    
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>


int lineno = 1;
int last_err_line;
int errcount;
int TotalErrors;


#define MAX_ERR_PER_LINE  30

/*
 * yyerr() is called automatically by YYPARSE() when an 
 * error is detected in the input stream.   To avoid an avalanch of
 * errors, yyerror() only prints one message per line,
 * and if it sees too-many errors on a line, aborts the program.
 */
 
int yyerror(const char *s)
{
	char ermsg[80];
	TotalErrors++;
	
	if (lineno == last_err_line) {
	
	    if (errcount++ >= MAX_ERR_PER_LINE) {
	    
			sprintf(ermsg, "%s: Line %d",s,lineno);
			fprintf(stderr,"line %d: too many errors to continue\n",lineno);
			exit(1);  //???
	    }
	    
	    fprintf(stderr,"******line %d: %s\n",lineno,s);
		return 0;
	}
	
	errcount = 0;
	sprintf (ermsg, "%s: Line %d",s,lineno);
    fprintf (stderr,"******line %d: %s\n",lineno,s);
    cout << s << endl;
    last_err_line = lineno;
    return 1;
}



int RGXMLSchemaSummary :: CompileSchema () {
	
	int errcnt = 0;
	int status;
	size_t startPosition;
	size_t endPosition;
	RGString* NextSchemaFile;
	RGXMLSchemaSummary* NewSummary;
	
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	AttributeStack.ClearAndDelete ();
	XPathStack.ClearAndDelete ();
	SequencedElementsStack.ClearAndDelete ();
	NestedElementNameStack.ClearAndDelete ();
	ElementDepth = 0;
	ListDepth = 0;
	SequenceDepth = 0;
	ListElementStack.ClearAndDelete ();
	ElementNameStack.ClearAndDelete ();
	
	LocalSchemasNeeded.ClearAndDelete ();
	SchemaDataResults.ClearAndDelete ();
	IncludedSchemaSummaries.ClearAndDelete ();
	FilesAlreadyDone.ClearAndDelete ();
	SchemaPath.FindAndReplaceAllSubstrings ("\\", "/");
	RGString LogPath (SchemaPath);
	Boolean FoundSlash = LogPath.FindLastSubstring ("/", startPosition, endPosition);
	
	if (FoundSlash)	{
	
		LogPath.ExtractAndRemoveSubstring (endPosition + 1, SchemaPath.Length () - 1);
		LogPath += "Logfile.txt";
	}
		
	else
		LogPath = "Logfile.txt";

	ClearAttributes ();
	
	FilesScanned = &FilesAlreadyDone;
	ResultList = &SchemaDataResults;
	
	RGString* ThisPath = new RGString (SchemaPath);
	FilesScanned->Append (ThisPath);
	
	yyin = freopen (SchemaPath, "rt", stdin);
	yyout = freopen (LogPath, "wt", stdout);

	TotalErrors = 0;
	
	if (YYPARSE()) {
		
		errcnt++;
		cout << "Schema " << SchemaPath << " did not compile." << endl;
		return 1;
	}
		
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	cout << "Schema " << SchemaPath << " compiled successfully." << endl;
	RGDListIterator it (LocalSchemasNeeded);
	
	while (NextSchemaFile = (RGString*) it ()) {
	
		NewSummary = new RGXMLSchemaSummary (*NextSchemaFile);
		status = NewSummary->CompileIncludedSchema (*FilesScanned, *ResultList);
		IncludedSchemaSummaries.Append (NewSummary);
		
		if (status > 0)
			return status;
	}
	
	fcloseall ();
	return 0;
}


int RGXMLSchemaSummary :: CompileIncludedSchema (RGDList& FilesAlreadyScanned, RGDList& Results) {

	int errcnt = 0;
	int status;
	RGString* NextSchemaFile;
	RGXMLSchemaSummary* NewSummary;
	
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	AttributeStack.ClearAndDelete ();
	XPathStack.ClearAndDelete ();
	SequencedElementsStack.ClearAndDelete ();
	NestedElementNameStack.ClearAndDelete ();
	ElementDepth = 0;
	ListDepth = 0;
	SequenceDepth = 0;
	ListElementStack.ClearAndDelete ();
	ElementNameStack.ClearAndDelete ();
	
	LocalSchemasNeeded.ClearAndDelete ();
	SchemaDataResults.ClearAndDelete ();
	IncludedSchemaSummaries.ClearAndDelete ();

	ClearAttributes ();
	
	FilesScanned = &FilesAlreadyScanned;
	ResultList = &Results;
	
	RGString* ThisPath = new RGString (SchemaPath);
	
	if (!FilesScanned->Contains (ThisPath))
		FilesScanned->Append (ThisPath);
		
	else {
	
		delete ThisPath;
		return 0;
	}
		
	yyin = freopen (SchemaPath, "rt", stdin);

	TotalErrors = 0;
	
	if (YYPARSE()) {
		
		errcnt++;
		cout << "Schema " << SchemaPath << " did not compile." << endl;
		return 1;
	}
		
	NameStack.ClearAndDelete ();
	NumberStack.ClearAndDelete ();
	cout << "Schema " << SchemaPath << " compiled successfully." << endl;
	RGDListIterator it (LocalSchemasNeeded);
	
	while (NextSchemaFile = (RGString*) it ()) {
	
		NewSummary = new RGXMLSchemaSummary (*NextSchemaFile);
		status = NewSummary->CompileIncludedSchema (*FilesScanned, *ResultList);
		IncludedSchemaSummaries.Append (NewSummary);
		
		if (status > 0)
			return status;
	}
	
	fcloseall ();
	return 0;
}


