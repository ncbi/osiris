#!/usr/bin/perl
#
# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#               National Center for Biotechnology Information
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government have not placed any restriction on its use or reproduction.
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
#  Please cite the author in any work or product based on this material.
#
# ===========================================================================
#
#  FileName: CParmOsiris.pl
#            perl script to generate CParmOsiris.{cpp,h}
#            which is the object for saving and restoring osiris
#            user parameters
#  Author:   Douglas Hoffman
#
#
use strict 'vars';
#
#  VARLIST elments
#
#  [0] variable name
#  [1] variable type
#     wxString
#     int
#     unsigned int
#     double
#     bool
#     bool true -- bool with a default of true
#  [2] default value, if undef,
#          wxString = empty
#          int = 0,
#          unsigned int, - currently uses its IO class for default
#          double = 0.0
#          bool = false,
#          bool true = true
#
#  [3] register (XML tag) name, optional, can be generated from [0]
#  [4] IO type for register, optional, can be generated from [1]
#
#  if only one item, then it is a comment
#
my $VARLIST =
[
  ["m_sLastExportXSLsearch", "wxString"],
  ["m_sLastExportDirectory", "wxString"],
  ["m_sInputDirectory", "wxString", "sInputDirectory", "inputDirectory"],
  ["m_sOutputDirectory", "wxString", "pConfig->GetFilePath()", "outputDirectory"],
  [" m_sKitName is obsolete, use volume name"],
  ["m_sKitName", "wxString","\"Cofiler\"","kit"],
  ["m_sVolumeName", "wxString",undef,"volume"],
  ["m_sLsName", "wxString",undef,"ls"],
  ["m_nMinRFU_Sample", "int", "RFU", "minRFUsample"],
  ["m_nMinRFU_Interlocus", "int", "RFU", "minRFUinterlocus"],
  ["m_nMinRFU_ILS", "int", "RFU", "minRFUILS"],
  ["m_nMinRFU_Ladder", "int", "RFU", "minRFUladder"],
  ["m_nMinLadderInterlocusRFU", "int", "-1", "minRFUladderInterlocus"],
  ["m_nSampleDetectionThreshold", "int", "-1", "minRFUsampleDetection"],
  ["m_sAnalysisOverride","wxString"],
  
  ##  this is awful - putting channel RFU and Detection in arrays
  
  ["m_anChannelRFU","vector<int>","-1"],
  ["m_anChannelDetection","vector<int>","-1"],

  ["m_bTimeStampSubDir", "bool"],
  ["m_bDataAnalyzed", "bool"],
  ["CMF settings"],
  ["m_sCMFsourceLab", "wxString"],
  ["m_sCMFdestLab", "wxString"],
  ["m_sCMFdefaultSample", "wxString",undef,undef,"m_ioDefaultSample"],
  ["m_sCMFbatchFormat", "wxString","DEFAULT_BATCH_FORMAT",undef,"m_ioBatchFormat"],
  ["m_sCMFuserID", "wxString","wxGetUserId()"],
  ["m_bCMFviewLocation", "bool"],
  ["m_bShowFileLocationDir","bool"],

  ["PNG Export Settings"],

  ["m_dPNGwidth", "double","1024.0"],
  ["m_dPNGheight", "double","768.0"],
  ["m_nPNGunits", "int"],
  ["m_nPNGusage", "int"],
  ["m_bPNGviewLocation", "bool"],

  ["m_nShowAlerts","int","-1",undef,"m_ioInt_1"],
  ["m_bShowPreview", "bool true"],
  ["m_bHideGraphicToolbar", "bool"],
  ["m_bHideGraphicScrollbar", "bool"],
  ["m_bHideTextToolbar", "bool"],
  ["m_bHideSampleToolbar", "bool"],
  ["m_bStartupMRU", "bool true"],
  ["m_bCheckBeforeExit", "bool true"],
  ["m_bWarnOnHistory", "bool true"],
  ["m_dZoomLocusMargin", "double"],

  ["grid"],
##  m_bTableSortBySeverity is deprecated

  ["m_nTableDisplayPeak", "int"],
  ["m_nTableSortBy", "int"],
  ["m_bTableControlsAtTop","bool true"],
##  ["m_bTableSortBySeverity", "bool"],
  ["m_nTableShowSampleDisplayNames", "int"],

  ["preview settings"],

  ["m_bPreviewDataAnalyzed", "bool true"],
  ["m_bPreviewDataRaw", "bool"],
  ["m_bPreviewDataLadder", "bool"],
  ["m_bPreviewDataBaseline", "bool"],
  ["m_bPreviewShowILS", "bool"],
  ["m_bPreviewShowRFU", "bool"],
  ["m_bPreviewShowLadderLabels", "bool"],
  ["m_bPreviewXBPS", "bool"],
  ["m_nPreviewShowArtifact", "int","m_ioIntViewPlotArtifact.GetDefault()","PreviewArtifact","m_ioIntViewPlotArtifact"],

  ["plot settings"],

  ["m_bPlotDataAnalyzed", "bool true"],
  ["m_bPlotDataRaw", "bool"],
  ["m_bPlotDataLadder", "bool"],
  ["m_bPlotDataBaseline", "bool"],
  ["m_bPlotDataXBPS", "bool"],
  ["m_bPlotShowILS", "bool"],
  ["m_bPlotShowRFU", "bool"],
  ["m_bPlotShowLadderLabels", "bool"],
  ["m_bPlotResizable", "bool true"],
  ["m_nPlotMinHeight", "int","-1",undef,"m_ioInt_1"],
  ["m_nPlotShowArtifact", "int","m_ioIntViewPlotArtifact.GetDefault()",undef,"m_ioIntViewPlotArtifact"],
  ["m_anPlotDisplayPeak", "vector<unsigned int>","1"],
  ["m_nPlotMaxLadderLabels", "int","-1","MaxLadderLabels","m_ioInt_1"],

  ["plot print settings for analysis printout"],
  
  # peaks

  ["m_bPrintCurveAnalyzed", "bool true"],
  ["m_bPrintCurveRaw", "bool"],
  ["m_bPrintCurveLadder", "bool"],
  ["m_bPrintCurveLadderLabels", "bool"],
  ["m_bPrintCurveBaseline", "bool"],
  
  ["m_bPrintCurveILSvertical", "bool"],
  ["m_bPrintCurveMinRFU", "bool"],

  # X-Axis
  ["m_bPrintXaxisILSBPS", "bool"],
  # true - ILS BPS, false - time

  
  ["m_nPrintXscale", "int", "0"],
  # 0 - scale w/o primer peak, 1 - scale to 0, 2 - scale to 0 neg control, 3 - specify
  ["m_nPrintXscaleMin", "int", "0"],
  ["m_nPrintXscaleMax", "int", "20000"],
  ["m_nPrintXscaleMinBPS", "int", "0"],
  ["m_nPrintXscaleMaxBPS", "int", "600"],

  ["m_nPrintYscale", "int", "0"],
  # 0 - individual channel, 1 - zoom all to tallest channel, 2 - user specified
  ["m_nPrintYscaleMin", "int", "-200"],
  ["m_nPrintYscaleMax", "int", "6000"],
  ["m_nPrintYcaleNegCtrl", "int","0"],
  # 0 - scale to peaks, 1 - include min RFU, 2 - scale to ILS

  # allele labels
  ["m_anPrintLabelsPeak", "vector<unsigned int>","1"],
  
  # Artifact labels
  ["m_nPrintArtifact", "int","m_ioIntViewPlotArtifact.GetDefault()",undef,"m_ioIntViewPlotArtifact"],

  # Page Heading
  ["m_bPrintHeading", "int", "0"],
  # 0 - File name; 1 - Sample Name
  ["m_sPrintHeadingNotes", "wxString"],
  
  # Channels per page
  ["m_nPrintChannelsSamples", "int", "8"],
  ["m_nPrintChannelsLadders", "int", "8"],
  ["m_nPrintChannelsNegCtrl", "int", "8"],
  ["m_bPrintChannelsOmitILS", "bool"],
  
  # Samples
  ["m_bPrintSamplesLadders", "bool true"],
  ["m_bPrintSamplesPosCtrl", "bool true"],
  ["m_bPrintSamplesNegCtrl", "bool true"],
  ["m_bPrintSamplesDisabled", "bool"],

  # colors
  ["m_nPrintColorRed","int","100"],
  ["m_nPrintColorGreen","int","100"],
  ["m_nPrintColorBlue","int","100"],
  ["m_nPrintColorYellow","int","100"],
  ["m_nPrintColorOrange","int","100"],
  ["m_nPrintColorPurple","int","100"],
  ["m_nPrintColorGray","int","100"],

  ["plot printout -- margins are in millimeters"],

  ["m_nPrintPlotMarginTop", "unsigned int", "25"],
  ["m_nPrintPlotMarginBottom", "unsigned int", "25"],
  ["m_nPrintPlotMarginLeft", "unsigned int", "25"],
  ["m_nPrintPlotMarginRight", "unsigned int", "25"],
  ["m_nPrintPlotPaperType", "int", "-1"],
  ["m_nPrintPlotPaperWidth", "int", "-1"],
  ["m_nPrintPlotPaperHeight", "int", "-1"],
  ["m_bPrintPlotLandscape", "bool"],
  ["m_nPrintPreviewZoom", "int", "-1"],

  ["XSLT saved parameter info"],

  ["m_sLastXSLInputFileDir","wxString"],

  # misc
  
  ["m_bPrivacySeen", "bool"]

];

#
#  following is from CVolumes.h
#
###  #define VOLUME_STRING "Operating Procedure"
#
my $VOLUME_STRING = "Operating Procedure";

my $STATIC_STRINGS =
[
  ["DEFAULT_BATCH_FORMAT", "%Y%m%d_%H%M%S"],
  ["LABEL_VIEW_LOCATION", "View File Location"],

  ["LABEL_ORIGIN", "Origin "],
  ["LABEL_INPUT", "Input Directory "],
  ["LABEL_OUTPUT", "Output Directory "],
  ["LABEL_KIT", "Kit Name "],
  ["LABEL_VOLUME", "${VOLUME_STRING} Name "],
  ["LABEL_LS", "Internal Lane Standard "],
  ["LABEL_RFU", "Minimum RFU "],
  ["LABEL_RFU_SAMPLE", "Sample: "],
  ["LABEL_RFU_ILS", "ILS: "],
  ["LABEL_RFU_LADDER", "Ladder: "],
  ["LABEL_RFU_INTERLOCUS", "Interlocus: "],
  ["LABEL_DATA", "Data "],
  ["LABEL_DATA_RAW", "Raw  "],
  ["LABEL_DATA_ANALYZED", "Analyzed"],
  ["LABEL_ANALYSIS", "Analysis: "],
  ["LABEL_DETECTION", "Detection: "],
  ["LABEL_INTERLOCUS", "Interlocus: "],
  ["ERROR_MSG_INTERLOCUS","Minimum interlocus RFU must be\\ngreater than or equal to the minimum RFU"]

];

sub GenerateStringsHeader
{
  my $sRtn = "";
  my $s;
  for my $aStr (@$STATIC_STRINGS)
  {
    $s = $aStr->[0];
    $sRtn .= <<EOF;
  static const wxString ${s};
EOF
  }
  $sRtn;
}
sub GenerateStringsCPP
{
  my $sRtn = "";
  for my $aStr (@$STATIC_STRINGS)
  {
    my ($s0,$s1) = @$aStr;
    $sRtn .= <<EOF
const wxString CParmOsiris::${s0}("${s1}");
EOF
  }
  $sRtn;
}

my $hArgs =
{
 "wxString" => "const wxString &",
 "bool true" => "bool ",
 "vector<int>" => "const vector<int> &",
 "vector<unsigned int>" => "const vector<unsigned int> &"
};

sub GetVarType
{
  my $s = shift;
  my $sRtn = $hArgs->{$s};
  $sRtn || ($sRtn = "${s} ");
  $sRtn;

}

sub comment
{
  my $sComment = shift;
  my $sRtn = "\n  //  ${sComment}\n\n";
  $sRtn;
}


sub GenerateCopyOrCompare
{
  my $EOL = shift; ## empty for compare, semi-colon for copy
  my $sRtn = "";
  for my $a (@$VARLIST)
  {
    my $s = $a->[0];
    if($#$a > 0)
    {
      $sRtn .= "  CP(${s})${EOL}\n";
    }
    else
    {
      $sRtn .= &comment($s);
    }
  }
  $sRtn;
}

sub chopPrefix
{
  my $s = shift;
  $s =~ s/^m_[a-z]{1,2}//;
  $s;
}

sub GenerateRegister
{
  my $sRtn = "";
  my $a;
  my $VarTypeToFunction =
  {
  "bool true" => "RegisterBoolTrue",
  "bool" => "RegisterBool",
  "int" => "RegisterInt",
  "unsigned int" => "RegisterUint",
  "double" => "RegisterDouble",
  "wxString" => "RegisterWxString",
  "vector<int>" => "RegisterIntVector",
  "vector<unsigned int>" => "RegisterUintVector"
  };
  for $a (@$VARLIST)
  {
    if($#$a > 0)
    {
      my ($sVarName,$sVarType,$sDefaultValue, $sTagName,$sIOvariable) = @$a;
      if(!$sTagName)
      {
        $sTagName = &chopPrefix($sVarName);
      }
      if($sIOvariable)
      {
        $sRtn .=
        "  Register(\"${sTagName}\",\&${sIOvariable},(void *) \&${sVarName});\n";
      }
      else
      {
        my $fnc = $VarTypeToFunction->{$sVarType};
        $fnc || die("Cannot find register function for ${sVarType}");
        $sRtn .= "  ${fnc}(\"${sTagName}\", \&${sVarName});\n";
      }
    }
    else
    {
      my $sComment = $a->[0];
      $sRtn .= &comment($sComment);

    }
  }
  $sRtn;
}


sub GenerateDefaults
{
  my $DEFAULTS =
  {
    "int" => "0",
    "double" => "0.0",
    "bool" => "false",
    "bool true" => "true"
  };
  my $sRtn = "";
  for my $s (@$VARLIST)
  {
    my ($sVarName,$sVarType,$sDefaultValue, $sTagName,$sIOvariable) = @$s;
    if(!defined($sVarType))
    {
      $sRtn .= &comment($sVarName);
    }
    elsif( ($sVarType eq "wxString") && !defined($sDefaultValue) )
    {
      $sRtn .= "  ${sVarName}.Empty();\n";
    }
    elsif($sVarType =~ m/vector<[\w ]+>$/)
    {
      $sRtn .= "  ${sVarName}.clear();\n";
      if(defined($sDefaultValue))
      {
        $sDefaultValue =~ s/[\s,]+/ /g;
        $sDefaultValue =~ s/^ | $//g;
        my @as = split / /,$sDefaultValue;
        for my $ss (@as)
        {
          $sRtn .= "  ${sVarName}.push_back($ss);\n";
        }
      }
    }
    else
    {
      defined($sDefaultValue) || ($sDefaultValue = $DEFAULTS->{$sVarType});
      defined($sDefaultValue) ||
        die("Cannot find default value for: ${sVarType} ${sVarName}");
      $sRtn .= "  ${sVarName} = ${sDefaultValue};\n";
    }
  }
  $sRtn;
}

sub GenerateSet
{
  my $sRtn = "";
  my $argNames =
  {
    "int" => "n",
    "double" => "d",
    "unsigned int" => "n",
    "wxString" => "s",
    "bool" => "b",
    "bool true" => "b",
    "vector<int>" => "an",
    "vector<unsigned int>" => "an"
  };
  for my $a (@$VARLIST)
  {
    if($#$a == 0)
    {
      $sRtn .= &comment($a->[0]);
    }
    else
    {
      my ($sVarName,$sVarType,$sDefaultValue, $sTagName,$sIOvariable) = @$a;
      my $arg = &GetVarType($sVarType);
      my $fnc = "Set" . &chopPrefix($sVarName);
      my $argName = $argNames->{$sVarType};
      $argName || die("Cannot find arg name for ${sVarType}");

      $sRtn .= <<EOF;
  void ${fnc}(${arg}${argName})
  {
    __SET_VALUE(${sVarName},${argName});
  }
EOF
    }
  }
  $sRtn;
}
sub GenerateGet
{
  my $sRtn = "";
  for my $a (@$VARLIST)
  {
    if($#$a == 0)
    {
      $sRtn .= &comment($a->[0]);
    }
    else
    {
      my ($sVarName,$sVarType,$sDefaultValue, $sTagName,$sIOvariable) = @$a;
      my $sReturn = &GetVarType($sVarType);
      my $fnc = "Get" . &chopPrefix($sVarName);
      $sRtn .= <<EOF;
  ${sReturn}${fnc}() const
  {
    return ${sVarName};
  }
EOF
    }
  }
  $sRtn;
}
sub GenerateVars
{
  my $sRtn = "";
  for my $a (@$VARLIST)
  {
    if($#$a == 0)
    {
      $sRtn .= &comment($a->[0]);
    }
    else
    {
      my ($sVarName,$sVarType,$sDefaultValue, $sTagName,$sIOvariable) = @$a;
      $sVarType =~ s/ true$//;
      $sRtn .= "  ${sVarType} ${sVarName};\n";
    }
  }
  $sRtn;
}


sub GenFiles
{

  my $sVars = &GenerateVars;
  my $sGets = &GenerateGet;
  my $sSets = &GenerateSet;
  my $sDefaults = &GenerateDefaults;
  my $sRegister = &GenerateRegister;
  my $sCopy = &GenerateCopyOrCompare(";");
  my $sCompare = &GenerateCopyOrCompare("");
  my $sStringHeader = &GenerateStringsHeader;
  my $sStringCPP = &GenerateStringsCPP;
  my $sCOPY = <<EOF1;

  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }
EOF1
  my $fileH = <<EOF;
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
*  FileName: CParmOsiris.h
*  Author:   this file was generated by CParmOsiris.pl
*            a perl script written by Douglas Hoffman
*
*/
#ifndef __C_PARM_OSIRIS_H__
#define __C_PARM_OSIRIS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxGlobalObject.h"
#include "ConfigDir.h"


#include "CParmGridAttributes.h"

class CParmOsiris : public nwxXmlPersist
{
public:
  CParmOsiris()
  {
    _Init();
  }
  CParmOsiris(const wxString &sFileName, bool bAutoSave = false)
  {
    m_sFileName = sFileName;
    _Init();
    m_bAutoSave = bAutoSave;
  }
  CParmOsiris(bool bAutoSave)
  {
    _Init();
    m_bAutoSave = bAutoSave;
  }
  CParmOsiris(const CParmOsiris &x)
  {
    (*this) = x;
    RegisterAll(true);
  }

  virtual ~CParmOsiris();

  const wxString &GetFileName() const
  {
    return m_sFileName;
  }

  void SetAutoSave(bool b = true)
  {
    m_bAutoSave = b;
  }
  bool IsAutoSave()
  {
    return m_bAutoSave;
  }
  bool IsModified()
  {
    return m_bModified;
  }
  CParmOsiris &operator = (const CParmOsiris &x);
  bool IsEqual(const CParmOsiris &x) const;
  bool operator == (const CParmOsiris &x) const
  {
    bool bEQ = IsEqual(x);
    return bEQ;
  }
  bool operator != (const CParmOsiris &x) const
  {
    bool bEQ = IsEqual(x);
    return !bEQ;
  }
  virtual bool Load()
  {
    bool b = LoadFile(m_sFileName);
    return b;
  }
  virtual bool Save();
  bool SaveIfModified()
  {
    bool bRtn = m_bModified;
    if(m_bModified)
    {
      bRtn = Save();
    }
    return bRtn;
  }

  // begin generated Get

${sGets}

  // end generated Get


  int GetMaxLadderLabels() const
  {
    return GetPlotMaxLadderLabels(); // backward compatibility
  }
  bool WarnOnHistory() const
  {
    return GetWarnOnHistory(); // backward compatibility
  }
  bool CheckBeforeExit() const
  {
    return GetCheckBeforeExit(); // backward compatibility
  }
  wxString GetVolumeOrKit() const
  {
    wxString sRtn(m_sVolumeName);
    if(sRtn.IsEmpty() && !m_sKitName.IsEmpty())
    {
      sRtn = "[";
      sRtn.Append(m_sKitName);
      sRtn.Append("]");
    }
    return sRtn;
  }
  int GetPrintColorByName(const wxString &sName) const;
  static const wxString NO_INIT;
  // end static/global stuff

private:

  // SET VALUES

  void __SET_VALUE(wxString &s1, const wxString &s2)${sCOPY}
  void __SET_VALUE(double &s1, double s2)${sCOPY}
  void __SET_VALUE(bool &s1, bool s2)${sCOPY}
  void __SET_VALUE(int &s1, int s2)${sCOPY}
  void __SET_VALUE(unsigned int &s1, unsigned int s2)${sCOPY}
  void __SET_VALUE(vector<int> &s1, const vector<int> &s2)${sCOPY}
  void __SET_VALUE(vector<unsigned int> &s1, const vector<unsigned int> &s2)${sCOPY}

public:

  // begin set

${sSets}

  // end set

  void SetPlotFixed(bool b)
  {
    SetPlotResizable(!b);
    if(b)
    {
      SetPlotMinHeight(-1);
    }
  }
  void SetMaxLadderLabels(int n)
  {
    SetPlotMaxLadderLabels(n); // backward compatibility
  }
private:
  void _CheckMod(bool b)
  {
    if(b) { m_bModified = true; }
  }
public:
  static bool IsAttrBold(int n)
  {
    return !!(n & (int)GRID_BOLD);
  }
  static bool IsAttrItalic(int n)
  {
    return !!(n & (int)GRID_ITALIC);
  }
  static bool IsAttrReverse(int n)
  {
    return !!(n & (int)GRID_REVERSE);
  }
  bool SetAttrNeedsAttn(int n)
  {
    bool bRtn = m_gridAttr.SetAttrNeedsAttn(n);
    _CheckMod(bRtn);
    return bRtn;
  }
  bool SetAttrEdited(int n)
  {
    bool bRtn = m_gridAttr.SetAttrEdited(n);
    _CheckMod(bRtn);
    return bRtn;
  }
  bool SetDefaultColours()
  {
    bool bRtn = m_gridAttr.SetDefaultColours();
    _CheckMod(bRtn);
    return bRtn;
  }
  void SetBackground(int nGRID_FLAG, const wxColour &c)
  {
    bool bRtn = m_gridAttr.SetBackground(nGRID_FLAG,c);
    _CheckMod(bRtn);
  }
  void SetForeground(int nGRID_FLAG, const wxColour &c)
  {
    bool bRtn = m_gridAttr.SetForeground(nGRID_FLAG,c);
    _CheckMod(bRtn);
  }
  void SetNormalForeground(const wxColour &c)
  {
    SetForeground(GRID_NORMAL,c);
  }
  void SetAlertForeground(const wxColour &c)
  {
    SetForeground(GRID_ALERT_CELL,c);
  }
  void SetDisabledForeground(const wxColour &c)
  {
    SetForeground(GRID_DISABLED,c);
  }
  void SetStatusForeground(const wxColour &c, bool bNeedsAttn)
  {
    bool bRtn = m_gridAttr.SetStatusForeground(c,bNeedsAttn);
    _CheckMod(bRtn);
  }
  void SetStatusBackground(const wxColour &c, bool bNeedsAttn)
  {
    bool bRtn = m_gridAttr.SetStatusBackground(c,bNeedsAttn);
    _CheckMod(bRtn);
  }
  const wxColour &GetBackground(int nGRID_FLAG) const
  {
    return m_gridAttr.GetBackground(nGRID_FLAG);
  }
  const wxColour &GetForeground(int nGRID_FLAG) const
  {
    return m_gridAttr.GetForeground(nGRID_FLAG);
  }
  const wxColour &GetStatusForeground(bool bNeedsAttn) const
  {
    return m_gridAttr.GetStatusForeground(bNeedsAttn);
  }
  const wxColour &GetStatusBackground(bool bNeedsAttn) const
  {
    return m_gridAttr.GetStatusBackground(bNeedsAttn);
  }

  bool IsGridItalic(int nGRID_FLAG) const
  {
    return m_gridAttr.IsItalic(nGRID_FLAG);
  }
  bool IsGridBold(int nGRID_FLAG) const
  {
    return m_gridAttr.IsBold(nGRID_FLAG);
  }
  bool IsGridReverse(int nGRID_FLAG) const
  {
    return m_gridAttr.IsReverse(nGRID_FLAG);
  }

  int GetAttrEdited() const
  {
    return m_gridAttr.GetAttrEdited();
  }
  int GetAttrNeedsAttn() const
  {
    return m_gridAttr.GetAttrNeedsAttn();
  }
protected:
  CParmOsiris(int)
  {
    // inherited constructor
    m_bModified = false;
    m_bAutoSave = false;
    SetDefaults();
  }
  virtual void RegisterAll(bool bInConstructor = false);
  void _Init();
  void SetDefaults();

  CParmGridAttributes m_gridAttr;
  wxString m_sFileName;  // file name for Save/Load

  // begin generated variables

${sVars}

  // end generated variables

  bool m_bModified;
  bool m_bAutoSave;
  nwxXmlIOwxString m_ioBatchFormat;
  nwxXmlIOwxString m_ioDefaultSample;
  nwxXmlIOint m_ioIntViewPlotArtifact;
  nwxXmlIOuint m_ioUint1;
  nwxXmlIOint m_ioInt_1; // default to -1
public:
${sStringHeader}

//  **************************************   static/global stuff

public:
  nwxDECLARE_GLOBAL_OBJECT_XML(CParmOsiris)

};

class CParmOsirisGlobal
{
public:
  CParmOsirisGlobal() : m_bGotten(false)
  {}
  virtual ~CParmOsirisGlobal()
  {
    if(m_bGotten)
    {
      Get()->SaveIfModified();
    }
  }
  CParmOsiris *Get()
  {
    m_bGotten = true;
    return CParmOsiris::GetGlobal();
  }
  CParmOsiris * operator ->()
  {
    return Get();
  }
private:
  bool m_bGotten;

};

class CParmOsirisLite : public CParmOsiris
{
public:
  CParmOsirisLite() : CParmOsiris(1)
  {
    RegisterAll(true);
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("inputDirectory", &m_sInputDirectory);
    RegisterWxString("outputDirectory", &m_sOutputDirectory);
    RegisterWxString("kit", &m_sKitName);
    RegisterWxString("ls", &m_sLsName);
    RegisterInt("minRFUsample", &m_nMinRFU_Sample);
    RegisterInt("minRFUILS", &m_nMinRFU_ILS);
    RegisterInt("minRFUladder", &m_nMinRFU_Ladder);
    RegisterInt("minRFUinterlocus", &m_nMinRFU_Interlocus);
    RegisterIntNonZero("minRFUladderInterlocus", &m_nMinLadderInterlocusRFU);
    RegisterIntNonZero("minRFUsampleDetection", &m_nSampleDetectionThreshold);
    RegisterIntVector("ChannelRFU", &m_anChannelRFU);
    RegisterIntVector("ChannelDetection", &m_anChannelDetection);
    RegisterBoolSkipFalse("DataAnalyzed", &m_bDataAnalyzed);
  }
  virtual bool Load()
  {
    return false;
  }
  virtual bool Save()
  {
    return false;
  }
};


#endif
EOF

  my $fileCpp = <<EOF;
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
*  FileName: CParmOsiris.cpp
*  Author:   this file was generated by CParmOsiris.pl
*            a perl script written by Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CParmOsiris.h"
#include "CLabSettings.h"
#include "CVolumes.h"
#include "wxIDS.h"
#include "nwx/nwxFileUtil.h"
#include <wx/filename.h>
#include <wx/utils.h>

nwxIMPLEMENT_GLOBAL_OBJECT(CParmOsiris)

const wxString CParmOsiris::NO_INIT(wxS(":"));

void CParmOsiris::_Init()
{
  SetNoInit(true);
  SetDefaults();
  if(m_sFileName.IsEmpty())
  {
    m_sFileName = mainApp::GetConfig()->GetConfigPath();
    m_sFileName += "osiris.xml";
  }
  m_bModified = false;
  m_bAutoSave = false;
  RegisterAll(true);
  if(!wxFileName::FileExists(m_sFileName))
  {}
  else if(!Load())
  {
    SetDefaults();
  }
}


CParmOsiris &CParmOsiris::operator =(const CParmOsiris &x)
{
#define CP(elem) elem = x.elem

  CP(m_sFileName);

  // begin generated copy

${sCopy}

  // end generated copy

  CP(m_bModified);
  m_bAutoSave = false;
  CP(m_gridAttr);

#undef CP
  return *this;
}

bool CParmOsiris::IsEqual(const CParmOsiris &x) const
{
#define CP(elem) else if(!(elem == x.elem)) { bRtn = false; }
  bool bRtn = true;
  if(0) {}
  // begin generated compare

${sCompare}

  // end generated compare


  CP(m_gridAttr)
//  CP(m_bModified)
//  CP(m_bAutoSave)

#undef CP
  return bRtn;
}


void CParmOsiris::RegisterAll(bool bInConstructor)
{
  if(bInConstructor)
  {
    m_ioDefaultSample.SetDefault(
      CLabNameStrings::DEFAULT_SPECIMEN_CATEGORY);
    m_ioBatchFormat.SetDefault(DEFAULT_BATCH_FORMAT);
    m_ioIntViewPlotArtifact.SetDefault(ARTIFACT_CRITICAL);
    m_ioUint1.SetDefault(1);
    m_ioInt_1.SetDefault(-1);
  }

  // begin generated register

${sRegister}

  // end generated register

  Register("GridColors",&m_gridAttr);
}

CParmOsiris::~CParmOsiris()
{
  if(m_bAutoSave)
  {
    SaveIfModified();
  }
}

void CParmOsiris::SetDefaults()
{
  ConfigDir *pConfig = mainApp::GetConfig();
  const unsigned int RFU = 150;

  wxString sExe = pConfig->GetExePath();
  wxString sInputDirectory = sExe;
  sInputDirectory += "TestAnalysis";
  nwxFileUtil::EndWithSeparator(&sInputDirectory);
  sInputDirectory += "Cofiler";

  m_bModified = true;

  // begin generated defaults

${sDefaults}

  // end generated defaults

  m_gridAttr.SetDefaults();
}
bool CParmOsiris::Save()
{
  bool bRtn = SaveFile(m_sFileName);
  if(!bRtn)
  {
    wxString sError =
      "An error occurred when writing OSIRS parameters file:\\n";
    sError.Append(m_sFileName);
    wxASSERT_MSG(0,sError);
    mainApp::LogMessage(sError);
  }
  else
  {
    m_bModified = false;
  }
  return bRtn;
}

int CParmOsiris::GetPrintColorByName(const wxString &sName) const
{
  // return the printing color intensity (1-100) for printing
  // color specified by sName
  wxString s(sName);
  s.MakeUpper();
  int nRtn = 100; // if color not found, default is 100%
  
 #define _CHECK(name, fnc)  if(s == wxT(name)) nRtn = fnc()  
  _CHECK("RED", GetPrintColorRed);
  else _CHECK("GREEN", GetPrintColorGreen);
  else _CHECK("BLUE", GetPrintColorBlue);
  else _CHECK("YELLOW", GetPrintColorYellow);
  else _CHECK("ORANGE", GetPrintColorOrange);
  else _CHECK("PURPLE", GetPrintColorPurple);
#undef _CHECK

  return nRtn;
}

${sStringCPP}

EOF

  [$fileH,$fileCpp];
}

sub NeedFile
{
  my $sFileName = shift;
  my @aStat = stat($sFileName);
  my $bRtn = ($#aStat < 9);
  if(!$bRtn)
  {
    my @aStatThis = stat($0);
    $bRtn = ($#aStatThis < 9) ||
      ($aStatThis[9] > $aStat[9]);
  }
  $bRtn;
}
sub LoadFile
{
  my $sFileName = shift;
  my $sRtn = "";
  my $bOpen = (open FIN ,"<${sFileName}");
  if($bOpen)
  {
    my @a = (<FIN>);
    $sRtn = join "",@a;
  }
  $sRtn;
}
sub DumpFile
{
  my $sFileName = shift;
  my $sContents = shift;
  my $bVerify = 1;
  (
  (open FOUT,">${sFileName}") &&
  (print FOUT $sContents) &&
  (close FOUT)
  ) || die("Cannot write ${sFileName}");
  $bVerify && (print "created ${sFileName}\n");
  0;
}
sub UpToDate
{
  my $bVerify = shift;
  if($bVerify)
  {
    my $sName;
    while($sName = shift)
    {
      (print "$sName is up to date\n");
    }
  }
  0;
}

sub Run
{
  my $bVerify = undef;
  for my $arg(@ARGV)
  {
    ($arg eq '-v') && ($bVerify = 1);
  }
  my $sFileH =   "CParmOsiris.h";
  my $sFileCPP = "CParmOsiris.cpp";
  my @aUpToDate = ();
  my $aFiles = &GenFiles;
  my @aNames = ($sFileH,$sFileCPP);
  my $sContents;
  my $sCurrent;
  my $sFileName;
  for my $i (0 .. 1)
  {
    $sFileName = shift @aNames;
    $sContents = shift @$aFiles;
    if(&NeedFile($sFileName))
    {
      $sCurrent = LoadFile($sFileName);
      if($sCurrent ne $sContents)
      {
        DumpFile($sFileName,$sContents,$bVerify);
      }
      else
      {
        push @aUpToDate,$sFileName;
      }
    }
    else
    {
      push @aUpToDate,$sFileName;
    }
  }
  &UpToDate($bVerify,@aUpToDate);
  0;
}

&Run;

