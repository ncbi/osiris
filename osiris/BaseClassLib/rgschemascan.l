%option lex-compat noyywrap never-interactive

%{


#include "rgstring.h"
#include "rgschemascan.tab.h"
#include "rgxmlschema.h"

extern int lineno;        /* current line number  */

RGString* s;



/*  */
%}
      
digit [0-9]
space [ \t\n]
Name [a-zA-Z\.]+[a-zA-Z0-9_:\.\/]*
Number [0-9]+
startc "<!--"

%%

\n						{ ++lineno; }

{space}+				{/*cout << "spaces..." << endl;*/}
">"						{ return RIGHTBRACKET; }
{startc}.*"-->"{space}*	{ /*cout << "Found comment:  " << yytext << endl;*/ }
"<?"[^?]*"?>"			{ /*cout << "Found opening line:  " << yytext << endl;*/ }
"<xs:schema".*">"		{ /*cout << "Found schema start; ignoring..." << endl;*/ }
"</xs:schema>"			{ /*cout << "Found schema end; ignoring..." << endl;*/}
"<xs:annotation>"{space}*.*{space}*"</xs:annotation>"   { /*cout << "Found annotation:  " << yytext << endl;*/}
"<xs:complexContent>"	{;}
"</xs:complexContent>"	{;}



"<xs:sequence>"			{ SequenceDepth++; return SEQUENCE; }
"</xs:sequence>"			{ return ENDSEQUENCE; }
"<xs:element"			{ ElementDepth++; return BEGINELEMENT; }
"</xs:element>"			{ return ENDELEMENT; }
"/>"					{ return END; }
"\""					{ return QUOTE; }

"="						{ return EQUALS; }
"name"					{ return NAME; }
"type"					{ return TYPE; }
"minOccurs"				{ return MINOCCURS; }
"maxOccurs"				{ return MAXOCCURS; }
"unbounded"				{ return UNBOUNDED; }
"ref"					{ return REFERENCE; }
"refer"					{ return REFERTOKEY; }
"base"					{ return BASE; }
"abstract"				{ return ABSTRACT; }
"value"					{ /*cout << "Found value token" << endl;*/ return VALUE; }
"preserve"				{ /*cout << "Found preserve token" << endl;*/ return WHITESPACEDATA; }
"replace"				{ /*cout << "Found replace token" << endl;*/ return WHITESPACEDATA; }
"collapse"				{ /*cout << "Found collapse token" << endl;*/ return WHITESPACEDATA; }
"substitutionGroup"		{ return SUBSTITUTIONGROUP; }
[0-9]+					{ yylval.ivalue = strtoul (yytext, 0, 10); s = new RGString (yytext); NameStack.Prepend (s);
							return STRINGNAME; }
-?(([0-9]+)|([0-9]*\.([0-9]+)?)([eE][-+]?[0-9]+)?) { yylval.dvalue = strtod(yytext,0);
							s = new RGString (yytext); NameStack.Prepend(s); return STRINGNAME; }
"<xs:sequence"			{ return COMPLEXSEQUENCE; }
"<xs:key"				{ return BEGINKEY; }
"</xs:key>"				{ return ENDKEY; }
"<xs:selector"			{ return SELECTOR; }
"xpath"					{ return XPATH; }
"<xs:field"				{ return FIELD; }
"<xs:keyref"			{ return KEYREFERENCE; }
"</xs:keyref>"			{ return ENDKEYREFERENCE; }
"<xs:include"			{ return INCLUDE; }
"schemaLocation"		{ return SCHEMALOCATION; }
"<xs:complexType"		{ ElementDepth++; return COMPLEXTYPE; }
"</xs:complexType>"		{ return ENDCOMPLEXTYPE; }
"<xs:simpleType"		{ ElementDepth++; return SIMPLETYPE; }
"</xs:simpleType>"		{ return ENDSIMPLETYPE; }
"<xs:extension"			{ return EXTENSION; }
"</xs:extension>"		{ return ENDEXTENSION; }
"<xs:restriction"		{ return RESTRICTION; }
"</xs:restriction>"		{ return ENDRESTRICTION; }
"<xs:minInclusive"		{ return MININCLUSIVE; }
"<xs:maxInclusive"		{ return MAXINCLUSIVE; }
"<xs:minExclusive"		{ return MINEXCLUSIVE; }
"<xs:maxExclusive"		{ return MAXEXCLUSIVE; }
"<xs:enumeration"		{ return ENUMERATION; }
"<xs:length"			{ return LENGTH; }
"<xs:maxLength"			{ return MAXLENGTH; }
"<xs:minLength"			{ return MINLENGTH; }
"<xs:totalDigits"		{ return TOTALDIGITS; }
"<xs:fractionDigits"	{ return FRACTIONDIGITS; }
"<xs:whiteSpace"		{ /*cout << "Found whitespace token" << endl;*/ return WHITESPACE; }
<<EOF>>					{ return RGEOF; }

{Name}					{ s = new RGString (yytext); NameStack.Prepend (s); return STRINGNAME; }

.               { return yytext[0]; }	/* needed to return all other characters. */



%%
