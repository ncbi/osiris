//
//	Source file for LocusData class
//

#include "LocusData.h"
#include "rgtokenizer.h"



LocusData :: LocusData (const RGString& locusName, const RGString& alleleString) : mLocusName (locusName) {

	RGStringArray alleles;
	RGStringArray delimiters;

	if (alleleString.Length () == 0)
		return;

	RGString alleleStr = alleleString;
	RGStringTokenizer alleleJoin (alleleStr);
	alleleJoin.AddDelimiter (",");
	alleleJoin.Split (alleles, delimiters);
	int n = alleles.Length ();

	if (n == 0)
		return;

	int i;
	RGString* newAllele;
	cout << "Locus Name = " << (char*)mLocusName.GetData () << " with allele(s) ";

	for (i=0; i<n; i++) {

		newAllele = new RGString (alleles [i]);
		cout << (char*)newAllele->GetData () << "   ";
		mAlleleList.append (newAllele);
	}

	cout << endl;
}


LocusData :: ~LocusData () {

	mAlleleList.clearAndDestroy ();
}



bool LocusData :: AlleleStringIsEmpty () const {

	return (mAlleleList.entries () == 0);
}



RGString LocusData :: FormXMLString () {

	RGString Result;

	if (AlleleStringIsEmpty ())
		return Result;

	RGTPtrDList<RGString> temp;
	RGString sixTabs = "\t\t\t\t\t\t";
	RGString sevenTabs = "\t\t\t\t\t\t\t";
	RGString eightTabs = "\t\t\t\t\t\t\t\t";
	Result << sixTabs << "<Locus>\n";
	Result << sevenTabs << "<Name>" << (char*)mLocusName.GetData () << "</Name>\n";
	RGString* nextAllele;

	while (mAlleleList.entries () > 0) {

		nextAllele = mAlleleList.RemoveFirst ();
		temp.append (nextAllele);
		Result << sevenTabs << "<Allele>\n";
		Result << eightTabs << "<Name>" << (char*)nextAllele->GetData () << "</Name>\n";
		Result << eightTabs << "<BioID>0</BioID>\n";
		Result << sevenTabs << "</Allele>\n";
	}

	Result << sixTabs << "</Locus>\n";

	//cout << (char*) Result.GetData ();

	while (temp.entries () > 0) {

		nextAllele = temp.RemoveFirst ();
		mAlleleList.append (nextAllele);
	}

	return Result;
}