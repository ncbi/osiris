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
*  FileName: rgindexedlabel.cpp
*  Author:   Robert Goor
*
*/
//
//  class RGIndexedLabel is a persistent class that maintains an index for checking identity and for indexing into
//  arrays as well as two string objects, one as a name (or alias) and the other as text for description
//

#include "rgindexedlabel.h"
#include "rgfile.h"
#include "rgvstream.h"



RGINDEXEDCOMPARISON RGIndexedLabel::Comparison = RGINDEX;

PERSISTENT_DEFINITION (RGIndexedLabel, _RGINDEXEDLABEL_, "IndexedLabel")



RGIndexedLabel :: RGIndexedLabel () : RGDocument (), Index (0) {

}


RGIndexedLabel :: RGIndexedLabel (unsigned long ind) : RGDocument (), Index (ind) {

}


RGIndexedLabel :: RGIndexedLabel (const RGString& name, unsigned long ind) : RGDocument (name), Index (ind) {

}



RGIndexedLabel :: RGIndexedLabel (const RGString& name, const RGString& label, const RGString& text) : 
RGDocument (name), Index (0),
Label (label),
Text (text) {

}


RGIndexedLabel :: RGIndexedLabel (unsigned long ind, const RGString& label, const RGString& text) :
RGDocument (), Index (ind), Label (label), Text (text) {

}


RGIndexedLabel :: ~RGIndexedLabel () {

}


size_t RGIndexedLabel :: StoreSize () const {

	size_t size = RGDocument::StoreSize ();
	size += sizeof (unsigned long);
	size += Label.StoreSize ();
	size += Text.StoreSize ();
	return size;
}


int RGIndexedLabel :: CompareTo (const RGPersistent* p) const {

	RGIndexedLabel* q = (RGIndexedLabel*) p;
	int ReturnValue;
	unsigned comp = RGIndexedLabel::Comparison;

	if (RGDocument::CompareOnName)
		return RGDocument::CompareTo (q);

	switch (comp) {

		case RGINDEX:
			if (Index < q->Index)
				ReturnValue = -1;

			else if (Index > q->Index)
				ReturnValue = 1;

			else
				ReturnValue = 0;

			break;

		case RGLABEL:
			ReturnValue = Label.CompareTo (q->Label);
			break;

		case RGBOTH:
			if (Index < q->Index)
				ReturnValue = -1;

			else if (Index > q->Index)
				ReturnValue = 1;

			else
				ReturnValue = Label.CompareTo (q->Label);

			break;
	}

	return ReturnValue;
}



unsigned RGIndexedLabel :: HashNumber (unsigned long Base) const {

	return Index%Base;
}


Boolean RGIndexedLabel :: IsEqualTo (const RGPersistent* p) const {

	RGIndexedLabel* q = (RGIndexedLabel*) p;
	Boolean ReturnValue;

	if (RGDocument::CompareOnName)
		return RGDocument::IsEqualTo (q);

	switch (RGIndexedLabel::Comparison) {

		case RGINDEX:
			if (Index != q->Index)
				ReturnValue = FALSE;

			else
				ReturnValue = TRUE;

			break;

		case RGLABEL:
			ReturnValue = (Label == q->Label);
			break;

		case RGBOTH:
			if (Index != q->Index)
				ReturnValue = FALSE;

			else
				ReturnValue = (Label == q->Label);

			break;
	}

	return ReturnValue;
}


void RGIndexedLabel :: RestoreAll (RGFile& f) {

	RGDocument::RestoreAll (f);
	f.Read (Index);
	Label.RestoreAll (f);
	Text.RestoreAll (f);
}


void RGIndexedLabel :: RestoreAll (RGVInStream& vin) {

	RGDocument::RestoreAll (vin);
	vin >> Index;
	Label.RestoreAll (vin);
	Text.RestoreAll (vin);
}


void RGIndexedLabel :: SaveAll (RGFile& f) const {

	RGDocument::SaveAll (f);
	f.Write (Index);
	Label.SaveAll (f);
	Text.SaveAll (f);
}



void RGIndexedLabel :: SaveAll (RGVOutStream& vout) const {

	RGDocument::SaveAll (vout);
	vout << Index;
	Label.SaveAll (vout);
	Text.SaveAll (vout);
}


void RGIndexedLabel :: WriteXMLData (RGFile& file, const RGString& tabLevel) {

	RGDocument::WriteXMLData (file, tabLevel);
	RGString ind;
	ind.Convert (Index, 10);
	RGString Line (tabLevel + "\t<Index>" + ind + "</Index>");
	Line.WriteTextLine (file);
	Line = tabLevel + "\t<Information>" + Text + "</Information>";
	Line.WriteTextLine (file);
}

