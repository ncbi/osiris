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
#  FileName: SampleName.xsl
#  Author:   Douglas Hoffman
#  Description:  Templates and functions for retrieving a sample name
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

  <xsl:output method="xml"/>
  <xsl:variable name="USEFILENAME" select="1"/>
  <xsl:template name="GetNameFromFile">
    <xsl:param name="s"/>
    <xsl:variable name="nLen" select="string-length($s)"/>
    <xsl:choose>
      <xsl:when test="($nLen &gt; 2) and (substring($s,$nLen - 1,1) = '_')">
        <xsl:value-of select="substring($s,1,$nLen - 2)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$s"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="GetSampleName">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when 
        test="$sample/SampleName and (string-length($sample/SampleName) &gt; 0)">
        <xsl:value-of select="string($sample/SampleName)"/>
      </xsl:when>
      <xsl:when test="contains($sample/Name,'_')">
        <xsl:value-of select="substring-before($sample/Name,'_')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="string($sample/Name)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="GetUniqueSampleName">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when test="not($sample)"/>
      <xsl:when test="$USEFILENAME">
        <xsl:call-template name="GetNameFromFile">
          <xsl:with-param name="s" select="string($sample/Name)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="GetSampleName">
          <xsl:with-param name="sample" select="$sample"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:GetSampleName">
    <xsl:param name="sample"/>
    <xsl:variable name="sRtn">
      <xsl:call-template name="GetSampleName">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="$sRtn"/>
  </func:function>

  <func:function name="os:GetUniqueSampleName">
    <xsl:param name="sample"/>
    <xsl:variable name="sRtn">
      <xsl:call-template name="GetUniqueSampleName">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="$sRtn"/>
  </func:function>

  <func:function name="os:GetAllUniqueSampleNames">
    <xsl:param name="oarFile"/>
    <xsl:variable name="tmpTree">
      <xsl:for-each select="$oarFile/OsirisAnalysisReport/Table/Sample">
        <SampleName>
          <xsl:call-template name="GetUniqueSampleName">
            <xsl:with-param name="sample" select="."/>
          </xsl:call-template>
        </SampleName>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="rtn" select="exsl:node-set($tmpTree)"/>
    <func:result select="$rtn"/>
  </func:function>


</xsl:stylesheet>
