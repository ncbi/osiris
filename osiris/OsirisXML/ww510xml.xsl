<?xml version="1.0" encoding="utf-8"?>
<!--
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
#  FileName: ww510xml.xsl
#  Author:   Douglas Hoffman
#  Description:  choose 5 or 10 second injection for output in xml
#    can be used by Woolworth5and10.xml to export text
#  11/1/11
#    bug fix related to running without secondary file
#    was calling template runLims to get text
#    where it now called template isprunBase
#    from template isp-5-and-10-xml
#    when 'Alternate file is not specified'
#
#  5/18/12
#    bug fix, variable $CanRunOne was set to 0, not is 1
#      runLims was being called when one of the two files is rejected
#      it is now running isprunBase
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:str="http://exslt.org/strings"
  xmlns:func="http://exslt.org/functions"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  xmlns:exsl="http://exslt.org/common"
  exclude-result-prefixes="str xsl func os exsl"
  extension-element-prefixes="str func os exsl"
  version="1.1">

  <xsl:import href="ispxml.xsl"/>
  <xsl:output method="xml" indent="yes" standalone="yes" version="1.0"/>

  <!-- parameters, 
       AltFileName - name of the 'other' file to process
       AltSwap - if non zero swap the primary and secondary

       There are two files, 
          MainFile, report file being viewed when export was requested
          AltFile, report file being specified in the export.
       These are used for:
          Primary file - preferred file for extracting sample
          Secondary file - file used for extracting sample if the 
             primary file has critical artifacts for the sample
             and this file doesn't
       Which:
          By default the MainFile is the Primary file
          and the AltFile is the Secondary file.
          If the parameter, AltSwap is numeric and non-zero, then
          this is swapped so that the AltFile is the Primary file
          and the MainFile is the secondary.
  -->

  <xsl:param name="AltFileName"/>
  <xsl:param name="AltSwap" select="0"/>
  <xsl:param name="TestControls" select="1"/>

  <func:function name="os:EqNotEmpty">
    <xsl:param name="strControl"/>
    <xsl:param name="strStdControl"/>
    <!-- $UPPER and $lower are in utils.xsl -->
    <xsl:variable name="s1" select="translate(string($strControl),$UPPER,$lower)"/>
    <xsl:variable name="s2" select="translate(string($strStdControl),$UPPER,$lower)"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="($s1 = '') or ($s1 != $s2)">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <xsl:variable name="CanRunOne" select="1"/>

  <xsl:variable name="USEPRM" select="'p'"/>
  <xsl:variable name="USESEC" select="'s'"/>
  <xsl:variable name="USEBOTH" select="'b'"/>
  <xsl:variable name="USENONE" select="''"/>

  <xsl:template name="TestSample">
    <xsl:param name="xml"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <xsl:param name="sample"/>
    <xsl:param name="ILSchannelNr"/>
    <xsl:param name="loci"/>
    <xsl:param name="anyPass" select="0"/>

    <xsl:variable name="tmp">
      <xsl:for-each select="$sample">
        <xsl:call-template name="RunSample">
          <xsl:with-param name="xml" select="$xml"/>
          <xsl:with-param name="exports" select="$exports"/>
          <xsl:with-param name="messages" select="$messages"/>
          <xsl:with-param name="sample" select="."/>
          <xsl:with-param name="ILSchannelNr" select="$ILSchannelNr"/>
          <xsl:with-param name="loci" select="$loci"/>
          <xsl:with-param name="countUD123dir" select="0"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="xmlSample" select="exsl:node-set($tmp)"/>
    <xsl:choose>
      <xsl:when test="not($xmlSample/sample)">
        <!-- none present or do not export -->
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:when test="number($anyPass) and count($xmlSample/sample[os:IsPristineSample(.)])">
        <!-- something passed -->
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when test="count($xmlSample/sample[not(os:IsPristineSample(.))])">
        <!-- something failed -->
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:otherwise>
        <!-- all passed -->
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="NoPrmNegative">
    <message nr="3">
      <xsl:text>Primary file did not pass the standard positive </xsl:text>
      <xsl:text>control, Control 1 positive control, </xsl:text>
      <xsl:text>and a negative control.</xsl:text>
    </message>
  </xsl:template>

  <xsl:template name="TestAllControls">
    <xsl:param name="Primary"/>
    <xsl:param name="Secondary"/>
    <xsl:param name="PrmSamples"/>
    <xsl:param name="SecSamples"/>
    <xsl:param name="PrmExports"/>
    <xsl:param name="SecExports"/>
    <xsl:param name="PrmMessages"/>
    <xsl:param name="SecMessages"/>
    <xsl:param name="PrmDirectoryUD"/>
    <xsl:param name="SecDirectoryUD"/>

    <xsl:variable name="PrmStdControlName" 
      select="$Primary/OsirisAnalysisReport/Heading/LabSettings/NameStrings/StandardControlName"/>
    <xsl:variable name="SecStdControlName"
      select="$Secondary/OsirisAnalysisReport/Heading/LabSettings/NameStrings/StandardControlName"/>

    <xsl:variable name="PrmILSchannelNr"
      select="$Primary/OsirisAnalysisReport/Heading/ILSchannelNr"/>
    <xsl:variable name="SecILSchannelNr"
      select="$Secondary/OsirisAnalysisReport/Heading/ILSchannelNr"/>

    <xsl:variable name="PrmLoci"
      select="$Primary/OsirisAnalysisReport/Heading/Channel/LocusName"/>
    <xsl:variable name="SecLoci"
      select="$Secondary/OsirisAnalysisReport/Heading/Channel/LocusName"/>


    <xsl:variable name="SecNegative"
      select="$SecSamples[Type = '-Control']"/>
    <xsl:variable name="PrmNegative"
            select="$PrmSamples[Type = '-Control']"/>

    <xsl:variable name="SecTestNegative">
      <xsl:call-template name="TestSample">
        <xsl:with-param name="xml" select="$Secondary"/>
        <xsl:with-param name="exports" select="$SecExports"/>
        <xsl:with-param name="messages" select="$SecMessages"/>
        <xsl:with-param name="sample" select="$SecNegative"/>
        <xsl:with-param name="ILSchannelNr" select="$SecILSchannelNr"/>
        <xsl:with-param name="loci" select="$SecLoci"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="PrmTestNegative">
      <xsl:call-template name="TestSample">
        <xsl:with-param name="xml" select="$Primary"/>
        <xsl:with-param name="exports" select="$PrmExports"/>
        <xsl:with-param name="messages" select="$PrmMessages"/>
        <xsl:with-param name="sample" select="$PrmNegative"/>
        <xsl:with-param name="ILSchannelNr" select="$PrmILSchannelNr"/>
        <xsl:with-param name="loci" select="$PrmLoci"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="AnyTestNegative">
      <xsl:choose>
        <xsl:when test="number($SecTestNegative)">
          <xsl:value-of select="$SecTestNegative"/>
        </xsl:when>
        <xsl:when test="number($PrmTestNegative)">
          <xsl:value-of select="$PrmTestNegative"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- template output -->
    
    <xsl:choose>
      <xsl:when test="$AnyTestNegative = 0">
        <message nr="1">
          <xsl:text>A negative control failed or was not present in both files.</xsl:text>
        </message>
      </xsl:when>

      <xsl:when test="not(number($SecTestNegative))">
        <message nr="2">
          <xsl:text>A negative control failed or was not present in the secondary file.</xsl:text>
        </message>

        <!-- primary negative control passed, check primary positive controls -->

        <xsl:variable name="PrmStdPosCtrl"
          select="$PrmSamples[os:EqNotEmpty(PositiveControl,$PrmStdControlName)]"/>
        <xsl:variable name="PrmControl1"
          select="$PrmSamples[(Type = '+Control') and not(os:EqNotEmpty(PositiveControl,$PrmStdControlName))]"/>

        <xsl:variable name="PrmTestControl1">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Primary"/>
            <xsl:with-param name="exports" select="$PrmExports"/>
            <xsl:with-param name="messages" select="$PrmMessages"/>
            <xsl:with-param name="sample" select="$PrmControl1"/>
            <xsl:with-param name="ILSchannelNr" 
              select="$PrmILSchannelNr"/>
            <xsl:with-param name="loci" select="$PrmLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="PrmTestStdPosCtrl">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Primary"/>
            <xsl:with-param name="exports" select="$PrmExports"/>
            <xsl:with-param name="messages" select="$PrmMessages"/>
            <xsl:with-param name="sample" select="$PrmStdPosCtrl"/>
            <xsl:with-param name="ILSchannelNr" 
              select="$PrmILSchannelNr"/>
            <xsl:with-param name="loci" select="$PrmLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="number($PrmTestControl1) and number($PrmTestStdPosCtrl) and number($PrmTestNegative)">
            <use>
              <xsl:value-of select="$USEPRM"/>
            </use>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="NoPrmNegative"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <!-- negative controls are OK -->

        <xsl:variable name="PrmStdPosCtrl"
          select="$PrmSamples[os:EqNotEmpty(PositiveControl,$PrmStdControlName)]"/>
        <xsl:variable name="PrmControl1"
          select="$PrmSamples[(Type = '+Control') and not(os:EqNotEmpty(PositiveControl,$PrmStdControlName))]"/>
        <xsl:variable name="SecStdPosCtrl"
          select="$SecSamples[os:EqNotEmpty(PositiveControl,$SecStdControlName)]"/>
        <xsl:variable name="SecControl1"
          select="$SecSamples[(Type = '+Control') and not(os:EqNotEmpty(PositiveControl,$SecStdControlName))]"/>


        <xsl:variable name="PrmTestControl1">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Primary"/>
            <xsl:with-param name="exports" select="$PrmExports"/>
            <xsl:with-param name="messages" select="$PrmMessages"/>
            <xsl:with-param name="sample" select="$PrmControl1"/>
            <xsl:with-param name="ILSchannelNr" select="$PrmILSchannelNr"/>
            <xsl:with-param name="loci" select="$PrmLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="PrmTestStdPosCtrl">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Primary"/>
            <xsl:with-param name="exports" select="$PrmExports"/>
            <xsl:with-param name="messages" select="$PrmMessages"/>
            <xsl:with-param name="sample" select="$PrmStdPosCtrl"/>
            <xsl:with-param name="ILSchannelNr" select="$PrmILSchannelNr"/>
            <xsl:with-param name="loci" select="$PrmLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="SecTestControl1">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Secondary"/>
            <xsl:with-param name="exports" select="$SecExports"/>
            <xsl:with-param name="messages" select="$SecMessages"/>
            <xsl:with-param name="sample" select="$SecControl1"/>
            <xsl:with-param name="ILSchannelNr" select="$SecILSchannelNr"/>
            <xsl:with-param name="loci" select="$SecLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="SecTestStdPosCtrl">
          <xsl:call-template name="TestSample">
            <xsl:with-param name="xml" select="$Secondary"/>
            <xsl:with-param name="exports" select="$SecExports"/>
            <xsl:with-param name="messages" select="$SecMessages"/>
            <xsl:with-param name="sample" select="$SecStdPosCtrl"/>
            <xsl:with-param name="ILSchannelNr" select="$SecILSchannelNr"/>
            <xsl:with-param name="loci" select="$SecLoci"/>
            <xsl:with-param name="anyPass" select="1"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="SecPosControl" select="$SecTestControl1 + $SecTestStdPosCtrl"/>
        <xsl:variable name="PrmPosControl" select="$PrmTestControl1 + $PrmTestStdPosCtrl"/>
        <xsl:variable name="AnyStdPosCtrl" select="$SecTestStdPosCtrl + $PrmTestStdPosCtrl"/>
        <xsl:variable name="AnyControl1" select="$SecTestControl1 + $PrmTestControl1"/>
        <xsl:variable name="AnyPosControl" select="$SecPosControl + $PrmPosControl"/>

        <xsl:choose>
          <xsl:when test="not(number($AnyPosControl))">
            <message nr="4">
              <xsl:text>No positive controls passed.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not(number($AnyStdPosCtrl))">
            <message nr="5">
              <xsl:text>No standard positive controls passed.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not(number($AnyControl1))">
            <message nr="6">
              <xsl:text>No Control 1 positive controls passed.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not(number($SecPosControl))">
            <message nr="7">
              <xsl:text>No secondary positive controls passed.</xsl:text>
            </message>
            <!-- both primary pos controls passed -->
            <xsl:choose>
              <xsl:when test="number($PrmTestNegative)">
                <use>
                  <xsl:value-of select="$USEPRM"/>
                </use>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="NoPrmNegative"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:when test="not(number($PrmPosControl))">
            <message nr="8">
              <xsl:text>No primary positive controls passed.</xsl:text>
            </message>
            <!-- both secondary pos controls passed -->
            <use>
              <xsl:value-of select="$USESEC"/>
            </use>
          </xsl:when>
          <xsl:otherwise>
            <use>
              <xsl:value-of select="$USEBOTH"/>
            </use>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template><!-- TestAllControls -->


  <xsl:template name="CopySample">
    <xsl:param name="sample"/>
    <xsl:param name="from"/>
    <xsl:variable name="elemName" select="name($sample)"/>
    <xsl:element name="{$elemName}">
      <xsl:for-each select="$sample/@*">
        <xsl:attribute name="{name(.)}">
          <xsl:value-of select="."/>
        </xsl:attribute>
      </xsl:for-each>
      <xsl:attribute name="from">
        <xsl:value-of select="$from"/>
      </xsl:attribute>
      <xsl:copy-of select="$sample/*"/>
    </xsl:element>
  </xsl:template>


  <xsl:template name="DumpNotFound">
    <xsl:param name="samples"/>
    <xsl:param name="from"/>
    <xsl:if test="$samples">
      <xsl:text>The following samples were found in the </xsl:text>
      <xsl:value-of select="$from"/>
      <xsl:text> file only:</xsl:text>
      <xsl:for-each select="$samples">
        <xsl:text>&#10;    </xsl:text>
        <xsl:value-of select="@name"/>
      </xsl:for-each>
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template name="GetVerificationList">
    <xsl:param name="samples"/>
    <xsl:param name="messages"/>
    <xsl:for-each select="$messages[MsgName = 'smVerificationSample']/MessageNumber">
      <xsl:variable name="n" select="number(.)"/>
      <xsl:variable name="xSample"
        select="$samples[SampleAlerts/MessageNumber = $n]"/>
      <xsl:if test="$xSample">
        <xsl:variable name="sName" select="os:GetUniqueSampleName($xSample)"/>
        <xsl:if test="$sName">
          <sample name="{$sName}"/>
        </xsl:if>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <!--                                         TEMPLATE RunTwoFiles -->

  <xsl:template name="RunTwoFiles">

    <xsl:param name="Primary"/>
    <xsl:param name="Secondary"/>
    <xsl:param name="PrmExports"/>
    <xsl:param name="SecExports"/>
    <xsl:param name="PrmMessages"/>
    <xsl:param name="SecMessages"/>
    <xsl:param name="DebugFilePrimary"/>
    <xsl:param name="DebugFileSecondary"/>
    <xsl:param name="userMessages"/>

    <xsl:variable name="VerificationListXML">
      <x>
        <xsl:call-template name="GetVerificationList">
          <xsl:with-param name="samples" select="$Primary/OsirisAnalysisReport/Table/Sample"/>
          <xsl:with-param name="messages" select="$PrmMessages"/>
        </xsl:call-template>
        <xsl:call-template name="GetVerificationList">
          <xsl:with-param name="samples" select="$Secondary/OsirisAnalysisReport/Table/Sample"/>
          <xsl:with-param name="messages" select="$SecMessages"/>
        </xsl:call-template>
      </x>
    </xsl:variable>
    <xsl:variable name="VerificationList" select="exsl:node-set($VerificationListXML)//sample/@uname"/>

    <xsl:variable name="tmpPrmXml">
      <xsl:call-template name="isprun">
        <xsl:with-param name="xml" select="$Primary"/>
        <xsl:with-param name="exports" select="$PrmExports"/>
        <xsl:with-param name="messages" select="$PrmMessages"/>
        <xsl:with-param name="VerificationList" select="$VerificationList"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="PrmXml" select="exsl:node-set($tmpPrmXml)"/>

    <xsl:variable name="SkipList" 
      select="$PrmXml/directory/sample[os:IsPristineSample(.)]/@uname"/>

    <xsl:variable name="tmpSecXml">
      <xsl:call-template name="isprun">
        <xsl:with-param name="xml" select="$Secondary"/>
        <xsl:with-param name="exports" select="$SecExports"/>
        <xsl:with-param name="messages" select="$SecMessages"/>
        <xsl:with-param name="SkipList" select="$SkipList"/>
        <xsl:with-param name="VerificationList" 
          select="$VerificationList"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="SecXml" select="exsl:node-set($tmpSecXml)"/>
    <xsl:call-template name="debugXML">
      <xsl:with-param name="file" select="$DebugFilePrimary"/>
      <xsl:with-param name="xml" select="$PrmXml"/>
    </xsl:call-template>
    <xsl:call-template name="debugXML">
      <xsl:with-param name="file" select="$DebugFileSecondary"/>
      <xsl:with-param name="xml" select="$SecXml"/>
    </xsl:call-template>


    <xsl:variable name="PrmHeader" select="$PrmXml/directory/header"/>
    <xsl:variable name="SecHeader" select="$SecXml/directory/header"/>

    <xsl:choose>
      <xsl:when test="$PrmHeader and $SecHeader">
        <directory>
          <header>
            <xsl:copy-of select="$PrmHeader/column"/>
            <xsl:if test="string-length($userMessages)">
              <column>
                <xsl:value-of select="$userMessages"/>
              </column>
            </xsl:if>
          </header>

          <!-- loop through all primary samples, pick primary or secondary, and dump -->

          <xsl:for-each select="$PrmXml/directory/sample">
            <xsl:variable name="PrmSample" select="."/>
            <xsl:variable name="name" select="$PrmSample/@uname"/>
            <xsl:variable name="SecSample" select="$SecXml/directory/sample[@uname = $name]"/>
            <xsl:choose>
              <xsl:when test="not($SecSample) or not(os:IsPristineSample($SecSample))">
                <xsl:call-template name="CopySample">
                  <xsl:with-param name="sample" select="$PrmSample"/>
                  <xsl:with-param name="from" select="'Primary'"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="CopySample">
                  <xsl:with-param name="sample" select="$SecSample"/>
                  <xsl:with-param name="from" select="'Secondary'"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>

          <!-- now loop through all secondary samples that are not in the primary list -->
          <xsl:variable name="PrmList"
            select="os:GetAllUniqueSampleNames($Primary)"/>
          <xsl:variable name="SecList"
            select="os:GetAllUniqueSampleNames($Secondary)"/>
          <xsl:variable name="SecNotPrm"
            select="$SecXml/directory/sample[ not(os:InList($PrmList/SampleName,@uname)) ]"/>
          <xsl:variable name="PrmNotSec"
            select="$PrmXml/directory/sample[ not(os:InList($SecList/SampleName,@uname)) ]"/>

          <xsl:for-each select="$SecNotPrm">
            <xsl:call-template name="CopySample">
              <xsl:with-param name="sample" select="."/>
              <xsl:with-param name="from" select="'Secondary'"/>
            </xsl:call-template>
          </xsl:for-each>
          <xsl:variable name="NotFound">
            <xsl:call-template name="DumpNotFound">
              <xsl:with-param name="samples" select="$PrmNotSec"/>
              <xsl:with-param name="from" select="'primary'"/>
            </xsl:call-template>
            <xsl:call-template name="DumpNotFound">
              <xsl:with-param name="samples" select="$SecNotPrm"/>
              <xsl:with-param name="from" select="'secondary'"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="string-length($NotFound)">
            <xsl:message>
              <xsl:value-of select="$NotFound"/>
            </xsl:message>
          </xsl:if>
        </directory>
      </xsl:when>
      <xsl:when test="$PrmHeader">
        <xsl:message>
          <xsl:text>No header found in secondary output.  This should never happen.</xsl:text>
        </xsl:message>
        <xsl:copy-of select="$PrmXml"/>
      </xsl:when>
      <xsl:when test="$SecHeader">
        <xsl:message>
          <xsl:text>No header found in primary output.  This should never happen.</xsl:text>
        </xsl:message>
        <xsl:copy-of select="$SecXml"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message>
          <xsl:text>No header found in any output.  This should never happen.</xsl:text>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>



  <!--                                         TEMPLATE TestRunTwoFiles -->

  <xsl:template name="TestRunTwoFiles">
    <xsl:param name="Primary"/>
    <xsl:param name="Secondary"/>
    <xsl:param name="DebugFilePrimary"/>
    <xsl:param name="DebugFileSecondary"/>

    <xsl:variable name="PrmExports"
      select="os:GetExports($Primary)"/>
    <xsl:variable name="PrmMessages"
      select="os:GetExportMessages($Primary)"/>
    <xsl:variable name="SecExports"
      select="os:GetExports($Secondary)"/>
    <xsl:variable name="SecMessages"
      select="os:GetExportMessages($Secondary)"/>

    <xsl:variable name="tmpPrmDirectoryUD">
      <xsl:call-template name="directoryUD">
        <xsl:with-param name="xml" select="$Primary"/>
        <xsl:with-param name="exports" select="$PrmExports"/>
        <xsl:with-param name="messages" select="$PrmMessages"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="tmpSecDirectoryUD">
      <xsl:call-template name="directoryUD">
        <xsl:with-param name="xml" select="$Secondary"/>
        <xsl:with-param name="exports" select="$SecExports"/>
        <xsl:with-param name="messages" select="$SecMessages"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="PrmDirectoryUD"
      select="exsl:node-set($tmpPrmDirectoryUD)"/>
    <xsl:variable name="SecDirectoryUD"
      select="exsl:node-set($tmpSecDirectoryUD)"/>
    <xsl:variable name="PrmNoExport"
      select="count($PrmDirectoryUD//*/@noexport)"/>
    <xsl:variable name="SecNoExport"
      select="count($SecDirectoryUD//*/@noexport)"/>


    <xsl:variable name="PrmSamples"
      select="$Primary/OsirisAnalysisReport/Table/Sample"/>
    <xsl:variable name="SecSamples"
      select="$Secondary/OsirisAnalysisReport/Table/Sample"/>

    <xsl:variable name="tmpUsage">
      <usage>
        <xsl:choose>
          <xsl:when test="$PrmNoExport and $SecNoExport">
            <message nr="9">
              <xsl:text>Directory level messages prevent all export</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not($PrmSamples) and not($SecSamples)">
            <message nr="10">
              <xsl:text>No samples found.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not($PrmSamples) and $SecNoExport">
            <message nr="11">
              <xsl:text>No samples in primary file and </xsl:text>
              <xsl:text>directory level messages in secondary </xsl:text>
              <xsl:text>file prevent all export.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not($SecSamples) and $PrmNoExport">
            <message nr="12">
              <xsl:text>No samples in secondary file and </xsl:text>
              <xsl:text>directory level messages in primary </xsl:text>
              <xsl:text>file prevent all export.</xsl:text>
            </message>
          </xsl:when>
          <xsl:when test="not($TestControls)">
            <use>
              <xsl:value-of select="$USEBOTH"/>
            </use>
          </xsl:when>
          <xsl:otherwise>
                 <xsl:call-template name="TestAllControls">
                   <xsl:with-param name="Primary" select="$Primary"/>
                   <xsl:with-param name="Secondary" select="$Secondary"/>
                   <xsl:with-param name="PrmSamples" select="$PrmSamples"/>
                   <xsl:with-param name="SecSamples" select="$SecSamples"/>
                   <xsl:with-param name="PrmExports" select="$PrmExports"/>
                   <xsl:with-param name="SecExports" select="$SecExports"/>
                   <xsl:with-param name="PrmMessages" select="$PrmMessages"/>
                   <xsl:with-param name="SecMessages" select="$SecMessages"/>
                   <xsl:with-param name="PrmDirectoryUD" select="$PrmDirectoryUD"/>
                   <xsl:with-param name="SecDirectoryUD" select="$SecDirectoryUD"/>
                 </xsl:call-template>
               </xsl:otherwise>
             </xsl:choose>
      </usage>
    </xsl:variable>
    <xsl:variable name="xmlUsage" select="exsl:node-set($tmpUsage)"/>
    <xsl:variable name="usage" select="$xmlUsage//use"/>
    <xsl:variable name="userMessages">
      <xsl:for-each select="$xmlUsage//message">
        <xsl:if test="position() != 1">
          <xsl:text>; </xsl:text>
        </xsl:if>
        <xsl:value-of select="@nr"/>
        <xsl:text>: </xsl:text>
        <xsl:value-of select="."/>
        <xsl:message>
          <xsl:value-of select="."/>
        </xsl:message>
      </xsl:for-each>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$usage = $USEBOTH">
        <xsl:call-template name="RunTwoFiles">
          <xsl:with-param name="Primary" select="$Primary"/>
          <xsl:with-param name="Secondary" select="$Secondary"/>
          <xsl:with-param name="PrmExports" select="$PrmExports"/>
          <xsl:with-param name="SecExports" select="$SecExports"/>
          <xsl:with-param name="PrmMessages" select="$PrmMessages"/>
          <xsl:with-param name="SecMessages" select="$SecMessages"/>
          <xsl:with-param name="DebugFilePrimary" select="$DebugFilePrimary"/>
          <xsl:with-param name="DebugFileSecondary" select="$DebugFileSecondary"/>
          <xsl:with-param name="userMessages" select="$userMessages"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="not($CanRunOne)"/>
      <xsl:when test="$usage = $USEPRM">
        <xsl:call-template name="isprunBase">
          <xsl:with-param name="xml" select="$Primary"/>
          <xsl:with-param name="userMessages" select="$userMessages"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$usage = $USESEC">
        <xsl:call-template name="isprunBase">
          <xsl:with-param name="xml" select="$Secondary"/>
          <xsl:with-param name="userMessages" select="$userMessages"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>


  <func:function name="os:AltDocument">
    <xsl:param name="altFileName"/>
    <xsl:choose>
      <xsl:when test="string-length($altFileName) != 0">
        <xsl:variable name="AltFileNameEncoded"
          select="os:encode-file-uri($altFileName)"/>
        <xsl:variable name="xmlAlt"
          select="document($AltFileNameEncoded)"/>
        <func:result select="$xmlAlt"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="false()"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  <xsl:template name="CheckAltFileName">
    <xsl:param name="altFileName"/>
    <xsl:choose>
      <xsl:when test="string-length($altFileName) = 0">
        <xsl:message>
          <xsl:text>Alternate file is not specified.</xsl:text>
        </xsl:message>
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
   
  </xsl:template>

  <xsl:template name="CheckAltFile">
    <xsl:param name="xmlAlt"/>
    <xsl:param name="AltFileName"/>
    <xsl:choose>
      <xsl:when test="not($xmlAlt) or not($xmlAlt/OsirisAnalysisReport)">
        <xsl:message>
          <xsl:text>Alternate file, </xsl:text>
          <xsl:value-of select="$AltFileName"/>
          <xsl:text>, is not a valid report file.</xsl:text>
        </xsl:message>
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--                                         TEMPLATE match=/ -->


  <xsl:template name="run-5-and-10-xml">
    <xsl:param name="xmlAlt"/>
    <xsl:param name="altSwap"/>
    <xsl:param name="DebugFilePrimary"/>
    <xsl:param name="DebugFileSecondary"/>
    <xsl:choose>
      <xsl:when test="not($xmlAlt) or not($xmlAlt/OsirisAnalysisReport)"/>
      <xsl:when test="os:Number0($altSwap) = 0">
        <xsl:call-template name="TestRunTwoFiles">
          <xsl:with-param name="Primary" select="/"/>
          <xsl:with-param name="Secondary" select="$xmlAlt"/>
          <xsl:with-param name="DebugFilePrimary"
            select="$DebugFilePrimary"/>
          <xsl:with-param name="DebugFileSecondary"
            select="$DebugFileSecondary"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="TestRunTwoFiles">
          <xsl:with-param name="Primary" select="$xmlAlt"/>
          <xsl:with-param name="Secondary" select="/"/>
          <xsl:with-param name="DebugFilePrimary"
            select="$DebugFileSecondary"/>
          <xsl:with-param name="DebugFileSecondary"
            select="$DebugFilePrimary"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template match="/">

    <xsl:variable name="hasAltFile">
      <xsl:call-template name="CheckAltFileName">
        <xsl:with-param name="altFileName" select="$AltFileName"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$hasAltFile = '0'">
        <xsl:call-template name="isprunBase">
          <xsl:with-param name="xml" select="/"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="xmlAlt"
          select="os:AltDocument($AltFileName)"/>
        <xsl:variable name="checkAlt">
          <xsl:call-template name="CheckAltFile">
            <xsl:with-param name="xmlAlt" select="$xmlAlt"/>
            <xsl:with-param name="AltFileName" select="$AltFileName"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$checkAlt = '1'">
          <xsl:call-template name="run-5-and-10-xml">
            <xsl:with-param name="xmlAlt" select="$xmlAlt"/>
            <xsl:with-param name="altSwap" select="$AltSwap"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
