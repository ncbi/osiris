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
#  FileName: ww510.xsl
#  Author:   Douglas Hoffman
#  Description:  Export file for selecting 5 or 10 second injection
#    for export to tab delimited text
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

  <xsl:import href="ww510xml.xsl"/>
  <xsl:import href="isptext.xsl"/>
  <xsl:import href="SamplesNeedAttn.xsl"/>
  <xsl:output method="text"/>

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
  <xsl:param name="ud3check" select="2"/>
  <xsl:param name="outputFile"/>


  <xsl:template name="OsirisExport">
  <export>
    <name>5 or 10 sec injection</name>
    <file-extension>tab</file-extension>
    <file-extension>txt</file-extension>
    <file-extension>stx</file-extension>
    <extension-override>false</extension-override>
    <default-location>*A</default-location>
    <xsl-params>
      <param>
        <name>AltFileName</name>
        <description>Alternate OSIRIS Report File</description>
        <type>input file name</type>
        <inputFileDefaultDir>*P</inputFileDefaultDir>
        <inputFileRequired>1</inputFileRequired>
        <inputFileExt>
          <x>oar</x>
          <x>oer</x>
        </inputFileExt>
      </param>
      <param>
        <name>AltSwap</name>
        <description>Use this as the primary (5-sec. injection) file</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>0</unchecked-value>
      </param>
      <param>
        <name>ud3check</name>
        <description>Cancel output when sample needs attention.</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>0</unchecked-value>
      </param>
    </xsl-params>
  </export>
  </xsl:template>

  <xsl:template name="run-5-and-10-text">

    <!-- called from an xsl that imports this file -->

    <xsl:param name="altFileName"/>
    <xsl:param name="altSwap"/>
    <xsl:variable name="OutFileDir">
      <xsl:if test="$outputFile">
        <xsl:value-of select="os:Path($outputFile)"/>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="nLen" select="string-length($OutFileDir)"/>
    <xsl:variable name="DebugFilePrimary">
      <xsl:if test="$nLen">
        <xsl:value-of select="$OutFileDir"/>
        <xsl:text>DebugPrimary.xml</xsl:text>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="DebugFileSecondary">
      <xsl:if test="$nLen">
        <xsl:value-of select="$OutFileDir"/>
        <xsl:text>DebugSecondary.xml</xsl:text>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="DebugOutput">
      <xsl:if test="$nLen">
        <xsl:value-of select="$OutFileDir"/>
        <xsl:text>DebugOutput.xml</xsl:text>
      </xsl:if>
    </xsl:variable>

    <xsl:variable name="hasAltFile">
      <xsl:call-template name="CheckAltFileName">
        <xsl:with-param name="altFileName" select="$altFileName"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="xmlAlt" select="os:AltDocument($altFileName)"/>

    <xsl:variable name="tmpXml">
      <xsl:choose>
        <xsl:when test="$hasAltFile = '0'">
          <xsl:call-template name="isprunBase">
            <xsl:with-param name="xml" select="/"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="run-5-and-10-xml">
            <xsl:with-param name="xmlAlt" select="$xmlAlt"/>
            <xsl:with-param name="altSwap" select="$altSwap"/>
            <xsl:with-param name="DebugFilePrimary"
              select="$DebugFilePrimary"/>
            <xsl:with-param name="DebugFileSecondary"
              select="$DebugFileSecondary"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="xml" select="exsl:node-set($tmpXml)"/>
    <xsl:variable name="nUD3" select="os:Number0($ud3check)"/>
    <xsl:variable name="ud3problem">
      <xsl:choose>
        <xsl:when test="$nUD3 = 0"/>
        <xsl:when test="not($xml) or not($xml/directory)"/>
        <xsl:when test="($hasAltFile = '0') or (os:Number0($altSwap) = 0)">
          <xsl:call-template name="AnySampleNeedsAttn">
            <xsl:with-param name="docPrimary" select="/"/>
            <xsl:with-param name="docSecondary" select="$xmlAlt"/>
            <xsl:with-param name="limsxml" select="$xml"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="AnySampleNeedsAttn">
            <xsl:with-param name="docPrimary" select="$xmlAlt"/>
            <xsl:with-param name="docSecondary" select="/"/>
            <xsl:with-param name="limsxml" select="$xml"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="string-length($ud3problem) = 0">
        <xsl:call-template name="xml2lims">
          <xsl:with-param name="xml" select="$xml"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message>
          <xsl:value-of select="$ud3problem"/>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:call-template name="debugXML">
      <xsl:with-param name="file" select="$DebugOutput"/>
      <xsl:with-param name="xml" select="$xml"/>
    </xsl:call-template>
  </xsl:template>


  <xsl:template match="/">
    <xsl:call-template name="run-5-and-10-text">
      <xsl:with-param name="altFileName" select="$AltFileName"/>
      <xsl:with-param name="altSwap" select="$AltSwap"/>
    </xsl:call-template>
  </xsl:template>

</xsl:stylesheet>
