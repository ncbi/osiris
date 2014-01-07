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
#  FileName: NeedsAttnOAR.xsl
#  Author:   Douglas Hoffman
#  Description:  Report whether or not a .oar (OSIRIS report) file
#    needs attention.  This will NOT be accurate if used with
#    a .oer file.
#
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml"/>
  
  <xsl:key name="messagesEnabled"
    match="/OsirisAnalysisReport/Messages/Message[not(Hidden = 'true') and not(Enabled = 'false')]"
    use="MessageNumber"/>

  <xsl:variable name="directoryMessages"
    select="/OsirisAnalysisReport/DirectoryAlerts//MessageNumber"/>
  <xsl:variable name="sampleMessages"
    select="/OsirisAnalysisReport/Table/Sample/SampleAlerts/MessageNumber"/>
  <xsl:variable name="locusMessages"
    select="/OsirisAnalysisReport/Table/Sample/Locus/LocusAlerts/MessageNumber"/>
  <xsl:variable name="locusAlleleMessages"
    select="/OsirisAnalysisReport/Table/Sample/Locus/Allele[not(Disabled) = 'true']/MessagesNumber"/>
  <xsl:variable name="ilsMessages"
    select="/OsirisAnalysisReport/Table/Sample/ILSAlerts/MessageNumber"/>
  <xsl:variable name="channelMessages"
    select="/OsirisAnalysisReport/Table/Sample/ChannelAlerts/Channel/MessageNumber"/>


  <xsl:variable name="AcceptanceDefault" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance"/>
  <xsl:variable name="AcceptanceLocus" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance1"/>
  <xsl:variable name="AcceptanceChannel" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance2"/>
  <xsl:variable name="AcceptanceILS" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance3"/>
  <xsl:variable name="AcceptanceSample" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance4"/>
  <xsl:variable name="AcceptanceDir" select="/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts/Acceptance5"/>



  <xsl:variable name="messages"
      select="/OsirisAnalysisReport/Messages/Message[not(Hidden = 'true') and not(Enabled = 'false')]"/>
  <xsl:variable name="messageCount" select="count($messages)"/>


  <xsl:template name="GetAcceptance">
    <xsl:param name="Accept"/>
    <xsl:choose>
      <xsl:when test="string(number($Accept)) != 'NaN'">
        <xsl:value-of select="$Accept"/>
      </xsl:when>
      <xsl:when test="string(number($AcceptanceDefault)) != 'NaN'">
        <xsl:value-of select="$AcceptanceDefault"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="checkMessages">
    <xsl:param name="list"/>
    <xsl:if test="$list">
      <xsl:value-of select="count($list[key('messagesEnabled',.)])"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="NeedsAttnOAR">
    <xsl:param name="keys"/>
    
    <xsl:variable name="key" select="substring-before($keys,':')"/>
    <xsl:variable name="next" select="substring-after($keys,':')"/>

    <xsl:variable name="x">
      <xsl:choose>
        <xsl:when test="$key = 'directoryMessage'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$directoryMessages"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$key = 'sampleBySample'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$sampleMessages"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$key = 'sampleByChannel'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$channelMessages"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$key = 'sampleByILS'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$ilsMessages"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$key = 'locusByLocus'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$locusMessages"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$key = 'locusByAllele'">
          <xsl:call-template name="checkMessages">
            <xsl:with-param name="list" select="$locusAlleleMessages"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$x &gt; 0">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when test="string-length($next)">
        <xsl:call-template name="NeedsAttnOAR">
          <xsl:with-param name="keys" select="$next"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!--
  <xsl:template name="dumpAll">
    <xsl:param name="title"/>
    <xsl:param name="list"/>
    <xsl:message>
      <xsl:text>***************************************** </xsl:text>
      <xsl:value-of select="$title"/>
    </xsl:message>
    <xsl:for-each select="$list">
      <xsl:message>
        <xsl:value-of select="."/>
      </xsl:message>
    </xsl:for-each>
  </xsl:template>
  -->
  <xsl:template match="/">
    <xsl:variable name="keyNames">
      <xsl:variable name="dir">
        <xsl:call-template name="GetAcceptance">
          <xsl:with-param name="Accept" select="$AcceptanceDir"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="sample">
        <xsl:call-template name="GetAcceptance">
          <xsl:with-param name="Accept" select="$AcceptanceSample"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="locus">
        <xsl:call-template name="GetAcceptance">
          <xsl:with-param name="Accept" select="$AcceptanceLocus"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="ils">
        <xsl:call-template name="GetAcceptance">
          <xsl:with-param name="Accept" select="$AcceptanceILS"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="channel">
        <xsl:call-template name="GetAcceptance">
          <xsl:with-param name="Accept" select="$AcceptanceChannel"/>
        </xsl:call-template>
      </xsl:variable>
      <!--
      <xsl:message>
        <xsl:text>dir=</xsl:text>
        <xsl:value-of select="$dir"/>
        <xsl:text> sample=</xsl:text>
        <xsl:value-of select="$sample"/>
        <xsl:text> channel=</xsl:text>
        <xsl:value-of select="$channel"/>
        <xsl:text> ils=</xsl:text>
        <xsl:value-of select="$ils"/>
        <xsl:text> locus=</xsl:text>
        <xsl:value-of select="$locus"/>
      </xsl:message>
      -->
      <xsl:if test="$dir &gt; 0">
        <xsl:text>directoryMessage:</xsl:text>
      </xsl:if>
      <xsl:if test="$sample &gt; 0">
        <xsl:text>sampleBySample:</xsl:text>
      </xsl:if>
      <xsl:if test="$channel &gt; 0">
        <xsl:text>sampleByChannel:</xsl:text>
      </xsl:if>
      <xsl:if test="$ils &gt; 0">
        <xsl:text>sampleByILS:</xsl:text>
      </xsl:if>
      <xsl:if test="$locus &gt; 0">
        <xsl:text>locusByLocus:</xsl:text>
        <xsl:text>locusByAllele:</xsl:text>
      </xsl:if>
    </xsl:variable>
    <xsl:variable name="need">
      <xsl:choose>
        <xsl:when test="$keyNames = ''"/>
        <xsl:when test="$messageCount = 0"/>
        <xsl:when test="$messageCount &gt; 99999">
          <xsl:value-of select="$messageCount"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="NeedsAttnOAR">
            <xsl:with-param name="keys" select="$keyNames"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <!--
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$directoryMessages"/>
      <xsl:with-param name="title" select="'Directory'"/>
    </xsl:call-template>
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$sampleMessages"/>
      <xsl:with-param name="title" select="'Sample'"/>
    </xsl:call-template>
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$locusMessages"/>
      <xsl:with-param name="title" select="'locus'"/>
    </xsl:call-template>
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$locusAlleleMessages"/>
      <xsl:with-param name="title" select="'locusAllele'"/>
    </xsl:call-template>
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$ilsMessages"/>
      <xsl:with-param name="title" select="'ils'"/>
    </xsl:call-template>
    <xsl:call-template name="dumpAll">
      <xsl:with-param name="list" select="$channelMessages"/>
      <xsl:with-param name="title" select="'channel'"/>
    </xsl:call-template>
    -->
    <xml>
      <xsl:if test="string-length($need) != 0">
        <xsl:value-of select="1"/>
      </xsl:if>
    </xml>
  </xsl:template>
</xsl:stylesheet>
