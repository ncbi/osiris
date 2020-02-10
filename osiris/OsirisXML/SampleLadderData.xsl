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
#  FileName: SampleLadderData.xsl
#  Author:   Douglas Hoffman
#  Description:  Export allele info from OSIRIS analysis file to a
#   tab-delimited text file
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:func="http://exslt.org/functions"
  xmlns:exsl="http://exslt.org/common"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  extension-element-prefixes="func exsl os"
  version="1.0">

  <xsl:import href="util.xsl"/>
  <xsl:output method="xml" indent="yes" standalone="yes" encoding="UTF-8" version="1.0"/>
  <xsl:strip-space elements="*"/>



  <xsl:template match="samplePeak" mode="SLDfixplot" priority="20">
    <xsl:param name="xfixloci"/>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:copy-of select="*"/>
      <xsl:variable name="channel" select="number(../nr)"/>
      <xsl:variable name="ilschannel" select="number(../../ilsChannel)"/>
      <xsl:if test="$channel != $ilschannel">
        <xsl:variable name="mean" select="mean"/>
        <locus>
          <xsl:value-of select="$xfixloci/locus[channel = $channel and $mean &gt;= startx and $mean &lt;= endx][1]/name"/>
        </locus>
      </xsl:if>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="*" mode="SLDfixplot" priority="10">
    <xsl:param name="xfixloci"/>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates mode="SLDfixplot">
        <xsl:with-param name="xfixloci" select="$xfixloci"/>
      </xsl:apply-templates>
    </xsl:copy>
  </xsl:template>

  <xsl:template name="SLDfixloci">
    <xsl:variable name="tmploci">
      <xsl:for-each select="locus">
        <xsl:sort select="channel" data-type="number" order="ascending"/>
        <xsl:sort select="start"  data-type="number" order="ascending"/>
        <xsl:copy-of select="."/>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="loci" select="exsl:node-set($tmploci)"/>
    <xsl:variable name="nCount" select="count($loci/locus)"/>
    <xsl:for-each select="$loci/locus">
      <xsl:variable name="ndx" select="position()"/>
      <xsl:copy>
        <xsl:copy-of select="@*"/>
        <xsl:copy-of select="*"/>
        <startx>
          <xsl:choose>
            <xsl:when test="$ndx = 1">
              <xsl:value-of select="1"/>
            </xsl:when>
            <xsl:when test="$loci/locus[$ndx - 1]/channel != channel">
              <xsl:value-of select="1"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="($loci/locus[$ndx - 1]/end + start) * 0.5"/>
            </xsl:otherwise>
          </xsl:choose>
        </startx>
        <endx>
          <xsl:choose>
            <xsl:when test="$ndx = $nCount">
              <xsl:value-of select="end + 1000000"/>
            </xsl:when>
            <xsl:when test="$loci/locus[$ndx + 1]/channel != channel">
              <xsl:value-of select="end + 1000000"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="($loci/locus[$ndx + 1]/start + end) * 0.5"/>
            </xsl:otherwise>
          </xsl:choose>
        </endx>
      </xsl:copy>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="SLDfixplot">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:copy-of select="*[name(.) != 'locus' and name(.) != 'channel']"/>
      <xsl:variable name="tmpfixloci">
        <xsl:call-template name="SLDfixloci"/>
      </xsl:variable>
      <xsl:variable name="xfixloci" select="exsl:node-set($tmpfixloci)"/>
      <xsl:for-each select="channel">
        <xsl:copy>
          <xsl:copy-of select="@*"/>
          <xsl:apply-templates mode="SLDfixplot">
            <xsl:with-param name="xfixloci" select="$xfixloci"/>
          </xsl:apply-templates>
        </xsl:copy>
      </xsl:for-each>
      <xsl:copy-of select="$xfixloci/locus"/>
    </xsl:copy>
  </xsl:template>

  <func:function name="os:fixplot">
    <xsl:param name="plt"/>
    <xsl:choose>
      <xsl:when test="not($plt)">
        <func:result select="$plt"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="result">
          <xsl:for-each select="$plt/plt">
            <xsl:call-template name="SLDfixplot"/>
          </xsl:for-each>
        </xsl:variable>
        <func:result select="exsl:node-set($result)"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  <xsl:template name="SLDpltPeakToOarAllele">
    <Allele>
      <Id>0</Id>
      <Name><xsl:value-of select="allele"/></Name>
      <BPS><xsl:value-of select="BPS"/></BPS>
      <RFU><xsl:value-of select="height"/></RFU>
      <meanbps><xsl:value-of select="meanbps"/></meanbps>
      <PeakArea><xsl:value-of select="PeakArea"/></PeakArea>
      <Width><xsl:value-of select="Width"/></Width>
      <Time><xsl:value-of select="mean"/></Time>
      <Fit><xsl:value-of select="fit"/></Fit>
      <OffLadder>false</OffLadder>
    </Allele>
  </xsl:template>

  <xsl:template match="Sample[Type='Ladder']/Locus[not(Allele)]" mode="SampleLadderData" priority="20">
    <xsl:param name="plt"/>
    <xsl:variable name="LocusName" select="string(LocusName)"/>
    <xsl:variable name="pltLocus" select="$plt/plt/locus[name = $LocusName]"/>
    <xsl:variable name="nChannel" select="string($pltLocus/channel)"/>
    <xsl:variable name="nStart" select="number($pltLocus/startx)"/>
    <xsl:variable name="nStop" select="number($pltLocus/endx)"/>
    <xsl:variable name="xPeak"
      select="$plt/plt/channel[nr = $nChannel]/samplePeak[locus = $LocusName]"/>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:copy-of select="*"/>
      <xsl:for-each select="$xPeak">
        <xsl:call-template name="SLDpltPeakToOarAllele"/>
      </xsl:for-each>
    </xsl:copy>
  </xsl:template>

  <func:function name="os:fixurl">
    <xsl:param name="url"/>
    <xsl:variable name="r" select="os:str-replace($url, ' ', '%20')"/>
    <func:result select="$r"/>
  </func:function>

  <xsl:template name="SampleLadderData" match="Sample[Type='Ladder']" mode="SampleLadderData" priority="20">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:variable name="pltfile" select="os:fixurl(concat(Name,'.plt'))"/>
      <xsl:variable name="plt0" select="document($pltfile,.)"/>
      <xsl:variable name="plt" select="os:fixplot($plt0)"/>
      <xsl:apply-templates mode="SampleLadderData" priority ="20">
        <xsl:with-param name="plt" select="$plt"/>
      </xsl:apply-templates>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="*" priority="10" mode="SampleLadderData">
    <xsl:param name="plt"/>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates mode="SampleLadderData">
        <xsl:with-param name="plt"/>
      </xsl:apply-templates>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="/">
    <xsl:apply-templates mode="SampleLadderData"/>
  </xsl:template>

</xsl:stylesheet>
