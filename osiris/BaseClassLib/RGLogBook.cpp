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
*  FileName: RGLogBook.cpp
*  Author:   Robert Goor
*
*/
//
//  class RGLogBook automates the outputting of text strings, writing to a file specified by name or by RGFile; class
//  retains an output level input at instantiation and suppresses all output below that level.  Level 1 is the maximum
//  and level 1 messages are always output.  Formatting is up to the calling program.
//

#include "RGLogBook.h"



RGLogBook :: RGLogBook (const RGString& fileName, int level, Boolean echo) : RGTextOutput (fileName, echo), Level (level),
CurrentOutputLevel (0) {

	if (level < 1)
		Level = 1;
}


RGLogBook :: RGLogBook (RGFile* ofile, int level, Boolean echo) : RGTextOutput (ofile, echo), Level (level), 
CurrentOutputLevel (0) {

	if (level < 1)
		Level = 1;
}


RGLogBook :: ~RGLogBook () {

	mLevels.clear ();
}


void RGLogBook :: SetOutputLevel (int level) {

	mLevels.push_front (CurrentOutputLevel);
	CurrentOutputLevel = level;
}


void RGLogBook :: ResetOutputLevel () {

	if (mLevels.empty ())
		CurrentOutputLevel = 0;

	else {

		CurrentOutputLevel = mLevels.front ();
		mLevels.pop_front ();
	}
}


Boolean RGLogBook :: Write (int level, const RGString& s) {

	if (level <= Level) {

		RGTextOutput::Write (s);
		return TRUE;
	}

	return FALSE;
}


Boolean RGLogBook :: Write (int level, const char* s) {

	if (level <= Level) {

		RGTextOutput::Write (s);
		return TRUE;
	}

	return FALSE;
}


RGTextOutput& RGLogBook :: operator<<(const RGString& s) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (s);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(const char* s) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (s);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(int i) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (i);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(double d) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (d);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(unsigned long i) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (i);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(char c) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (c);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(long l) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (l);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(short s) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (s);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(unsigned char c) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (c);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(unsigned int i) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (i);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(unsigned short s) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (s);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(const Endl& endLine) {

	if (TestCurrentLevel ())
		RGTextOutput::operator << (endLine);

	return *this;
}


RGTextOutput& RGLogBook :: operator<<(const CLevel& clevel) {

	SetOutputLevel (clevel.mCLevel);
	return *this;
}


RGTextOutput& RGLogBook :: operator<<(const PLevel& plevel) {

	ResetOutputLevel ();
	return *this;
}


Boolean RGLogBook :: TestCurrentLevel () const {

	if ((CurrentOutputLevel > 0) && (CurrentOutputLevel <= Level))
		return TRUE;

	return FALSE;
}


