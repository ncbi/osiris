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
*  FileName: RGLogBook.h
*  Author:   Robert Goor
*
*/
//
//  class RGLogBook automates the outputting of text strings, writing to a file specified by name or by RGFile; class
//  retains an output level input at instantiation and suppresses all output below that level.  Level 1 is the maximum
//  and level 1 messages are always output.  Formatting is up to the calling program.
//

#ifndef _LOGBOOK_H_
#define _LOGBOOK_H_

#include "RGTextOutput.h"
#include "rgfile.h"
#include "rgstring.h"
#include "rgdefs.h"
#include <list>




class RGLogBook : public RGTextOutput {

public:
	RGLogBook (const RGString& fileName, int level, Boolean echo = FALSE);
	RGLogBook (RGFile* ofile, int level, Boolean echo = FALSE);
	virtual ~RGLogBook ();

	virtual void SetOutputLevel (int level);
	virtual void ResetOutputLevel ();

	virtual Boolean Write (int level, const RGString& s);
	virtual Boolean Write (int level, const char* s);

	virtual RGTextOutput& operator<<(const RGString& s);
	virtual RGTextOutput& operator<<(const char* s);
	virtual RGTextOutput& operator<<(int i);
	virtual RGTextOutput& operator<<(double d);
	virtual RGTextOutput& operator<<(unsigned long i);
	virtual RGTextOutput& operator<<(char c);
	virtual RGTextOutput& operator<<(long l);
	virtual RGTextOutput& operator<<(short s);
	virtual RGTextOutput& operator<<(unsigned char c);
	virtual RGTextOutput& operator<<(unsigned int i);
	virtual RGTextOutput& operator<<(unsigned short s);
	virtual RGTextOutput& operator<<(const Endl& endLine);
	virtual RGTextOutput& operator<<(const CLevel& clevel);
	virtual RGTextOutput& operator<<(const PLevel& plevel);

	virtual Boolean TestCurrentLevel () const;

private:
	int Level;
	int CurrentOutputLevel;
	list<int> mLevels;
};

#endif  /*  _LOGBOOK_H_  */

