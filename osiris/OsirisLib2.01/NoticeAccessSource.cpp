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
*  FileName: NoticeAccessSource.cpp
*  Author:   Robert Goor
*
*/
#include "RGTextOutput.h"
#include "RGLogBook.h"


	int AddNoticeToList (Notice* newNotice);
	Boolean IsNoticeInList (const Notice* target);
	Notice* GetNotice (const Notice* target);
	Boolean ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim);
	void ClearNoticeObjects ();
	int NumberOfNoticeObjects ();



	int mHighestSeverityLevel;
	int mHighestMessageLevel;

	RGDList NewNoticeList;


, mHighestSeverityLevel (-1), mHighestMessageLevel (-1)




int DataSignal :: AddNoticeToList (Notice* newNotice) {

	if (NewNoticeList.Entries () == 0) {

		mHighestSeverityLevel = newNotice->GetSeverityLevel ();
		mHighestMessageLevel = newNotice->GetMessageLevel ();
		NewNoticeList.Append (newNotice);
	}

	else {

		int temp = newNotice->GetSeverityLevel ();

		if (mHighestSeverityLevel >= temp) {

			mHighestSeverityLevel = temp;
			NewNoticeList.Prepend (newNotice);
		}

		else {

			RGDListIterator it (NewNoticeList);
			Notice* nextNotice;
			bool noticeInserted = false;

			while (nextNotice = (Notice*) it ()) {

				if (temp < nextNotice->GetSeverityLevel ()) {

					--it;
					it.InsertAfterCurrentItem (newNotice);
					noticeInserted = true;
					break;
				}
			}

			if (!noticeInserted)
				NewNoticeList.Append (newNotice);
		}

		temp = newNotice->GetMessageLevel ();

		if (mHighestMessageLevel > temp)
			mHighestMessageLevel = temp;
	}

	return NewNoticeList.Entries ();
}



Boolean DataSignal :: IsNoticeInList (const Notice* target) {

	if (NewNoticeList.Find (target))
		return TRUE;

	return FALSE;
}


Notice* DataSignal :: GetNotice (const Notice* target) {

	return (Notice*) NewNoticeList.Find (target);
}



Boolean DataSignal :: ReportNoticeObjects (RGTextOutput& text, const RGString& indent, const RGString& delim) {

	if (NewNoticeList.Entries () > 0) {

		RGDListIterator it (NewNoticeList);
		Notice* nextNotice;
		text.SetOutputLevel (mHighestMessageLevel);

		if (!text.TestCurrentLevel ()) {

			text.ResetOutputLevel ();
			return FALSE;
		}

		Endl endLine;
		text << endLine;
		text << indent << "Notices for curve with (Mean, Sigma, Peak, Fit) = " << delim << delim << delim << delim << delim;
		text << GetMean () << delim << GetStandardDeviation () << delim << Peak () << delim << Fit << endLine;
		text.ResetOutputLevel ();

		while (nextNotice = (Notice*) it ())
			nextNotice->Report (text, indent, delim);

		text.Write (1, "\n");
	}

	else
		return FALSE;

	return TRUE;
}


void DataSignal :: ClearNoticeObjects () {

	NewNoticeList.ClearAndDelete ();
}



int DataSignal :: NumberOfNoticeObjects () {

	return NewNoticeList.Entries ();
}


