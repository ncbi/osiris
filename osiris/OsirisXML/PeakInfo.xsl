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
#  FileName: PeakInfo.xsl
#  Author:   Douglas Hoffman
#  Description:  Export allele info from OSIRIS analysis file to a
#   tab-delimited text file.
#  Date:  10/6/2015
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:exsl="http://exslt.org/common"
  extension-element-prefixes="exsl"
  version="1.0">

  <xsl:output method="text"/>

  <!--
  <xsl:param name="OL" select="yes"/>
  <xsl:param name="UseChannelNr" select="0"/>
  -->

  <xsl:variable name="alleleCount" select="10"/>
  <!--
       variables tmpColumnList and column list
       list of columns to display. 
        @title is column header to be shown in the output
       @path is the XPath inside <Allele/> to find the data
       -->

  <xsl:variable name="tmpColumnList">
    <cols>
      <col title="Allele" path="Name"/>
      <col title="Size" path="BPS"/>
      <col title="Height" path="RFU"/>
    </cols>
  </xsl:variable>
  <xsl:variable name="ColumnList" select="exsl:node-set($tmpColumnList)//col"/>

  <xsl:variable name="TAB" select="'&#9;'"/>
  <xsl:variable name="EOL" select="'&#10;'"/>

  <xsl:template name="writeAlleleHeaders">
    <xsl:param name="Title"/>
    <xsl:param name="Begin" select="1"/>
    <xsl:param name="End" select="$alleleCount"/>
    <xsl:variable name="diff" select="$End - $Begin"/>
    <xsl:choose>
      <xsl:when test="$End &lt; 1"/>
      <xsl:when test="$diff &lt; 0"/>
      <xsl:when test="$diff &gt; 4">
        <xsl:variable name="mid" select="floor( ($End + $Begin) * 0.5 )"/>
        <xsl:call-template name="writeAlleleHeaders">
          <xsl:with-param name="Title" select="$Title"/>
          <xsl:with-param name="Begin" select="$Begin"/>
          <xsl:with-param name="End" select="$mid"/>
        </xsl:call-template>
        <xsl:call-template name="writeAlleleHeaders">
          <xsl:with-param name="Title" select="$Title"/>
          <xsl:with-param name="Begin" select="$mid + 1"/>
          <xsl:with-param name="End" select="$End"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat($TAB,$Title,' ',$Begin)"/>
        <xsl:call-template name="writeAlleleHeaders">
          <xsl:with-param name="Title" select="$Title"/>
          <xsl:with-param name="Begin" select="$Begin + 1"/>
          <xsl:with-param name="End" select="$End"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="repeat">
    <xsl:param name="String" select="$TAB"/>
    <xsl:param name="Count"/>
    <xsl:choose>
      <xsl:when test="$Count &lt; 1"/>
      <xsl:when test="$Count &gt; 4">
        <xsl:variable name="half" select="floor($Count * 0.5)"/>
        <xsl:call-template name="repeat">
          <xsl:with-param name="Count" select="$half"/>
          <xsl:with-param name="String" select="$String"/>
        </xsl:call-template>
        <xsl:call-template name="repeat">
          <xsl:with-param name="Count" select="$Count - $half"/>
          <xsl:with-param name="String" select="$String"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$String"/>
        <xsl:call-template name="repeat">
          <xsl:with-param name="Count" select="$Count - 1"/>
          <xsl:with-param name="String" select="$String"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="writeHeader">
    <xsl:text>Sample File</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Marker</xsl:text>
    <xsl:for-each select="$ColumnList">
      <xsl:call-template name="writeAlleleHeaders">
        <xsl:with-param name="Title" select="string(@title)"/>
      </xsl:call-template>
    </xsl:for-each>
    <xsl:value-of select="$EOL"/>
  </xsl:template>

  <xsl:template name="writeSample">
    <xsl:param name="Sample"/>
    <xsl:if test="$Sample/Locus/Allele[not(Disabled = 'true')]">
      <xsl:for-each select="$Sample/Locus">
        <xsl:value-of select="concat($Sample/Name,$TAB,LocusName)"/>
        <xsl:variable name="Alleles"
          select="Allele[not(Disabled = 'true')]"/>
        <xsl:variable name="nAlleles" select="count($Alleles)"/>
        <xsl:for-each select="$ColumnList">
          <xsl:variable name="path" select="@path"/>
          <xsl:for-each select="$Alleles[position() &lt;= $alleleCount]">
            <xsl:value-of select="$TAB"/>
            <xsl:value-of select="*[name() = $path]"/>
          </xsl:for-each>
          <xsl:call-template name="repeat">
            <xsl:with-param name="Count" select="$alleleCount - $nAlleles"/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:value-of select="$EOL"/>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template match="/">
    <xsl:call-template name="writeHeader"/>
    <xsl:for-each select="/OsirisAnalysisReport/Table/Sample">
      <xsl:call-template name="writeSample">
        <xsl:with-param name="Sample" select="."/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
