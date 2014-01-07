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
*  FileName: Expression.h
*  Author:   Robert Goor
*
*/
// Declaration file for base class expression and subclasses.  These 
// classes perform the actual calculations in messages
//



#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "rgpersist.h"
#include "rgfile.h"
#include "rgvstream.h"
#include "rgdlist.h"
#include "rgstring.h"

#include "SmartMessage.h"

//#include <iostream>

//using namespace std;



const int _EXPRESSION_  =       2001;
const int _UNARY_       =       2002;
const int _BINARY_      =       2003;

const int _AND_         =       2004;
const int _OR_          =       2005;
const int _NOT_         =       2006;

//const int _LOGICALEQUAL_ =      2009;
//const int _GTEQUAL_     =       2010;
//const int _LTEQUAL_     =       2011;
//const int _NOTEQUAL_    =       2012;
//const int _LESSTHAN_    =       2013;
//const int _GREATERTHAN_ =       2014;

const int _EXCLUSIVEOR_      =  2015;
const int _SINGLEMESSAGE_ =		2016;
const int _ANY_           =		2017;
const int _ALL_           =		2018;
const int _CONSTANTEXPR_  =		2019;


//typedef void (*ppmf)();
PERSISTENT_PREDECLARATION (Expression)
PERSISTENT_PREDECLARATION (Unary)
PERSISTENT_PREDECLARATION (Binary)
PERSISTENT_PREDECLARATION (And)
PERSISTENT_PREDECLARATION (Or)
PERSISTENT_PREDECLARATION (Not)
//PERSISTENT_PREDECLARATION (LogicalEqual)
//PERSISTENT_PREDECLARATION (GTEqual)
//PERSISTENT_PREDECLARATION (LTEqual)
//PERSISTENT_PREDECLARATION (NotEqual)
//PERSISTENT_PREDECLARATION (LessThan)
//PERSISTENT_PREDECLARATION (GreaterThan)
PERSISTENT_PREDECLARATION (ExclusiveOr)
PERSISTENT_PREDECLARATION (SingleMessage)
PERSISTENT_PREDECLARATION (AnyExpr)
PERSISTENT_PREDECLARATION (AllExpr)
PERSISTENT_PREDECLARATION (ConstantExpr)


class ExpressionCleanUp {

public:
	ExpressionCleanUp () {};
	~ExpressionCleanUp ();
};



class Expression : public RGPersistent {

PERSISTENT_DECLARATION (Expression)

public:
	Expression() : RGPersistent () {}
	Expression (const Expression& ex);
	virtual ~Expression();

	virtual Expression* MakeCopy ();  
	virtual int Initialize (int scope, const SmartMessage* sm);
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateLocal (const bool* msgArray) const;

	virtual void Print (const RGString& indent) {}
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

	static Expression* Compile (const RGString& expressionString);
	static void SetCompiledExpression (Expression* ex) { finalExpression = ex; }
	static Expression* GetCompiledExpression () { return finalExpression; }
	static RGString GetExpressionCriterion () { return ExpressionCriterion; }
	static int CompileInput (char* Buffer, int MaxSize);
	static void CleanUpBuffers ();

protected:
	static Expression* finalExpression;
	static RGString ExpressionCriterion;
	static int RemainingCharacters;
	static ExpressionCleanUp CleanUp;
};



class Unary : public Expression {

PERSISTENT_DECLARATION (Unary)

public:
	Unary ();
	Unary (Expression* op);
	Unary (const Unary& un);
	~Unary();

	virtual Expression* MakeCopy ();	  
	virtual int Initialize (int scope, const SmartMessage* sm);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	Expression* mOperand;
};



class Binary : public Expression {

PERSISTENT_DECLARATION (Binary)

public:
	Binary ();
	Binary (Expression* op1, Expression* op2);
	Binary (const Binary& bin);
	~Binary();
	  
	virtual Expression* MakeCopy ();
	virtual int Initialize (int scope, const SmartMessage* sm);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	Expression* mOperand1;
	Expression* mOperand2;
};



class And : public Binary {

PERSISTENT_DECLARATION (And)

public:
	And ();
	And (Expression* op1, Expression* op2);
	~And();
	  
	virtual Expression* MakeCopy ();
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateLocal (const bool* msgArray) const;
	virtual void Print (const RGString& indent);

protected:

};



class Or : public Binary {

PERSISTENT_DECLARATION (Or)

public:
	Or ();
	Or (Expression* op1, Expression* op2);
	~Or();
	  
	virtual Expression* MakeCopy ();
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual void Print (const RGString& indent);
	virtual bool EvaluateLocal (const bool* msgArray) const;

protected:

};



//class Max : public Binary {
//
//PERSISTENT_DECLARATION (Max)
//
//public:
//  Max ();
//  Max (Expression* op1, Expression* op2);
//  ~Max();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void Maxintint (Expression&);
//  friend void Maxintdouble (Expression&);
//  friend void Maxdoubleint (Expression&);
//  friend void Maxdoubledouble (Expression&);
//
//};



//class Min : public Binary {
//
//PERSISTENT_DECLARATION (Min)
//
//public:
//  Min ();
//  Min (Expression* op1, Expression* op2);
//  ~Min();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void Minintint (Expression&);
//  friend void Minintdouble (Expression&);
//  friend void Mindoubleint (Expression&);
//  friend void Mindoubledouble (Expression&);
//
//};



class Not : public Unary {

PERSISTENT_DECLARATION (Not)

public:
	Not ();
	Not (Expression* op);
	~Not();
	  
	virtual Expression* MakeCopy ();
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateLocal (const bool* msgArray) const;
	virtual void Print (const RGString& indent);

protected:

};



//class LogicalEqual : public Binary {
//
//PERSISTENT_DECLARATION (LogicalEqual)
//
//public:
//  LogicalEqual ();
//  LogicalEqual (Expression* op1, Expression* op2);
//  ~LogicalEqual();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void Equalintint (Expression&);
//  friend void Equalintdouble (Expression&);
//  friend void Equaldoubleint (Expression&);
//  friend void Equaldoubledouble (Expression&);
//
//};
//
//
//
//class GTEqual : public Binary {
//
//PERSISTENT_DECLARATION (GTEqual)
//
//public:
//  GTEqual ();
//  GTEqual (Expression* op1, Expression* op2);
//  ~GTEqual();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void GTEqualintint (Expression&);
//  friend void GTEqualintdouble (Expression&);
//  friend void GTEqualdoubleint (Expression&);
//  friend void GTEqualdoubledouble (Expression&);
//
//};
//
//
//
//
//class LTEqual : public Binary {
//
//PERSISTENT_DECLARATION (LTEqual)
//
//public:
//  LTEqual ();
//  LTEqual (Expression* op1, Expression* op2);
//  ~LTEqual();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void LTEqualintint (Expression&);
//  friend void LTEqualintdouble (Expression&);
//  friend void LTEqualdoubleint (Expression&);
//  friend void LTEqualdoubledouble (Expression&);
//
//};
//
//
//
//
//class NotEqual : public Binary {
//
//PERSISTENT_DECLARATION (NotEqual)
//
//public:
//  NotEqual ();
//  NotEqual (Expression* op1, Expression* op2);
//  ~NotEqual();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void NotEqualintint (Expression&);
//  friend void NotEqualintdouble (Expression&);
//  friend void NotEqualdoubleint (Expression&);
//  friend void NotEqualdoubledouble (Expression&);
//
//};
//
//
//
//
//class LessThan : public Binary {
//
//PERSISTENT_DECLARATION (LessThan)
//
//public:
//  LessThan ();
//  LessThan (Expression* op1, Expression* op2);
//  ~LessThan();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void LessThanintint (Expression&);
//  friend void LessThanintdouble (Expression&);
//  friend void LessThandoubleint (Expression&);
//  friend void LessThandoubledouble (Expression&);
//
//};
//
//
//
//
//class GreaterThan : public Binary {
//
//PERSISTENT_DECLARATION (GreaterThan)
//
//public:
//  GreaterThan ();
//  GreaterThan (Expression* op1, Expression* op2);
//  ~GreaterThan();
//  
//  virtual int Initialize (int scope, const SmartMessage* sm);
//
//protected:
//  friend void GreaterThanintint (Expression&);
//  friend void GreaterThanintdouble (Expression&);
//  friend void GreaterThandoubleint (Expression&);
//  friend void GreaterThandoubledouble (Expression&);
//
//};



class ExclusiveOr : public Binary {

PERSISTENT_DECLARATION (ExclusiveOr)

public:
  ExclusiveOr ();
  ExclusiveOr (Expression* op1, Expression* op2);
  ~ExclusiveOr();
  
  virtual Expression* MakeCopy ();
  virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
  virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
  virtual bool EvaluateLocal (const bool* msgArray) const;
  virtual void Print (const RGString& indent);

protected:

};



class SingleMessage : public Expression {

PERSISTENT_DECLARATION (SingleMessage)

public:
	SingleMessage () : Expression () {}
	SingleMessage (const RGString& name);
	virtual ~SingleMessage();
  
	virtual Expression* MakeCopy ();
	virtual int Initialize (int scope, const SmartMessage* sm);
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateLocal (const bool* msgArray) const;
	virtual void Print (const RGString& indent);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	RGString mName;
	int mIndex;
	int mScope;
};


class AnyExpr : public Expression {

PERSISTENT_DECLARATION (AnyExpr)

public:
	AnyExpr () : Expression () {}
	AnyExpr (const RGString& name);
	virtual ~AnyExpr();
  
	virtual Expression* MakeCopy ();
	virtual int Initialize (int scope, const SmartMessage* sm);
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual void Print (const RGString& indent);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	RGString mName;
	int mIndex;
	int mScope;

	bool EvaluateAnyForSamples (RGDList& subObjects) const;
	bool EvaluateAnyForDataChannels (RGDList& subObjects) const;
	bool EvaluateAnyForLoci (RGDList& subObjects) const;
};


class AllExpr : public Expression {

PERSISTENT_DECLARATION (AllExpr)

public:
	AllExpr () : Expression () {}
	AllExpr (const RGString& name);
	virtual ~AllExpr();
  
	virtual Expression* MakeCopy ();
	virtual int Initialize (int scope, const SmartMessage* sm);
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual void Print (const RGString& indent);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	RGString mName;
	int mIndex;
	int mScope;

	bool EvaluateAllForSamples (RGDList& subObjects) const;
	bool EvaluateAllForDataChannels (RGDList& subObjects) const;
	bool EvaluateAllForLoci (RGDList& subObjects) const;
};



class ConstantExpr : public Expression {

	PERSISTENT_DECLARATION (ConstantExpr)

public:
	ConstantExpr () : Expression (), mValue (false) {}
	ConstantExpr (bool value);
	virtual ~ConstantExpr ();
  
	virtual Expression* MakeCopy ();
	virtual int Initialize (int scope, const SmartMessage* sm);
	virtual bool Evaluate (const bool* msgArray, RGDList& subObjects) const;
	virtual bool Evaluate (SmartMessagingComm& comm, int numHigherObjects) const;
	virtual bool EvaluateLocal (const bool* msgArray) const;
	virtual void Print (const RGString& indent);
	  
	virtual size_t StoreSize () const;
	virtual void RestoreAll (RGFile& f);
	virtual void RestoreAll (RGVInStream& s);
	virtual void SaveAll (RGFile& f) const;
	virtual void SaveAll (RGVOutStream& s) const;

protected:
	bool mValue;
};



#endif   /*  _EXPRESSION_H_  */





