%option lex-compat noyywrap never-interactive

%{

#include "Expression.h"
//#include "ExpressionParser.hpp"
#include <string.h>
RGString *s;



//#define YY_NO_SCAN_BUFFER 1
//#define YY_NO_SCAN_BYTES 1
//#define YY_NO_SCAN_STRING 1

#define YY_NO_UNISTD_H

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) result = Expression::CompileInput (buf, max_size)

void Expression :: CleanUpBuffers () {
#if 0
// not used on the macintosh

	if (yy_current_buffer != NULL)
		Expr_delete_buffer (yy_current_buffer);
		
	yy_current_buffer = NULL;
#endif
}

int Expression :: CompileInput (char* Buffer, int MaxSize) {

	int howManyCopied;

	if (RemainingCharacters == 0)
		return YY_NULL;
	
	const char* currentPtr = ExpressionCriterion.GetData () + ExpressionCriterion.Length () - RemainingCharacters;
		
	if (RemainingCharacters <= MaxSize) {
	
		memcpy (Buffer, currentPtr, RemainingCharacters);
		howManyCopied = RemainingCharacters;
		RemainingCharacters = 0;
		return howManyCopied;
	}
	
	// So, RemainingCharacters > MaxSize, so copy MaxSize characters
	
	memcpy (Buffer, currentPtr, MaxSize);
	RemainingCharacters -= MaxSize;
	return MaxSize;		
}


/*  */
%}
      
digit [0-9]
space [ \t]
comma [,]
open [(]
close [)]
name [a-zA-Z_]+[a-zA-Z0-9_]*
%%

{space}+		;	
"//".*			{; }


space       {;}
"1"			{ return TRUECONST; }
"0"			{ return FALSECONST; }
"true"			{ return TRUECONST; }
"false"			{ return FALSECONST; }
"TRUE"			{ return TRUECONST; }
"FALSE"			{ return FALSECONST; }
"True"			{ return TRUECONST; }
"False"			{ return FALSECONST; }

"AND"			{ return AND; }
"and"			{ return AND; }
"And"			{ return AND; }

"OR"			{ return OR; }
"or"			{ return OR; }
"Or"			{ return OR; }

"^"				{ return EXCLUSIVEOR; }
"!"				{ return NOT; }
"~"				{ return NOT; }
"NOT"			{ return NOT; }
"not"			{ return NOT; }
"Not"			{ return NOT; }

[a-zA-Z_]+[a-zA-Z0-9_]*		{ s = new RGString (yytext); name_stack.Prepend (s); return NAME; }

"("			{ return '('; }
")"			{ return ')'; }
";"			{ return END; }

.               { return yytext[0]; }	/* needed to return single character ops???? */


%%

