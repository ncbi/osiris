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
*  FileName: rgindexedlabel.h
*  Author:   Robert Goor
*
*/
//
//  class RGIndexedLabel is a persistent class that maintains an index for checking identity and for indexing into
//  arrays as well as two string objects, one as a name (or alias) and the other as text for description
//

#ifndef _RGINDEXEDLABEL_H_
#define _RGINDEXEDLABEL_H_

#include "rgpersist.h"
#include "rgstring.h"
#include "rgdefs.h"
#include "rgtree.h"

class RGFile;
class RGVInStream;
class RGVOutStream;

const int _RGINDEXEDLABEL_ = 9;

enum RGINDEXEDCOMPARISON {RGINDEX, RGLABEL, RGBOTH};


PERSISTENT_PREDECLARATION (RGIndexedLabel)


class RGIndexedLabel : public RGDocument {

PERSISTENT_DECLARATION (RGIndexedLabel)

public:
	RGIndexedLabel ();
	RGIndexedLabel (unsigned long ind);
	RGIndexedLabel (const RGString& name, unsigned long ind);
	RGIndexedLabel (const RGString& name, const RGString& label, const RGString& text);
	RGIndexedLabel (unsigned long ind, const RGString& label, const RGString& text);
	~RGIndexedLabel ();

	void SetIndex (unsigned long ind) { Index = ind; }
	void SetLabel (const RGString& label) { Label = label; }
	void SetLabel (const char* label) { Label = label; }
	void SetText (const RGString& txt) { Text = txt; }
	void SetText (const char* txt) { Text = txt; }

	unsigned long GetIndex () const { return Index; }
	RGString GetLabel () const { return Label; }
	RGString GetText () const { return Text; }

	static void SetComparisonToIndex () { Comparison = RGINDEX; RGDocument::SetCompareOnName (FALSE); }
	static void SetComparisonToLabel () { Comparison = RGLABEL; }
	static void SetComparisonToBoth () { Comparison = RGBOTH; }

	virtual size_t StoreSize () const;
	virtual int CompareTo (const RGPersistent*) const;
	virtual unsigned HashNumber (unsigned long Base) const;
	virtual Boolean IsEqualTo (const RGPersistent*) const;

	virtual void RestoreAll (RGFile&);
	virtual void RestoreAll (RGVInStream&);
	virtual void SaveAll (RGFile&) const;
	virtual void SaveAll (RGVOutStream&) const;

	virtual void WriteXMLData (RGFile& file, const RGString& tabLevel);

protected:
	unsigned long Index;
	RGString Label;
	RGString Text;

	static RGINDEXEDCOMPARISON Comparison;
};

#endif  /*  _RGINDEXEDLABEL_H_  */
