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
#  FileName: extractSamples.xsl
#  Author:   Douglas Hoffman
#  Description:  Export specific samples from an OSIRIS report into
#   a new OSIRIS report.
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:str="http://exslt.org/strings"
  xmlns:date="http://exslt.org/dates-and-times"
  extension-element-prefixes="str date"
  exclude-result-prefixes="xsl date str"
  version="1.0">

  <xsl:output method="xml"/>

  <xsl:param name="outputFile"/>
  <xsl:param name="names"/>
  <xsl:param name="p1IncludeControls" select="1"/>
  <xsl:param name="p2IncludeLadders" select="1"/>
  <xsl:param name="p3IncludeDisabled" select="0"/>

  <xsl:variable name="delim" select="','"/>

  <xsl:template name="OsirisExport">
    <export>
    <name>Selected samples</name>
    <file-extension>oer</file-extension>
    <extension-override>false</extension-override>
    <default-location>*A</default-location>
    <xsl-params>
      <param>
        <name>names</name>
        <description>Enter part or whole names of samples to export separated by commas</description>
        <type>text</type>
      </param>
      <param>
        <name>p1IncludeControls</name>
        <description>Include positive and negative controls</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>9</unchecked-value>
      </param>
      <param>
        <name>p2IncludeLadders</name>
        <description>Include ladder files</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>9</unchecked-value>
      </param>
      <param>
        <name>p3IncludeDisabled</name>
        <description>Include disabled samples</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>9</unchecked-value>
      </param>
    </xsl-params>
    </export>
  </xsl:template>

  <xsl:template name="trim">
    <xsl:param name="str"/>
    <xsl:variable name="len" select="string-length($str)"/>
    <xsl:choose>
      <xsl:when test="$len = 0"/>
      <xsl:when test="$str = ' '"/>
      <xsl:when test="substring($str,1,1) = ' '">
        <xsl:call-template name="trim">
          <xsl:with-param name="str" select="substring($str,2,$len - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="substring($str,$len,1) = ' '">
        <xsl:call-template name="trim">
          <xsl:with-param name="str" select="substring($str,1,$len - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="synonyms">
    <xsl:param name="syns"/>
    <xsl:if test="$syns and count($syns)">
      <xsl:for-each select="$syns">
        <xsl:variable name="thisSyn">
          <xsl:call-template name="trim">
            <xsl:with-param name="str" select="."/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$thisSyn != ''">
          <xsl:value-of select="$delim"/>
          <xsl:value-of select="$thisSyn"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template name="searchList">
    <xsl:variable name="search1">
      <xsl:value-of select="$names"/>
      <xsl:variable name="treeSearch">
        <xsl:choose>
          <xsl:when test="$p1IncludeControls and $p2IncludeLadders">
            <xsl:text>Strings</xsl:text>
          </xsl:when>
          <xsl:when test="$p1IncludeControls">
            <xsl:text>ControlStrings</xsl:text>
          </xsl:when>
          <xsl:when test="$p2IncludeLadders">
            <xsl:text>LadderStrings</xsl:text>
          </xsl:when>
        </xsl:choose>
      </xsl:variable>
      <xsl:if test="$treeSearch != ''">
        <xsl:call-template name="synonyms">
          <xsl:with-param name="syns"
            select="/OsirisAnalysisReport/Heading/LabSettings/NameStrings/*[contains(name(),$treeSearch)]/Synonym"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>
    <xsl:value-of select="translate($search1,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
  </xsl:template>


  <xsl:template name="isMatch">
    <xsl:param name="name"/>
    <xsl:param name="list"/>
    <xsl:variable name="hasDelim">
      <xsl:choose>
        <xsl:when test="contains($list,$delim)">
          <xsl:value-of select="1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="testStr">
      <xsl:variable name="testStr0">
        <xsl:choose>
          <xsl:when test="$hasDelim = 1">
            <xsl:value-of select="substring-before($list,$delim)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$list"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:call-template name="trim">
        <xsl:with-param name="str" select="$testStr0"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="(string-length($testStr) &gt; 0) and contains($name,$testStr)">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when test="$hasDelim = 1">
        <xsl:call-template name="isMatch">
          <xsl:with-param name="name" select="$name"/>
          <xsl:with-param name="list" select="substring-after($list,$delim)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <!-- default template match, just copy the node -->

  <xsl:template match="*" priority="1">
    <xsl:copy-of select="."/>
  </xsl:template>

  <!--  Table/Sample templates -->

  <xsl:template name="isEnabled">
    <xsl:param name="sample"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="$sample/EnableHistory">
          <xsl:for-each select="$sample/EnableHistory/Enable">
            <xsl:sort select="Time" order="descending" data-type="number"/>
            <xsl:choose>
              <xsl:when test="position() != 1"/>
              <xsl:when test="Enabled = 'true'">
                <xsl:value-of select="1"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="0"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$rtn"/>
  </xsl:template>

  <xsl:template match="Sample" priority="100">
    <xsl:param name="allNames"/>
    <xsl:variable name="match">
      <xsl:variable name="name1" 
          select="translate(Name,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
      <xsl:call-template name="isMatch">
        <xsl:with-param name="name" select="$name1"/>
        <xsl:with-param name="list" select="$allNames"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ok">
      <xsl:choose>
        <xsl:when test="$match != 1">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="$p3IncludeDisabled = 1">
          <xsl:value-of select="1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="isEnabled">
            <xsl:with-param name="sample" select="."/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="$ok = 1">
      <xsl:copy-of select="."/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="Table" priority="100">
    <xsl:param name="allNames"/>
    <Table>
      <xsl:apply-templates>
        <xsl:with-param name="allNames" select="$allNames"/>
      </xsl:apply-templates>
    </Table>
  </xsl:template>


  <!--  Heading templates -->

  <xsl:template match="NewFileName" priority="100"/>
  <xsl:template match="NewFileCreationTime" priority="100"/>
  <xsl:template match="SaveHistory" priority="100"/>

  <xsl:template match="FileName" priority="100">
    <xsl:copy-of select="."/>
    <xsl:if test="string-length($outputFile)">
      <xsl:text>&#10;</xsl:text>
      <NewFileName>
        <xsl:value-of select="$outputFile"/>
      </NewFileName>
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="CreationTime" priority="100">
    <xsl:variable name="timeNow" select="date:seconds()"/>
    <xsl:variable name="saveHist" select="../SaveHistory"/>
    <xsl:copy-of select="."/>
    <xsl:text>&#10;</xsl:text>
    <NewFileCreationTime>
      <xsl:value-of select="$timeNow"/>
    </NewFileCreationTime>
    <xsl:text>&#10;</xsl:text>
    <SaveHistory>
      <xsl:text>&#10;</xsl:text>
      <Time>
        <xsl:value-of select="$timeNow"/>
      </Time>
      <xsl:text>&#10;</xsl:text>
      <xsl:if test="$saveHist">
        <xsl:for-each select="$saveHist/Time">
          <xsl:copy-of select="."/>
        </xsl:for-each>
      </xsl:if>
    </SaveHistory>
  </xsl:template>

  <xsl:template match="Heading" priority="100">
    <Heading>
      <xsl:apply-templates/>
    </Heading>
  </xsl:template>

  <xsl:template match="OsirisAnalysisReport" priority="100">
    <xsl:param name="allNames"/>
    <OsirisAnalysisReport>
      <xsl:apply-templates>
        <xsl:with-param name="allNames" select="$allNames"/>
      </xsl:apply-templates>
    </OsirisAnalysisReport>
  </xsl:template>

  <xsl:template match="/">
    <xsl:variable name="allNames">
      <xsl:call-template name="searchList"/>
    </xsl:variable>
    <xsl:if test="string-length($allNames)">
      <xsl:apply-templates>
        <xsl:with-param name="allNames" select="$allNames"/>
      </xsl:apply-templates>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
