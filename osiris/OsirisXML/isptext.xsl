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
#  FileName: isptext.xsl
#  Author:   Douglas Hoffman
#  Description:  Export DATA for the Illinois State Police STaCS LIMS
#    in tab delimited text to be used as import for a LIMS system.
#    This uses ispxml.xsl to export the data into xml and then transforms
#    that xml into text
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
  <xsl:output method="text" encoding="iso-8859-1"/>
  <xsl:param name="outputFile"/>

  <xsl:variable name="TAB" select="'&#9;'"/>
  <xsl:variable name="EOL" select="'&#13;&#10;'"/>

  <xsl:template name="OsirisExport">
  <export>
    <name>ISP STaCS</name>
    <file-extension>tab</file-extension>
    <file-extension>txt</file-extension>
    <file-extension>stx</file-extension>
    <extension-override>false</extension-override>
    <default-location>*A</default-location>
  </export>
  </xsl:template>



  <xsl:template name="DumpHeaderText">
    <xsl:param name="xml"/>
    <xsl:call-template name="join">
      <xsl:with-param name="nodes" select="$xml/directory/header/column"/>
      <xsl:with-param name="delim" select="$TAB"/>
    </xsl:call-template>
    <xsl:value-of select="$EOL"/>
  </xsl:template>

  <xsl:template name="repeat">
    <xsl:param name="count" select="0"/>
    <xsl:param name="str" select="$TAB"/>
    <xsl:if test="$count &gt; 0">
      <xsl:value-of select="$str"/>
      <xsl:call-template name="repeat">
        <xsl:with-param name="count" select="$count - 1"/>
        <xsl:with-param name="str" select="$str"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="SkipSample">
    <xsl:param name="sample"/>
    <xsl:value-of select="0"/>
  </xsl:template>

  <xsl:template name="DumpSampleText">
    <xsl:param name="sample"/>
    <xsl:variable name="min1" select="os:min($sample//UD1/@priority)"/>
    <xsl:variable name="min2" select="os:min($sample//UD2/@priority)"/>
    <xsl:variable name="min3" select="os:min($sample//UD3/@priority)"/>
    <xsl:variable name="ud1">
      <xsl:call-template name="join">
        <xsl:with-param name="nodes"
          select="$sample//UD1[(@priority = $min1) and string-length(.)]"/>
        <xsl:with-param name="delim" select="';'"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ud2tmp">
      <xsl:call-template name="join">
        <xsl:with-param name="nodes"
          select="$sample//UD2[@priority = $min2 and string-length(.)]"/>
        <xsl:with-param name="delim" select="';'"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ud3">
      <xsl:call-template name="join">
        <xsl:with-param name="nodes"
          select="$sample//UD3[@priority = $min3 and string-length(.)]"/>
        <xsl:with-param name="delim" select="';'"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ud2">
     <xsl:choose>
        <xsl:when test="$ud1 != 'REEXTRACT'">
          <xsl:value-of select="$ud2tmp"/>
        </xsl:when>
        <xsl:when test="$ud2tmp = 'SWAB' or $ud2tmp = 'REEXTRACT SWAB'">
          <xsl:text>REEXTSWAB</xsl:text>
        </xsl:when>
        <xsl:when test="$ud2tmp = 'BLOOD' or $ud2tmp = 'REEXTRACT BLOOD'">
          <xsl:text>REEXTBLOOD</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$ud2tmp"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="sampleName" select="$sample/@name"/>
    <xsl:variable name="sampleFile" select="$sample/@file"/>
    <xsl:variable name="verification" select="os:IsVerificationSample($sample)"/>
    <xsl:variable name="bSkipSample">
      <xsl:call-template name="SkipSample">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if test="$bSkipSample = 0">
      <xsl:for-each select="$sample/locus">
        <xsl:if test="not($verification) or os:IsPristine(.)">

          <!-- for the verificcation sample, only pristine loci are written -->

          <xsl:value-of select="$sampleName"/>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="@name"/>
          
          <xsl:choose>
            <xsl:when test="$ud1 = 'REEXTRACT' or $ud1 = 'REAMP'">
              <xsl:call-template name="repeat">
                <xsl:with-param name="str" select="$TAB"/>
                <xsl:with-param name="count" select="4"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:for-each select="allele[position() &lt; 5]">
                <xsl:value-of select="$TAB"/>
                <xsl:value-of select="@name"/>
              </xsl:for-each>
              <xsl:call-template name="repeat">
                <xsl:with-param name="str" select="$TAB"/>
                <xsl:with-param name="count" select="4 - count(allele)"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="$ud1"/>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="$ud2"/>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="$ud3"/>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="$TAB"/>
          <xsl:value-of select="$sampleFile"/>
          <xsl:text>.fsa</xsl:text>
          <xsl:value-of select="$EOL"/>
        </xsl:if>
      </xsl:for-each>
      
    </xsl:if>
    
  </xsl:template>

  <xsl:template name="xml2lims">
    <xsl:param name="xml"/>
    <xsl:variable name="samples" select="$xml/directory/sample"/>
    <xsl:if test="$samples">
      <xsl:call-template name="DumpHeaderText">
        <xsl:with-param name="xml" select="$xml"/>
      </xsl:call-template>
      <xsl:for-each select="$samples">
        <xsl:call-template name="DumpSampleText">
          <xsl:with-param name="sample" select="."/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template name="runLims">
    <xsl:param name="xml"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>

    <xsl:variable name="tmpXmlOut">
      <xsl:call-template name="isprun">
        <xsl:with-param name="xml" select="$xml"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="xmlOut" select="exsl:node-set($tmpXmlOut)"/>

    <xsl:call-template name="xml2lims">
      <xsl:with-param name="xml" select="$xmlOut"/>
    </xsl:call-template>
    <xsl:if test="string-length($outputFile)">
      <xsl:variable name="testFile">
        <xsl:value-of select="os:Path($outputFile)"/>
        <xsl:text>test_isp.xml</xsl:text>
      </xsl:variable>
      <xsl:call-template name="debugXML">
        <xsl:with-param name="file" select="$testFile"/>
        <xsl:with-param name="xml" select="$xmlOut"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="runLimsBase">
    <xsl:param name="xml"/>

    <xsl:call-template name="runLims">
      <xsl:with-param name="xml" select="$xml"/>
      <xsl:with-param name="exports"
        select="os:GetExports($xml)"/>
      <xsl:with-param name="messages"
        select="os:GetExportMessages($xml)"/>
    </xsl:call-template>

  </xsl:template>

  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="/directory">
        <xsl:call-template name="xml2lims">
          <xsl:with-param name="xml" select="/"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="runLimsBase">
          <xsl:with-param name="xml" select="/"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
