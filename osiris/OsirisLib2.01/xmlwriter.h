// downloaded from
// http://www.codeproject.com/useritems/XML_writer.asp
//
// this is a temporary piece of crap
//
#ifndef xmlwriter_h
#define xmlwriter_h

#include "rgstring.h"
#include "rgfile.h"
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
typedef stack <RGString> StackStrings;


class xmlwriter
{
public:
	xmlwriter(const RGString& sTmp);
	xmlwriter(RGFile* pF);
	virtual ~xmlwriter();
	void CloseLastTag();
	void CloseAllTags();
	void CreateChild(const RGString& sTag, const RGString& sValue);
	void CreateChild(const RGString& sTag, int n);
	void CreateChildUnsigned(const RGString& sTag, unsigned int n);
	void CreateTag(const RGString& sTag);
	void AddAttributes(const RGString& sAttrName, const RGString& sAttrvalue);
	void AddComment(const RGString& sComment);

	static RGString& EscAscii 
    (const RGString& s, RGString* pResult,bool bEscLow = false, bool bAnsi = false);
  // if bAnsi is false, the string is assumed to be UTF-8
  // if 8 bit characters are found and it is not valid UTF-8
  // it will be processed as ANSI

private:
	//  djh1
	//
	// modified the names of the next 4 functions
	// moved to private, created wrappers (above)
	// which call EscAscii on 
	// the string arguments
	//
  static int _DecodeUTF8(const char *ps, int *pnBytes);
	void _CreateChild(const RGString& sTag, const RGString& sValue);
	void _CreateTag(const RGString& sTag);
	void _AddAttributes(const RGString& sAttrName, const RGString& sAttrvalue);
	void _AddComment(const RGString& sComment);
	void beginFile();
	RGString sXmlFile;
  RGString s1;
  RGString s2;
  RGString temp;
	vector<RGString> vectAttrData;
	RGFile* fp;
	bool bClose;
	int iLevel;
	StackStrings sTagStack;
	
  static const char *getTabs(int n);
  static const char *TABS;
};

#endif // xmlwriter_h


