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
#  FileName: exportArtifacts.xsl
#  Author:   Douglas Hoffman
#  Description:  Export subset of samples from an OSIRIS report to 
#    another OSIRIS report selecting samples by searching message text 
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:str="http://exslt.org/strings"
  xmlns:date="http://exslt.org/dates-and-times"
  extension-element-prefixes="str date"
  exclude-result-prefixes="xsl date str"
  version="1.0">

  <xsl:output method="xml"/>
  <xsl:key name="messageFind" match="/OsirisAnalysisReport/Messages/Message[not(Hidden = 'true')]/Text" use="../MessageNumber"/>
  <xsl:param name="outputFile"/>
  <xsl:param name="artifactSearch"/>
  <xsl:param name="artifactXclude"/>
  <xsl:param name="zCriticalOnly" select="1"/>

  <xsl:param name="names"/>
  <xsl:param name="p1IncludeControls" select="1"/>
  <xsl:param name="p2IncludeLadders" select="1"/>
  <xsl:param name="p3IncludeDisabled" select="0"/>

  <xsl:variable name="delim" select="','"/>
  <xsl:variable name="UPPER" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
  <xsl:variable name="lower" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="maxLevel">
    <xsl:choose>
      <xsl:when test="$zCriticalOnly = 0">
        <xsl:value-of select="100"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="19"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

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

  <xsl:template name="trimList">
    <xsl:param name="str"/>
    <xsl:choose>
      <xsl:when test="contains($str,$delim)">
        <xsl:variable name="leftOfIt">
          <xsl:call-template name="trim">
            <xsl:with-param name="str" select="substring-before($str,$delim)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$leftOfIt != ''">
          <xsl:value-of select="$leftOfIt"/>
          <xsl:value-of select="$delim"/>
        </xsl:if>
        <xsl:call-template name="trimList">
          <xsl:with-param name="str" select="substring-after($str,$delim)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="trim">
          <xsl:with-param name="str" select="$str"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="trimListLower">
    <xsl:param name="str"/>
    <xsl:variable name="a" select="translate($str,$UPPER,$lower)"/>
    <xsl:call-template name="trimList">
      <xsl:with-param name="str" select="$a"/>
    </xsl:call-template>
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
    <xsl:call-template name="trimListLower">
      <xsl:with-param name="str" select="translate($search1,$UPPER,$lower)"/>
    </xsl:call-template>
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
      <xsl:choose>
        <xsl:when test="$hasDelim = 1">
          <xsl:value-of select="substring-before($list,$delim)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$list"/>
        </xsl:otherwise>
      </xsl:choose>
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

  <xsl:template name="concatMessages">
    <xsl:param name="node"/>
    <xsl:variable name="sRtn">
      <xsl:for-each select="$node/MessageNumber">
        <xsl:variable name="text">
          <xsl:call-template name="trim">
            <xsl:with-param name="str" select="key('messageFind',.)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="string-length($text) != 0">
          <xsl:value-of select="$text"/>
          <xsl:text> </xsl:text>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>
    <xsl:call-template name="trim">
      <xsl:with-param name="str" select="$sRtn"/>
    </xsl:call-template>
  </xsl:template>


  <xsl:template name="searchMessages">
    <xsl:param name="nodes"/>
    <xsl:param name="includeList"/>
    <xsl:param name="excludeList"/>
    <xsl:param name="nFirst" select="1"/>
    <xsl:param name="nLast"/>

    <xsl:choose>
      <xsl:when test="$nFirst = $nLast">
        <xsl:variable name="messageText">
          <xsl:call-template name="concatMessages">
            <xsl:with-param name="node" select="$nodes[$nFirst]"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:message>
          <xsl:text>    </xsl:text>
          <xsl:value-of select="$messageText"/>
        </xsl:message>

        <xsl:variable name="messageTextL"
          select="translate(string($messageText),$UPPER,$lower)"/>
            
        <xsl:variable name="matchExclude">
          <xsl:choose>
            <xsl:when test="$excludeList = ''">
              <xsl:value-of select="0"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="isMatch">
                <xsl:with-param name="name" select="$messageTextL"/>
                <xsl:with-param name="list" select="$excludeList"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

            
        <xsl:variable name="matchInclude">
          <xsl:choose>
            <xsl:when test="$matchExclude = '1'">
              <xsl:value-of select="0"/>
            </xsl:when>
            <xsl:when test="$includeList = ''">
              <xsl:value-of select="1"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="isMatch">
                <xsl:with-param name="name" select="$messageTextL"/>
                <xsl:with-param name="list" select="$includeList"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:value-of select="$matchInclude"/>
      </xsl:when>
      <xsl:when test="$nFirst &lt; $nLast">
        <xsl:variable name="mid" select="floor(($nFirst + $nLast) * 0.5)"/>
        <xsl:variable name="x1">
          <xsl:call-template name="searchMessages">
            <xsl:with-param name="nodes" select="$nodes"/>
            <xsl:with-param name="includeList" select="$includeList"/>
            <xsl:with-param name="excludeList" select="$excludeList"/>
            <xsl:with-param name="nFirst" select="$nFirst"/>
            <xsl:with-param name="nLast" select="$mid"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$x1 = 1">
            <xsl:value-of select="$x1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="searchMessages">
              <xsl:with-param name="nodes" select="$nodes"/>
              <xsl:with-param name="includeList" select="$includeList"/>
              <xsl:with-param name="excludeList" select="$excludeList"/>
              <xsl:with-param name="nFirst" select="$mid + 1"/>
              <xsl:with-param name="nLast" select="$nLast"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
  </xsl:template>




  <xsl:template name="checkSample">
    <xsl:param name="sample"/>
    <xsl:param name="list"/>
    <xsl:param name="exclude"/>

    <xsl:message>
      <xsl:text>&#10;Sample: </xsl:text>
      <xsl:value-of select="Name"/>
    </xsl:message>
    <xsl:variable name="messageParents"
      select="$sample//*[(name() != 'OldAllele') and (count(./Level[. &gt; $maxLevel]) = 0) and (count(./MessageNumber) != 0) and not(./Disabled = 'true')]"/>
    <xsl:variable name="nodeCount" select="count($messageParents)"/>
    <xsl:variable name="include">
      <xsl:call-template name="searchMessages">
        <xsl:with-param name="nodes" select="$messageParents"/>
        <xsl:with-param name="includeList" select="$list"/>
        <xsl:with-param name="excludeList" select="$exclude"/>
        <xsl:with-param name="nLast" select="$nodeCount"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$include"/>
  </xsl:template>

  <!-- default template match, just copy the node -->

  <xsl:template match="*" priority="1">
    <xsl:text>&#10;</xsl:text>
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
    <xsl:param name="include"/>
    <xsl:param name="exclude"/>

    <xsl:variable name="isEnabled">
      <xsl:choose>
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

    <xsl:variable name="matchName">
      <xsl:choose>
        <xsl:when test="$isEnabled = 0">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="name1" 
            select="translate(Name,$UPPER,$lower)"/>
          <xsl:call-template name="isMatch">
            <xsl:with-param name="name" select="$name1"/>
            <xsl:with-param name="list" select="$allNames"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="ok">
      <xsl:choose>
        <xsl:when test="$matchName = 1">
          <xsl:value-of select="1"/>
        </xsl:when>
        <xsl:when test="$isEnabled = 0">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="checkSample">
            <xsl:with-param name="sample" select="."/>
            <xsl:with-param name="list" select="$include"/>
            <xsl:with-param name="exclude" select="$exclude"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
      
    <xsl:if test="$ok = 1">
      <xsl:text>&#10;</xsl:text>
      <xsl:copy-of select="."/>
    </xsl:if>

  </xsl:template>

  <xsl:template match="Table" priority="100">
    <xsl:param name="allNames"/>
    <xsl:param name="include"/>
    <xsl:param name="exclude"/>
    <xsl:text>&#10;</xsl:text>
    <Table>
      <xsl:apply-templates>
        <xsl:with-param name="allNames" select="$allNames"/>
        <xsl:with-param name="include" select="$include"/>
        <xsl:with-param name="exclude" select="$exclude"/>
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
    <xsl:text>&#10;</xsl:text>
    <Heading>
      <xsl:apply-templates/>
    </Heading>
  </xsl:template>

  <xsl:template match="OsirisAnalysisReport" priority="100">
    <xsl:param name="allNames"/>
    <xsl:param name="include"/>
    <xsl:param name="exclude"/>
    <OsirisAnalysisReport>
      <xsl:text>&#10;</xsl:text>
      <xsl:apply-templates>
        <xsl:with-param name="allNames" select="$allNames"/>
        <xsl:with-param name="include" select="$include"/>
        <xsl:with-param name="exclude" select="$exclude"/>
      </xsl:apply-templates>
      <xsl:text>&#10;</xsl:text>
    </OsirisAnalysisReport>
  </xsl:template>


  <xsl:template match="/">
    <xsl:variable name="aSearch">
      <xsl:call-template name="trimListLower">
        <xsl:with-param name="str" select="$artifactSearch"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aExclude">
      <xsl:call-template name="trimListLower">
        <xsl:with-param name="str" select="$artifactXclude"/>
      </xsl:call-template>
    </xsl:variable>

    
    <xsl:variable name="allNames">
      <xsl:call-template name="searchList"/>
    </xsl:variable>
    <xsl:apply-templates>
      <xsl:with-param name="allNames" select="$allNames"/>
      <xsl:with-param name="include" select="$aSearch"/>
      <xsl:with-param name="exclude" select="$aExclude"/>
    </xsl:apply-templates>
  </xsl:template>

</xsl:stylesheet>
