
%{



#include <stdio.h>
#include <string.h>
//#include <unistd.h>
#include <stdlib.h>
#include <math.h>
//#include <iostream>
#include "rgstring.h"
#include "Expression.h"
#include "rgdlist.h"

//using namespace std;


//#undef Exprparse
//#define Exprparse YYPARSE



#define nl '\n'  

struct yy_buffer_state;

char *sval; 
extern int yylex(void);
int Exprerror(const char *s);
//extern yy_buffer_state* yy_current_buffer;
extern void Expr_delete_buffer( yy_buffer_state* b );

RGDList name_stack;


size_t num;
RGString* rstr;



char buffer[80]; 

%}

/* 
 * Define the tokens produced by yylex()
 */
 

%union {
  
  unsigned int   ivalue;
  double         dvalue;
  Expression*    evalue;
}


%token <ivalue> NAME TRUECONST FALSECONST AND OR EXCLUSIVEOR NOT END

%type <evalue> expr


%left OR EXCLUSIVEOR
%left AND
%nonassoc NOT

/*
*  Begin specification of the parser.
 */
%%

complete_expr:	expr {

				Expression::SetCompiledExpression ((Expression*)$1);
	}

	;

expr:		NAME			{					
	            
	            //
	            // process name
	            //
	            
				rstr = (RGString*)name_stack.GetFirst ();
				$$ = new SingleMessage (*rstr);
				delete rstr;
	}
	
	|       TRUECONST   {
	            
	            //rstr = (RGString*)name_stack.GetFirst ();
	            //delete rstr;
	            $$ = new ConstantExpr (true);
	}
	
	|       FALSECONST   {
	            
	            //rstr = (RGString*)name_stack.GetFirst ();
	            //delete rstr;
	            $$ = new ConstantExpr (false);
	}
		
	|       '(' expr ')' {
	            
	            $$ = $2;
	}
	
	|       expr AND expr {
	            
	            $$ = new And ($1, $3);
	}
	
	|       expr OR expr {
	            
	            $$ = new Or ($1, $3);
	}
	
	|       NOT expr %prec NOT {    //????????????????????????????????
	            
	            $$ = new Not ($2);
	}
	
	|       expr EXCLUSIVEOR expr {
	            
	            $$ = new ExclusiveOr ($1, $3);
	}
	
	;

%%
    
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>



//int lineno = 1;
int last_err_line;
//int errcount;


#define MAX_ERR_PER_LINE  1

/*
 * yyerr() is called automatically by YYPARSE() when an 
 * error is detected in the input stream.   To avoid an avalanch of
 * errors, yyerror() only prints one message per line,
 * and if it sees too-many errors on a line, aborts the program.
 */
 
int Exprerror(const char *s) {

//	char ermsg[80];
    cout << "Error in expression..." << s << endl;
	return 1;
}



Expression* Expression :: Compile (const RGString& expressionString) {
	
	int errcnt = 0;
	finalExpression = NULL;
	RemainingCharacters = (int)expressionString.Length ();
	ExpressionCriterion = expressionString;
	name_stack.ClearAndDelete ();
		
	if ( Exprparse ())
		errcnt++;
	
	if (errcnt > 0)
		cout << "Found errors in:  " << expressionString.GetData () << endl;
		
	if (name_stack.Entries () > 0)
		cout << "Left with " << name_stack.Entries () << " names still in stack...compilation incomplete." << endl;
		
	name_stack.ClearAndDelete ();
		
	if (errcnt == 0)
		return finalExpression;
	
	return NULL;

}

