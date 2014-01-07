// xmlwriter.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "xmlwriter.h"
#include <stdarg.h>

const char *xmlwriter::TABS = 
"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
const char *xmlwriter::getTabs(int n)
{
  n &= 31;
  const char *rtn = &TABS[32 - n];
  return rtn;
}

xmlwriter::xmlwriter(const RGString& sTmp)
{
  sXmlFile = sTmp;
  iLevel = 0;
  fp = new RGFile (sTmp, "w");
  temp.IncreaseSizeTo(128);
  s1.IncreaseSizeTo(4096);
  s2.IncreaseSizeTo(128);
  if (!fp->isValid ())
  {
		std::cout << "Unable to open output file" << endl;
		bClose = false;
		return;
	}
	else
	{
		bClose = true;
		beginFile();
	}
}


xmlwriter :: xmlwriter(RGFile *pF)
{
	fp = pF;
  iLevel = 0;
	bClose = false;
	beginFile();
}


void xmlwriter::beginFile()
{
	fp->Write ("<?xml version=\"1.0\" encoding=\"UTF-8\" \?>");
}


xmlwriter::~xmlwriter()
{
	if (bClose)
	{
		delete fp;
	}

	vectAttrData.clear();
}


void xmlwriter :: _CreateTag (const RGString& sTag)
{
	fp->Write ("\n");
	
	//Indent properly
  fp->Write (getTabs(iLevel));

	temp = "<" + sTag;

	//Add Attributes
	while(0 < vectAttrData.size()/2)
	{
		temp += " " + vectAttrData.back() + "=";
//		fprintf(fp," %s=", sTmp.c_str());
		vectAttrData.pop_back();
//		sTmp = vectAttrData.back();
		temp += "\"" + vectAttrData.back() + "\"";
//		fprintf(fp,"\"%s\"", sTmp.c_str());
		vectAttrData.pop_back();
	}

	vectAttrData.clear();
	temp += ">";
	fp->Write (temp);
//	fputs(">",fp);
	sTagStack.push (sTag);
	iLevel++;
}


void xmlwriter :: CloseLastTag ()
{
//	fputs("\n",fp);
	fp->Write ("\n");
	iLevel--;

    //Indent properly
  fp->Write(getTabs(iLevel));

	temp = "</" + sTagStack.top () + ">";
	fp->Write (temp);
//	fprintf(fp,"</%s>",sTagStack.top().c_str());
	sTagStack.pop();//pop out the last tag
	return;
}

void xmlwriter :: CloseAllTags ()
{
	while(sTagStack.size() != 0)
	{
//	   fputs("\n",fp);
	   fp->Write ("\n");
	   iLevel--;

     //Indent properly
	   fp->Write (getTabs(iLevel));

	   temp = "</" + sTagStack.top() + ">";
	   fp->Write (temp);
//	   fprintf(fp,"</%s>",sTagStack.top().c_str());
	   sTagStack.pop();//pop out the last tag
  }
	return;
}


void xmlwriter::_CreateChild(const RGString& sTag, const RGString& sValue)
{
//	fputs("\n",fp);
	fp->Write ("\n");

	//Indent properly
  fp->Write (getTabs(iLevel));
//		fputs("\t",fp);

	temp = "<" + sTag;
//	fprintf(fp,"<%s",sTag.c_str());

	//Add Attributes
	while(0 < vectAttrData.size()/2)
	{
//		string sTmp = vectAttrData.back();
		temp += " " + vectAttrData.back () + "=";
//		fprintf(fp," %s=", sTmp.c_str());
		vectAttrData.pop_back();
//		sTmp = vectAttrData.back();
		temp += "\"" + vectAttrData.back () + "\"";
//		fprintf(fp,"\"%s\"", sTmp.c_str());
		vectAttrData.pop_back();
	}
	vectAttrData.clear();
	//add value and close tag
	temp += ">" + sValue + "</" + sTag + ">";
	fp->Write (temp);
//	fprintf(fp,">%s</%s>",sValue.c_str(),sTag.c_str());
}

void xmlwriter::_AddAttributes(const RGString& sKey, const RGString& sVal)
{
	vectAttrData.push_back(sVal);
	vectAttrData.push_back(sKey);
}


void xmlwriter::_AddComment(const RGString& sComment)
{
//	fputs("\n",fp);
	fp->Write ("\n");

	//Indent properly
	fp->Write (getTabs(iLevel));
//		fputs("\t",fp);

	temp = "<!--" + sComment + "-->";
	fp->Write (temp);
//	fprintf(fp,"<!--%s-->",sComment.c_str());
}


RGString& xmlwriter::EscAscii(
  const RGString& s, RGString *pResult, bool bEscLow)
{
	char stmp[2] = {0,0};
  const int NTEMP = 2047;
  char sTemp[NTEMP + 1];
  const char *pStart = s.GetData();
  char *psDest = sTemp;
  char *psCheck = &sTemp[NTEMP - 32];
  size_t n = s.Length();

  *pResult = "";
  n += (n >> 1);
  pResult->IncreaseSizeTo (n);
	for (const char *psIn = pStart;
		*psIn;
		++psIn)
	{
		switch(*psIn)
		{
		case '<':
      strcpy(psDest,"&lt;");
      psDest += 4;
			break;

		case '>':
      strcpy(psDest,"&gt;");
      psDest += 4;
			break;

		case '"':
      strcpy(psDest,"&quot;");
      psDest += 6;
			break;

		case '&':
      strcpy(psDest,"&amp;");
      psDest += 5;
			break;

		default:
      if( ((*psIn) & 0x80) ||
          (bEscLow && ((*psIn) < 0x20))
        )
      {
        sprintf(psDest,"&#%d;",((int)(*psIn)) & 255);
        while(*psDest) {psDest++;}
      }
      else
      {
        *psDest = *psIn;
        psDest++;
      }
			break;
		}
    if(psDest >= psCheck)
    {
      *psDest = 0;
      pResult->Append(sTemp);
      psDest = sTemp;
    }
	}
  if(psDest > sTemp)
  {
    *psDest = 0;
    pResult->Append(sTemp);
  }
	return *pResult;
}


void xmlwriter :: CreateChild(const RGString& sTag, const RGString& sValue)
{
	EscAscii (sTag, &s2);
	EscAscii (sValue, &s1);
	_CreateChild (s2, s1);
}


void xmlwriter :: CreateChild (const RGString& sTag, int n)
{
	EscAscii (sTag, &s1);
	s2.Convert (n, 10);
//	sprintf(s,"%d",n);
//	string s2(s);
	_CreateChild (s1, s2);
}


void xmlwriter :: CreateChildUnsigned(const RGString& sTag, unsigned int n)
{
	EscAscii(sTag, &s1);
	s2.Convert ((unsigned long)n, 10);
//	sprintf(s,"%u",n);
//	string s2(s);
	_CreateChild (s1, s2);
}


void xmlwriter :: CreateTag(const RGString& sTag)
{
	_CreateTag(EscAscii (sTag,&s1));
}


void xmlwriter::AddAttributes(const RGString& sName, const RGString& sValue)
{
	EscAscii (sName, &s2);
	EscAscii (sValue, &s1);
	_AddAttributes (s2, s1);
}


void xmlwriter::AddComment(const RGString& sComment)
{
	EscAscii (sComment, &s1);
	RGString und("__");
	s1.FindAndReplaceAllSubstrings ("--", und);
	_AddComment (s1);
}
