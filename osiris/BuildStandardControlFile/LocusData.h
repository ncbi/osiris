//
//	class to retain locus data, including locus name and list of allele names
//

#ifndef _LOCUSDATA_H_
#define _LOCUSDATA_H_

#include "rgstring.h"
#include "rgtptrdlist.h"
#include <iostream>
#include <tchar.h>

using namespace std;

class LocusData {

public:
	LocusData (const RGString& locusName, const RGString& alleleString);
	~LocusData ();

	bool AlleleStringIsEmpty () const;
	RGString FormXMLString ();

protected:
	RGString mLocusName;
	RGTPtrDList<RGString> mAlleleList;
};


#endif /*  _LOCUSDATA_H_  */


