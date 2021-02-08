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
*  FileName: LocusName.h
*  Author:   Robert Goor
*
*/
//
//     Abstract base class ChannelData for retaining data pertaining to one data channel, such as the sampled data
//  associated with that channel
//

#ifndef _LOCUSNAME_H_
#define _LOCUSNAME_H_

#include "rgstring.h"
#include "rgdlist.h"

class LocusName {

public:
	LocusName (const RGString& name);
	virtual ~LocusName ();
	RGString GetName () const;
	virtual bool CompareLC (const RGString& testName);

protected:
	RGString mName;
	RGString mLowerCaseName;
};

class SpecialName : public LocusName {

public:
	SpecialName (const RGString& name);
	virtual ~SpecialName ();
	virtual bool CompareLC (const RGString& testName);

protected:
	RGDList mSynonyms;
};


class AMELName : public SpecialName {
	
public:
	AMELName ();
	virtual ~AMELName () {}
};


class TH01Name : public SpecialName {

public:
	TH01Name ();
	virtual ~TH01Name () {}
};


class PentaE : public SpecialName {

public:
	PentaE ();
	virtual ~PentaE () {}
};

class PentaD : public SpecialName {

public:
	PentaD ();
	virtual ~PentaD () {}
};


class DYS389I : public SpecialName {

public:
	DYS389I ();
	virtual ~DYS389I () {}
};


class DYS389II : public SpecialName {

public:
	DYS389II ();
	virtual ~DYS389II () {}
};


class DYS385AB : public SpecialName {

public:
	DYS385AB ();
	virtual ~DYS385AB () {}
};


class YGATAH4 : public SpecialName {

public:
	YGATAH4 ();
	virtual ~YGATAH4 () {}
};


#endif /*  _LOCUSNAME_H_  */


