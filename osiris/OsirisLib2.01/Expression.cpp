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
*  FileName: Expression.cpp
*  Author:   Robert Goor
*
*/
// Source file for base class expression and subclasses.  These 
// classes perform the actual calculations in messages
//



#include <stdlib.h>
#include <math.h>

#include "ChannelData.h"
#include "DataSignal.h"
#include "Genetics.h"
#include "Expression.h"
#include "SmartMessage.h"
#include "STRLCAnalysis.h"

#include <iostream>

using namespace std;



PERSISTENT_DEFINITION (Expression, _EXPRESSION_, "Expression")
PERSISTENT_DEFINITION (Unary, _UNARY_, "Unary")
PERSISTENT_DEFINITION (Binary, _BINARY_, "Binary")

PERSISTENT_DEFINITION (And, _AND_, "And")
PERSISTENT_DEFINITION (Or, _OR_, "Or")
PERSISTENT_DEFINITION (Not, _NOT_, "Not")
//PERSISTENT_DEFINITION (LogicalEqual, _LOGICALEQUAL_, "LogicalEqual")
//PERSISTENT_DEFINITION (GTEqual, _GTEQUAL_, "GTEqual")
//PERSISTENT_DEFINITION (LTEqual, _LTEQUAL_, "LTEqual")
//PERSISTENT_DEFINITION (NotEqual, _NOTEQUAL_, "NotEqual")
//PERSISTENT_DEFINITION (LessThan, _LESSTHAN_, "LessThan")
//PERSISTENT_DEFINITION (GreaterThan, _GREATERTHAN_, "GreaterThan")
PERSISTENT_DEFINITION (ExclusiveOr, _EXCLUSIVEOR_, "ExclusiveOr")
PERSISTENT_DEFINITION (SingleMessage, _SINGLEMESSAGE_, "SingleMessage")
PERSISTENT_DEFINITION (AnyExpr, _ANY_, "AnyExpr")
PERSISTENT_DEFINITION (AllExpr, _ALL_, "AllExpr")
PERSISTENT_DEFINITION (ConstantExpr, _CONSTANTEXPR_, "ConstantExpr")


ExpressionCleanUp Expression::CleanUp;


ExpressionCleanUp :: ~ExpressionCleanUp () {

	Expression::CleanUpBuffers ();
}


Expression* Expression :: finalExpression = NULL;
RGString Expression :: ExpressionCriterion;
int Expression :: RemainingCharacters = 0;


Expression :: Expression (const Expression& ex) : RGPersistent () {

}


Expression :: ~Expression () {}


Expression* Expression :: MakeCopy () {

	return new Expression;
}


int Expression :: Initialize (int scope, const SmartMessage* sm) {
  
	return -1;
}


bool Expression :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	return false;
}


bool Expression :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return false;
}


bool Expression :: EvaluateLocal (const bool* msgArray) const {

	return false;
}



size_t Expression :: StoreSize () const {
  
	return RGPersistent::StoreSize();
}



void Expression :: RestoreAll (RGFile& f) {
  
	RGPersistent :: RestoreAll (f);
}




void Expression :: RestoreAll (RGVInStream& s) {
  
	RGPersistent :: RestoreAll (s);
}




void Expression :: SaveAll (RGFile& f) const {
  
	RGPersistent :: SaveAll (f);
}




void Expression :: SaveAll (RGVOutStream& s) const {
  
	RGPersistent :: SaveAll (s);
}



Unary :: Unary () : Expression (), mOperand (NULL) {}


Unary :: Unary (Expression* op) : Expression (), mOperand (op) {}


Unary :: Unary (const Unary& un) : Expression (un) {

	if (un.mOperand == NULL)
		mOperand = NULL;

	else
		mOperand = un.mOperand->MakeCopy ();
}


Unary :: ~Unary () {
  
	delete mOperand;
}


Expression* Unary :: MakeCopy () {

	if (mOperand == NULL)
		return new Unary (NULL);

	Expression* newExpression = mOperand->MakeCopy ();
	return new Unary (newExpression);
}


int Unary :: Initialize (int scope, const SmartMessage* sm) {
	
	if (mOperand == NULL) {

		cout << "Unary operator has no operand" << endl;
		return -1;
	}

	return mOperand->Initialize (scope, sm);
}





size_t Unary :: StoreSize () const {
  
	size_t count = Expression::StoreSize();
	count += sizeof (Expression*);
	count += mOperand->StoreSize();
	return count;
}



void Unary :: RestoreAll (RGFile& f) {
  
	Expression :: RestoreAll (f);
	delete mOperand;
	mOperand = NULL;
	f >> mOperand;
}




void Unary :: RestoreAll (RGVInStream& s) {
  
	Expression :: RestoreAll (s);
	delete mOperand;
	mOperand = NULL;
	s >> mOperand;
}




void Unary :: SaveAll (RGFile& f) const {
  
	Expression :: SaveAll (f);
	mOperand->SaveAll (f);
}




void Unary :: SaveAll (RGVOutStream& s) const {
  
	Expression :: SaveAll (s);
	mOperand->SaveAll (s);
}





Binary :: Binary () : Expression (), mOperand1 (NULL), mOperand2 (NULL) {}


Binary :: Binary (Expression* op1, Expression* op2) : Expression (), 
mOperand1 (op1), mOperand2 (op2) {
   
}


Binary :: Binary (const Binary& bin) : Expression (bin) {
	
	if (bin.mOperand1 == NULL)
		mOperand1 = NULL;

	else
		mOperand1 = bin.mOperand1->MakeCopy ();

	if (bin.mOperand2 == NULL)
		mOperand2 = NULL;

	else
		mOperand2 = bin.mOperand2->MakeCopy ();
}


Binary :: ~Binary () {
  
	delete mOperand1;
	delete mOperand2;
}


Expression* Binary :: MakeCopy () {

	Expression* newExpression1;
	Expression* newExpression2;

	if (mOperand1 == NULL)
		newExpression1 = NULL;

	else
		newExpression1 = mOperand1->MakeCopy ();

	if (mOperand2 == NULL)
		newExpression2 = NULL;

	else
		newExpression2 = mOperand2->MakeCopy ();

	return new Binary (newExpression1, newExpression2);
}



int Binary :: Initialize (int scope, const SmartMessage* sm) {

	if ((mOperand1 == NULL) || (mOperand2 == NULL)) {

		cout << "Binary operator missing one or more operands" << endl;
		return -1;
	}
  
	int type1 = mOperand1->Initialize (scope, sm);
	int type2 = mOperand2->Initialize (scope, sm);
	  
	if (type1 == 0) {
	    
		if (type2 == 0)
			return 0;
	    
		cout << "Binary operator:  second operand did not initialize" << endl;
		return 1;
	}
	  
	else if (type2 == 0) {

		cout << "Binary operator:  first operand did not initialize" << endl;
		return 2;
	}
	
	cout << "Binary operator:  neither operand initialized" << endl;
	return 3;
}





size_t Binary :: StoreSize () const {
  
	size_t count = Expression::StoreSize();
	count += 2 * sizeof (Expression*);
	count += mOperand1->StoreSize();
	count += mOperand2->StoreSize();
	return count;
}



void Binary :: RestoreAll (RGFile& f) {
  
	Expression :: RestoreAll (f);
	delete mOperand1;
	mOperand1 = NULL;
	f >> mOperand1;
	delete mOperand2;
	mOperand2 = NULL;
	f >> mOperand2;
}




void Binary :: RestoreAll (RGVInStream& s) {
  
	Expression :: RestoreAll (s);
	delete mOperand1;
	mOperand1 = NULL;
	s >> mOperand1;
	delete mOperand2;
	mOperand2 = NULL;
	s >> mOperand2;
}




void Binary :: SaveAll (RGFile& f) const {
  
	Expression :: SaveAll (f);
	mOperand1->SaveAll (f);
	mOperand2->SaveAll (f);
}




void Binary :: SaveAll (RGVOutStream& s) const {
  
	Expression :: SaveAll (s);
	mOperand1->SaveAll (s);
	mOperand2->SaveAll (s);
}




And :: And () : Binary () {}



And :: And (Expression* op1, Expression* op2) : 
    Binary (op1, op2) {}



And :: ~And () {}


Expression* And :: MakeCopy () {

	Expression* newExpression1;
	Expression* newExpression2;

	if (mOperand1 == NULL)
		newExpression1 = NULL;

	else
		newExpression1 = mOperand1->MakeCopy ();

	if (mOperand2 == NULL)
		newExpression2 = NULL;

	else
		newExpression2 = mOperand2->MakeCopy ();

	return new And (newExpression1, newExpression2);
}


bool And :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	return mOperand1->Evaluate (msgArray, subObjects) && mOperand2->Evaluate (msgArray, subObjects);
}


bool And :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return mOperand1->Evaluate (comm, numHigherObjects) && mOperand2->Evaluate (comm, numHigherObjects);
}


bool And :: EvaluateLocal (const bool* msgArray) const {

	return mOperand1->EvaluateLocal (msgArray) && mOperand2->EvaluateLocal (msgArray);
}


void And :: Print (const RGString& indent) {

	cout << indent.GetData () << "And expression with subexpressions:" << endl;
	RGString indent2 = indent + "     ";
	mOperand1->Print (indent2);
	mOperand2->Print (indent2);
}


Or :: Or () : Binary () {}



Or :: Or (Expression* op1, Expression* op2) : 
    Binary (op1, op2) {}



Or :: ~Or () {}


Expression* Or :: MakeCopy () {

	Expression* newExpression1;
	Expression* newExpression2;

	if (mOperand1 == NULL)
		newExpression1 = NULL;

	else
		newExpression1 = mOperand1->MakeCopy ();

	if (mOperand2 == NULL)
		newExpression2 = NULL;

	else
		newExpression2 = mOperand2->MakeCopy ();

	return new Or (newExpression1, newExpression2);
}


bool Or :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	return mOperand1->Evaluate (msgArray, subObjects) || mOperand2->Evaluate (msgArray, subObjects);
}


bool Or :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return mOperand1->Evaluate (comm, numHigherObjects) || mOperand2->Evaluate (comm, numHigherObjects);
}


bool Or :: EvaluateLocal (const bool* msgArray) const {

	return mOperand1->EvaluateLocal (msgArray) || mOperand2->EvaluateLocal (msgArray);
}


void Or :: Print (const RGString& indent) {

	cout << indent.GetData () << "Or expression with subexpressions:" << endl;
	RGString indent2 = indent + "     ";
	mOperand1->Print (indent2);
	mOperand2->Print (indent2);
}



//Max :: Max () : Binary () {}
//
//
//
//Max :: Max (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//Max :: ~Max () {}
//
//
//
//int Max :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 1;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &Maxintint;
//      Type = 0;
//      break;
//      
//    case 1:
//      Evaluate = &Maxintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &Maxdoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &Maxdoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void Maxintint (Expression& ex) {
//  
//  Max& mu = (Max&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  
//  if (mu.op1int >= mu.op2int)
//    *mu.IntAns = mu.op1int;
//  
//  else
//    *mu.IntAns = mu.op2int;
//}
//
//
//void Maxintdouble (Expression& ex) {
//  
//  Max& mu = (Max&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  double temp = mu.op1int;
//  
//  if (temp >= mu.op2double)
//    *mu.DoubleAns = temp;
//  
//  else
//    *mu.DoubleAns = mu.op2double;
//}
//
//
//void Maxdoubleint (Expression& ex) {
//  
//  Max& mu = (Max&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  double temp = mu.op2int;
//  
//  if (mu.op1double >= temp)
//    *mu.DoubleAns = mu.op1double;
//  
//  else
//    *mu.DoubleAns = temp;
//}
//
//
//void Maxdoubledouble (Expression& ex) {
//  
//  Max& mu = (Max&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  
//  if (mu.op1double >= mu.op2double)
//    *mu.DoubleAns = mu.op1double;
//  
//  else
//    *mu.DoubleAns = mu.op2double;
//}
//
//
//
//
//Min :: Min () : Binary () {}
//
//
//
//Min :: Min (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//Min :: ~Min () {}
//
//
//
//int Min :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 1;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &Minintint;
//      Type = 0;
//      break;
//      
//    case 1:
//      Evaluate = &Minintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &Mindoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &Mindoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void Minintint (Expression& ex) {
//  
//  Min& mu = (Min&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  
//  if (mu.op1int <= mu.op2int)
//    *mu.IntAns = mu.op1int;
//  
//  else
//    *mu.IntAns = mu.op2int;
//}
//
//
//void Minintdouble (Expression& ex) {
//  
//  Min& mu = (Min&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  double temp = mu.op1int;
//  
//  if (temp <= mu.op2double)
//    *mu.DoubleAns = temp;
//  
//  else
//    *mu.DoubleAns = mu.op2double;
//}
//
//
//void Mindoubleint (Expression& ex) {
//  
//  Min& mu = (Min&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  double temp = mu.op2int;
//  
//  if (mu.op1double <= temp)
//    *mu.DoubleAns = mu.op1double;
//  
//  else
//    *mu.DoubleAns = temp;
//}
//
//
//void Mindoubledouble (Expression& ex) {
//  
//  Min& mu = (Min&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  
//  if (mu.op1double <= mu.op2double)
//    *mu.DoubleAns = mu.op1double;
//  
//  else
//    *mu.DoubleAns = mu.op2double;
//}





Not :: Not () : Unary () {}



Not :: Not (Expression* op) : Unary (op) {}


Not :: ~Not () {}


Expression* Not :: MakeCopy () {

	if (mOperand == NULL)
		return new Not (NULL);

	Expression* newExpression = mOperand->MakeCopy ();
	return new Not (newExpression);
}



bool Not :: Evaluate (const bool* msgArray, RGDList& subObjects) const {
  
	return !(mOperand->Evaluate (msgArray, subObjects));
}


bool Not :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return !(mOperand->Evaluate (comm, numHigherObjects));
}


bool Not :: EvaluateLocal (const bool* msgArray) const {

	return !(mOperand->EvaluateLocal (msgArray));
}


void Not :: Print (const RGString& indent) {

	cout << indent.GetData () << "Not expression with subexpression:" << endl;
	RGString indent2 = indent + "     ";
	mOperand->Print (indent2);
}



//LogicalEqual :: LogicalEqual () : Binary () {}
//
//
//
//LogicalEqual :: LogicalEqual (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//LogicalEqual :: ~LogicalEqual () {}
//
//
//
//int LogicalEqual :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &Equalintint;
//      break;
//      
//    case 1:
//      Evaluate = &Equalintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &Equaldoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &Equaldoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void Equalintint (Expression& ex) {
//  
//  LogicalEqual& mu = (LogicalEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int == mu.op2int;
//}
//
//
//void Equalintdouble (Expression& ex) {
//  
//  LogicalEqual& mu = (LogicalEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int == mu.op2double;
//}
//
//
//void Equaldoubleint (Expression& ex) {
//  
//  LogicalEqual& mu = (LogicalEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double == mu.op2int;
//}
//
//
//void Equaldoubledouble (Expression& ex) {
//  
//  LogicalEqual& mu = (LogicalEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double == mu.op2double;
//}
//
//
//
//
//GTEqual :: GTEqual () : Binary () {}
//
//
//
//GTEqual :: GTEqual (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//GTEqual :: ~GTEqual () {}
//
//
//
//int GTEqual :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &GTEqualintint;
//      break;
//      
//    case 1:
//      Evaluate = &GTEqualintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &GTEqualdoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &GTEqualdoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void GTEqualintint (Expression& ex) {
//  
//  GTEqual& mu = (GTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int >= mu.op2int;
//}
//
//
//void GTEqualintdouble (Expression& ex) {
//  
//  GTEqual& mu = (GTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int >= mu.op2double;
//}
//
//
//void GTEqualdoubleint (Expression& ex) {
//  
//  GTEqual& mu = (GTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double >= mu.op2int;
//}
//
//
//void GTEqualdoubledouble (Expression& ex) {
//  
//  GTEqual& mu = (GTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double >= mu.op2double;
//}
//
//
//
//
//
//LTEqual :: LTEqual () : Binary () {}
//
//
//
//LTEqual :: LTEqual (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//LTEqual :: ~LTEqual () {}
//
//
//
//int LTEqual :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &LTEqualintint;
//      break;
//      
//    case 1:
//      Evaluate = &LTEqualintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &LTEqualdoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &LTEqualdoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void LTEqualintint (Expression& ex) {
//  
//  LTEqual& mu = (LTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int <= mu.op2int;
//}
//
//
//void LTEqualintdouble (Expression& ex) {
//  
//  LTEqual& mu = (LTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int <= mu.op2double;
//}
//
//
//void LTEqualdoubleint (Expression& ex) {
//  
//  LTEqual& mu = (LTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double <= mu.op2int;
//}
//
//
//void LTEqualdoubledouble (Expression& ex) {
//  
//  LTEqual& mu = (LTEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double <= mu.op2double;
//}
//
//
//
//
//NotEqual :: NotEqual () : Binary () {}
//
//
//
//NotEqual :: NotEqual (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//NotEqual :: ~NotEqual () {}
//
//
//
//int NotEqual :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &NotEqualintint;
//      break;
//      
//    case 1:
//      Evaluate = &NotEqualintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &NotEqualdoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &NotEqualdoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void NotEqualintint (Expression& ex) {
//  
//  NotEqual& mu = (NotEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int != mu.op2int;
//}
//
//
//void NotEqualintdouble (Expression& ex) {
//  
//  NotEqual& mu = (NotEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int != mu.op2double;
//}
//
//
//void NotEqualdoubleint (Expression& ex) {
//  
//  NotEqual& mu = (NotEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double != mu.op2int;
//}
//
//
//void NotEqualdoubledouble (Expression& ex) {
//  
//  NotEqual& mu = (NotEqual&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double != mu.op2double;
//}
//
//
//
//
//LessThan :: LessThan () : Binary () {}
//
//
//
//LessThan :: LessThan (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//LessThan :: ~LessThan () {}
//
//
//
//int LessThan :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &LessThanintint;
//      break;
//      
//    case 1:
//      Evaluate = &LessThanintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &LessThandoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &LessThandoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//void LessThanintint (Expression& ex) {
//  
//  LessThan& mu = (LessThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int < mu.op2int;
//}
//
//
//void LessThanintdouble (Expression& ex) {
//  
//  LessThan& mu = (LessThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int < mu.op2double;
//}
//
//
//void LessThandoubleint (Expression& ex) {
//  
//  LessThan& mu = (LessThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double < mu.op2int;
//}
//
//
//void LessThandoubledouble (Expression& ex) {
//  
//  LessThan& mu = (LessThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double < mu.op2double;
//}
//
//
//
//
//GreaterThan :: GreaterThan () : Binary () {}
//
//
//
//GreaterThan :: GreaterThan (Expression* op1, Expression* op2) : 
//    Binary (op1, op2) {}
//
//
//
//GreaterThan :: ~GreaterThan () {}
//
//
//
//int GreaterThan :: Initialize (int scope, const SmartMessage* sm) {
//  
//  int type = Binary::Initialize (iAns, dAns, ex);
//  Type = 0;
//  
//  switch (type) {
//    
//    case 0:
//      Evaluate = &GreaterThanintint;
//      break;
//      
//    case 1:
//      Evaluate = &GreaterThanintdouble;
//      break;
//      
//    case 2:
//      Evaluate = &GreaterThandoubleint;
//      break;
//      
//    case 3:
//      Evaluate = &GreaterThandoubledouble;
//      break;
//  }
//  
//  return Type;
//}
//
//
//
//
//void GreaterThanintint (Expression& ex) {
//  
//  GreaterThan& mu = (GreaterThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int > mu.op2int;
//}
//
//
//void GreaterThanintdouble (Expression& ex) {
//  
//  GreaterThan& mu = (GreaterThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1int > mu.op2double;
//}
//
//
//void GreaterThandoubleint (Expression& ex) {
//  
//  GreaterThan& mu = (GreaterThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double > mu.op2int;
//}
//
//
//void GreaterThandoubledouble (Expression& ex) {
//  
//  GreaterThan& mu = (GreaterThan&) ex;
//  mu.Operand1->Evaluate(*mu.Operand1);
//  mu.Operand2->Evaluate(*mu.Operand2);
//  *mu.IntAns = mu.op1double > mu.op2double;
//}





ExclusiveOr :: ExclusiveOr () : Binary () {}



ExclusiveOr :: ExclusiveOr (Expression* op1, Expression* op2) : 
    Binary (op1, op2) {}



ExclusiveOr :: ~ExclusiveOr () {}


Expression* ExclusiveOr :: MakeCopy () {

	Expression* newExpression1;
	Expression* newExpression2;

	if (mOperand1 == NULL)
		newExpression1 = NULL;

	else
		newExpression1 = mOperand1->MakeCopy ();

	if (mOperand2 == NULL)
		newExpression2 = NULL;

	else
		newExpression2 = mOperand2->MakeCopy ();

	return new ExclusiveOr (newExpression1, newExpression2);
}



bool ExclusiveOr :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	bool ans1 = mOperand1->Evaluate (msgArray, subObjects);
	bool ans2 = mOperand2->Evaluate (msgArray, subObjects);
	return ((ans1 && !ans2) || (!ans1 && ans2));
}


bool ExclusiveOr :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	bool ans1 = mOperand1->Evaluate (comm, numHigherObjects);
	bool ans2 = mOperand2->Evaluate (comm, numHigherObjects);
	return ((ans1 && !ans2) || (!ans1 && ans2));
}


bool ExclusiveOr :: EvaluateLocal (const bool* msgArray) const {

	bool ans1 = mOperand1->EvaluateLocal (msgArray);
	bool ans2 = mOperand2->EvaluateLocal (msgArray);
	return ((ans1 && !ans2) || (!ans1 && ans2));
}


void ExclusiveOr :: Print (const RGString& indent) {

	cout << indent.GetData () << "Exclusive Or expression with subexpressions:" << endl;
	RGString indent2 = indent + "     ";
	mOperand1->Print (indent2);
	mOperand2->Print (indent2);
}



SingleMessage :: SingleMessage (const RGString& name) : Expression (), mName (name),
mIndex (0), mScope (0) {

}


SingleMessage :: ~SingleMessage() {

}


Expression* SingleMessage :: MakeCopy () {

	SingleMessage* sm = new SingleMessage (mName);
	sm->mIndex = mIndex;
	sm->mScope = mScope;
	return sm;
}


int SingleMessage :: Initialize (int scope, const SmartMessage* sm) {

	mIndex = sm->FindMessageIndex (mName, mScope);

	if (mIndex < 0) {

		cout << "Could not find message index for " << mName.GetData () << endl;
		return -1;
	}

	// Now, this takes account of superlists...the appropriate index and scope have been saved.

	//if (mScope != scope) {

	//	cout << "Scope " << scope << " did not match expected scope " << mScope << " for " << mName.GetData () << endl;
	//	return -1;
	//}

	return 0;
}


bool SingleMessage :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	// We'll correct this later to account for different list(s)

	return msgArray [mIndex];
}


bool SingleMessage :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	int topNum = numHigherObjects - 1;
	SmartMessagingObject* smo = comm.SMOStack [topNum];

	if (mScope == smo->GetObjectScope ()) {

		return smo->GetMessageValue (mIndex);
	}

	int i;

	for (i=topNum-1; i>=0; i--) {

		smo = comm.SMOStack [i];

		if (mScope == smo->GetObjectScope ())
			return smo->GetMessageValue (mIndex);
	}

	return false;
}


bool SingleMessage :: EvaluateLocal (const bool* msgArray) const {

	// We'll correct this later to account for different list(s)

	return msgArray [mIndex];
}


void SingleMessage :: Print (const RGString& indent) {

	cout << indent.GetData () << "Single Message expression with name:  " << mName.GetData () << endl;
}


	
size_t SingleMessage :: StoreSize () const {

	return Expression::StoreSize () + mName.StoreSize ();
}


void SingleMessage :: RestoreAll (RGFile& f) {

	Expression::RestoreAll (f);
	mName.RestoreAll (f);
}


void SingleMessage :: RestoreAll (RGVInStream& s) {

	Expression::RestoreAll (s);
	mName.RestoreAll (s);
}


void SingleMessage :: SaveAll (RGFile& f) const {

	Expression::SaveAll (f);
	mName.SaveAll (f);
}


void SingleMessage :: SaveAll (RGVOutStream& s) const {

	Expression::SaveAll (s);
	mName.SaveAll (s);
}



AnyExpr :: AnyExpr (const RGString& name) : Expression (), mName (name),
mIndex (0), mScope (0) {

}


AnyExpr :: ~AnyExpr() {

}


Expression* AnyExpr :: MakeCopy () {

	AnyExpr* sm = new AnyExpr (mName);
	sm->mIndex = mIndex;
	sm->mScope = mScope;
	return sm;
}


int AnyExpr :: Initialize (int scope, const SmartMessage* sm) {

	mIndex = sm->FindMessageIndex (mName, mScope);

	if (mIndex < 0) {

		cout << "'Any Expression':  Could not find message index for " << mName.GetData () << endl;
		return -1;
	}

	// Later we'll correct this to take account of sublists...

	if (mScope != scope - 1) {

		cout << "'Any Expression':  Scope " << scope - 1 << " did not match expected scope " << mScope << " for " << mName.GetData () << endl;
		return -1;
	}

	return 0;
}


bool AnyExpr :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	bool ans = false;

	switch (mScope) {

		case 4:
			ans = EvaluateAnyForSamples (subObjects);
			break;

		case 3:
			ans = EvaluateAnyForDataChannels (subObjects);
			break;

		case 2:
			ans = EvaluateAnyForLoci (subObjects);
			break;

		default:
			return false;
	}

	return ans;
}


void AnyExpr :: Print (const RGString& indent) {

	cout << indent.GetData () << "'Any' expression with name:  " << mName.GetData () << endl;
}

	
size_t AnyExpr :: StoreSize () const {

	return Expression::StoreSize () + mName.StoreSize ();
}


void AnyExpr :: RestoreAll (RGFile& f) {

	Expression::RestoreAll (f);
	mName.RestoreAll (f);
}


void AnyExpr :: RestoreAll (RGVInStream& s) {

	Expression::RestoreAll (s);
	mName.RestoreAll (s);
}


void AnyExpr :: SaveAll (RGFile& f) const {

	Expression::SaveAll (f);
	mName.SaveAll (f);
}


void AnyExpr :: SaveAll (RGVOutStream& s) const {

	Expression::SaveAll (s);
	mName.SaveAll (s);
}


bool AnyExpr :: EvaluateAnyForSamples (RGDList& subObjects) const {

	// Later, recognize subObjects entries as DataChannel* and go through list

	ChannelData* q;
	RGDListIterator it (subObjects);

	while (q = (ChannelData*) it ()) {

		if (q->GetMessageValue (mIndex))
			return true;
	}

	return false;
}


bool AnyExpr :: EvaluateAnyForDataChannels (RGDList& subObjects) const {

	// Later, recognize subObjects entries as Locus* and go through list

	Locus* q;
	RGDListIterator it (subObjects);

	while (q = (Locus*) it ()) {

		if (q->GetMessageValue (mIndex))
			return true;
	}

	return false;
}


bool AnyExpr :: EvaluateAnyForLoci (RGDList& subObjects) const {

	// Later, recognize subObjects entries as DataSignal* and go through list

	DataSignal* q;
	RGDListIterator it (subObjects);

	while (q = (DataSignal*) it ()) {

		if (q->GetMessageValue (mIndex))
			return true;
	}

	return false;
}



AllExpr :: AllExpr (const RGString& name) : Expression (), mName (name),
mIndex (0), mScope (0) {

}


AllExpr :: ~AllExpr() {

}


Expression* AllExpr :: MakeCopy () {

	AllExpr* sm = new AllExpr (mName);
	sm->mIndex = mIndex;
	sm->mScope = mScope;
	return sm;
}


int AllExpr :: Initialize (int scope, const SmartMessage* sm) {

	mIndex = sm->FindMessageIndex (mName, mScope);

	if (mIndex < 0) {

		cout << "'All Expression':  Could not find message index for " << mName.GetData () << endl;
		return -1;
	}

	// Later we'll correct this to take account of sublists...

	if (mScope != scope - 1) {

		cout << "'All Expression':  Scope " << scope - 1 << " did not match expected scope " << mScope << " for " << mName.GetData () << endl;
		return -1;
	}

	return 0;
}


bool AllExpr :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	bool ans = false;

	switch (mScope) {

		case 4:
			ans = EvaluateAllForSamples (subObjects);
			break;

		case 3:
			ans = EvaluateAllForDataChannels (subObjects);
			break;

		case 2:
			ans = EvaluateAllForLoci (subObjects);
			break;

		default:
			return false;
	}

	return ans;
}


void AllExpr :: Print (const RGString& indent) {

	cout << indent.GetData () << "'All' expression with name:  " << mName.GetData () << endl;
}


	
size_t AllExpr :: StoreSize () const {

	return Expression::StoreSize () + mName.StoreSize ();
}


void AllExpr :: RestoreAll (RGFile& f) {

	Expression::RestoreAll (f);
	mName.RestoreAll (f);
}


void AllExpr :: RestoreAll (RGVInStream& s) {

	Expression::RestoreAll (s);
	mName.RestoreAll (s);
}


void AllExpr :: SaveAll (RGFile& f) const {

	Expression::SaveAll (f);
	mName.SaveAll (f);
}


void AllExpr :: SaveAll (RGVOutStream& s) const {

	Expression::SaveAll (s);
	mName.SaveAll (s);
}


bool AllExpr :: EvaluateAllForSamples (RGDList& subObjects) const {

	// Later, recognize subObjects entries as ChannelData* and go through list

	ChannelData* q;
	RGDListIterator it (subObjects);

	while (q = (ChannelData*) it ()) {

		if (!q->GetMessageValue (mIndex))
			return false;
	}

	return true;
}


bool AllExpr :: EvaluateAllForDataChannels (RGDList& subObjects) const {

	// Later, recognize subObjects entries as Locus* and go through list

	Locus* q;
	RGDListIterator it (subObjects);

	while (q = (Locus*) it ()) {

		if (!q->GetMessageValue (mIndex))
			return false;
	}

	return true;
}


bool AllExpr :: EvaluateAllForLoci (RGDList& subObjects) const {

	// Later, recognize subObjects entries as DataSignal* and go through list

	DataSignal* q;
	RGDListIterator it (subObjects);

	while (q = (DataSignal*) it ()) {

		if (!q->GetMessageValue (mIndex))
			return false;
	}

	return true;
}


ConstantExpr :: ConstantExpr (bool value) : Expression (), mValue (value) {

}


ConstantExpr :: ~ConstantExpr () {

}


Expression* ConstantExpr :: MakeCopy () {

	return new ConstantExpr (mValue);
}


int ConstantExpr :: Initialize (int scope, const SmartMessage* sm) {

	return 0;
}


bool ConstantExpr :: Evaluate (const bool* msgArray, RGDList& subObjects) const {

	return mValue;
}


bool ConstantExpr :: Evaluate (SmartMessagingComm& comm, int numHigherObjects) const {

	return mValue;
}


bool ConstantExpr :: EvaluateLocal (const bool* msgArray) const {

	return mValue;
}


void ConstantExpr :: Print (const RGString& indent) {

	cout << indent.GetData () << "Constant expression with value:  ";

	if (mValue)
		cout << "true" << endl;

	else
		cout << "false" << endl;
}

	
size_t ConstantExpr :: StoreSize () const {

	return Expression::StoreSize () + sizeof (int);
}


void ConstantExpr :: RestoreAll (RGFile& f) {

	Expression::RestoreAll (f);
	int temp;
	f.Read (temp);

	if (temp)
		mValue = true;

	else
		mValue = false;
}


void ConstantExpr :: RestoreAll (RGVInStream& s) {

	Expression::RestoreAll (s);
	int temp;
	s >> temp;

	if (temp)
		mValue = true;

	else
		mValue = false;
}


void ConstantExpr :: SaveAll (RGFile& f) const {

	int temp;

	if (mValue)
		temp = 1;

	else
		temp = 0;

	Expression::SaveAll (f);
	f.Write (temp);
}


void ConstantExpr :: SaveAll (RGVOutStream& s) const {

	int temp;

	if (mValue)
		temp = 1;

	else
		temp = 0;

	Expression::SaveAll (s);
	s << temp;
}


