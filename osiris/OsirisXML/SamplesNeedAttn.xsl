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
#  FileName: SamplesNeedsAttn.xsl
#  Author:   Douglas Hoffman
#  Description:  Functions/Templates to determine if 
#    a sample needs attention, whether it needs edit
#    approval or acceptance
#
#  change - 2/24/12 - loop for each sample
#   if it needs review for editing then reject
#   else
#   if it has UD3 only and need acceptance then reject
#
#   When rejecting, indicate primary or secondary file
#
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

  <!--
       test for attention needed for the following
       conditions: Sample, ILS, Channel, Locus
  -->

  <xsl:import href="util.xsl"/>
  <xsl:output method="text"/>
  <xsl:strip-space elements="*"/>
  <xsl:param name="xmlfile" select="'/home/hoffman/junk/isp-xsl/STRbaseIF.xml'"/>

  <!-- EOL -->

  <xsl:variable name="attnEOL" select="'&#13;&#10;'"/>
  <xsl:variable name="lenattnEOL" select="string-length($attnEOL)"/>


  <xsl:template name="GetNumber">
    <xsl:param name="nPreferred" select="'x'"/>
    <xsl:param name="nDefault" select="'x'"/>
    <xsl:choose>
      <xsl:when test="string(number($nPreferred)) != 'NaN'">
        <xsl:value-of select="$nPreferred"/>
      </xsl:when>
      <xsl:when test="string(number($nDefault)) != 'NaN'">
        <xsl:value-of select="$nDefault"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:getNumber">
    <xsl:param name="nPreferred"/>
    <xsl:param name="nDefault"/>
    <xsl:variable name="s">
      <xsl:call-template name="GetNumber">
        <xsl:with-param name="nPreferred" select="$nDefault"/>
        <xsl:with-param name="nDefault" select="$nDefault"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="number($s)"/>
  </func:function>

  <func:function name="os:hasUD12">
    <xsl:param name="sample"/>
    <xsl:variable name="rtn"
      select="(count($sample//UD1) != 0) and (count($sample//UD2) != 0)"/>
    <func:result select="$rtn"/>
  </func:function>

  <xsl:template name="HasUD3">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when test="$sample//UD3">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:hasUD3">
    <xsl:param name="sample"/>
    <xsl:variable name="nRtn">
      <xsl:call-template name="HasUD3">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="boolean(number($nRtn))"/>
  </func:function>

  <func:function name="os:MsgFound">
    <xsl:param name="msgNumbers"/>
    <xsl:param name="messages"/>
    <xsl:variable name="editCount"
      select="count($msgNumbers[ os:InList($messages,string(.)) ])"/>
    <func:result select="boolean($editCount)"/>
  </func:function>


  <func:function name="os:SomethingNeedsAttn">
    <!-- return true if:
         UD1 and UD2 are empty AND
         (editing has been done w/o required review OR
         UD3 is present w/o required acceptance.
         For efficiency, it is the responsibility of the
         calling template or function to determine if the
         sample does not have UD1 nor UD2
    -->
    <xsl:param name="sampleLims"/>
    <xsl:param name="nNoteCount"/>
    <xsl:param name="msgNumbers"/>
    <xsl:param name="messages"/>
    <xsl:param name="nAcceptCount"/>
    <xsl:param name="nReviewCount"/>
    <xsl:param name="nAcceptRequired"/>
    <xsl:param name="nReviewRequired"/>

    <xsl:variable name="rtn">
      <xsl:text>0</xsl:text>
      <xsl:choose>
        <xsl:when
          test="($nNoteCount &gt; 0) and 
                ($nReviewCount &lt; $nReviewRequired)">
          <!-- this has been edited -->
          <xsl:value-of select="1"/>
        </xsl:when>
        <xsl:when
          test="os:hasUD3($sampleLims) and
                os:MsgFound($msgNumbers,$messages)">
          <!-- this has alerts and UD3  -->
          <xsl:if test="$nAcceptCount &lt; $nAcceptRequired">
            <xsl:value-of select="1"/>
          </xsl:if>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <func:function name="os:LocusNeedsAttn">
    <xsl:param name="sampleLims"/>
    <xsl:param name="Locus"/>
    <xsl:param name="ChannelAlerts"/>
    <xsl:param name="nAcceptReqd"/>
    <xsl:param name="nReviewReqd"/>
    <xsl:param name="messages"/>
    <xsl:variable name="nReview" select="count($Locus/ReviewList/Review)"/>
    <xsl:variable name="nAccept" select="count($Locus/AcceptanceList/Acceptance)"/>
    <xsl:variable name="sLocus" select="string($Locus/LocusName)"/>
    <xsl:variable name="nEdits"
      select="count($Locus/Notes) + count($ChannelAlerts/Channel/OldArtifact/Allele[Locus = $sLocus])"/>
    <!-- counting old artifact because of bug OS-257 -->
    <xsl:variable name="msgNumbers" select="$Locus/*/MessageNumber"/>
    <xsl:variable name="rtn" 
      select="os:SomethingNeedsAttn(
              $sampleLims, 
              $nEdits,
              $msgNumbers,
              $messages,
              $nAccept,
              $nReview,
              $nAcceptReqd,
              $nReviewReqd)"/>
    <func:result select="$rtn"/>
  </func:function>

  <xsl:template name="AnyLocusNeedsAttn">
    <xsl:param name="sampleLims"/>
    <xsl:param name="Locus"/>
    <xsl:param name="ChannelAlerts"/>
    <xsl:param name="nAcceptReqd"/>
    <xsl:param name="nReviewReqd"/>
    <xsl:param name="messages"/>
    <xsl:param name="nFirst" select="1"/>
    <xsl:param name="nLast" select="count($Locus)"/>

    <xsl:choose>
      <xsl:when test="$nFirst &lt; $nLast">
        <!-- divide and conquer -->
        <xsl:variable name="mid" select="floor(($nFirst + $nLast) * 0.5)"/>
        <xsl:variable name="try1">
          <xsl:call-template name="AnyLocusNeedsAttn">
            <xsl:with-param name="sampleLims" select="$sampleLims"/>
            <xsl:with-param name="Locus" select="$Locus"/>
            <xsl:with-param name="ChannelAlerts" select="$ChannelAlerts"/>
            <xsl:with-param name="nAcceptReqd" select="$nAcceptReqd"/>
            <xsl:with-param name="nReviewReqd" select="$nReviewReqd"/>
            <xsl:with-param name="messages" select="$messages"/>
            <xsl:with-param name="nFirst" select="$nFirst"/>
            <xsl:with-param name="nLast" select="$mid"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$try1 = '1'">
            <xsl:value-of select="$try1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="AnyLocusNeedsAttn">
              <xsl:with-param name="sampleLims" select="$sampleLims"/>
              <xsl:with-param name="Locus" select="$Locus"/>
              <xsl:with-param name="ChannelAlerts" select="$ChannelAlerts"/>
              <xsl:with-param name="nAcceptReqd" select="$nAcceptReqd"/>
              <xsl:with-param name="nReviewReqd" select="$nReviewReqd"/>
              <xsl:with-param name="messages" select="$messages"/>
              <xsl:with-param name="nFirst" select="$mid + 1"/>
              <xsl:with-param name="nLast" select="$nLast"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$nFirst = $nLast">
        <xsl:if test="os:LocusNeedsAttn(
                      $sampleLims,
                      $Locus[$nFirst],
                      $ChannelAlerts,
                      $nAcceptReqd,
                      $nReviewReqd,
                      $messages)">
          <xsl:text>1</xsl:text>
        </xsl:if>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ThisSampleNeedsAttn">
    <xsl:param name="sample"/>
    <xsl:param name="sampleLims"/>
    <xsl:param name="AcceptanceLocus"/>
    <xsl:param name="AcceptanceChannel"/>
    <xsl:param name="AcceptanceILS"/>
    <xsl:param name="AcceptanceSample"/>
    <!--xsl:param name="AcceptanceDir"/-->
    <xsl:param name="ReviewLocus"/>
    <xsl:param name="ReviewChannel"/>
    <xsl:param name="ReviewILS"/>
    <xsl:param name="ReviewSample"/>
    <!--xsl:param name="ReviewDir"/-->
    <xsl:param name="messages"/>

    <xsl:choose>
      <xsl:when 
        test="os:SomethingNeedsAttn(
              $sampleLims,
              count($sample/SampleNotes),
              $sample/SampleAlerts/MessageNumber,
              $messages,
              count($sample/Acceptance/SampleAcceptance/Acceptance),
              count($sample/Reviews/SampleReview/Review),
              $AcceptanceSample,
              $ReviewSample)">

        <!-- Sample level needs review/acceptance -->

        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when 
        test="os:SomethingNeedsAttn(
              $sampleLims,
              count($sample/ILSNotes),
              $sample/ILSAlerts/MessageNumber,
              $messages,
              count($sample/Acceptance/ILSAcceptance/Acceptance),
              count($sample/Reviews/ILSReview/Review),
              $AcceptanceILS,
              $ReviewILS)">

        <!-- ILS needs review/acceptance -->
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when 
        test="os:SomethingNeedsAttn(
              $sampleLims,
              count($sample/ChannelNotes),
              $sample/ChannelAlerts/ChannelAcceptance/MessageNumber,
              $messages,
              count($sample/Acceptance/Channel/Acceptance),
              count($sample/Reviews/ChannelReview/Review),
              $AcceptanceChannel,
              $ReviewChannel)">
        <!-- channel needs review/acceptance -->
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="AnyLocusNeedsAttn">
          <xsl:with-param name="sampleLims" select="$sampleLims"/>
          <xsl:with-param name="Locus" select="$sample/Locus"/>
          <xsl:with-param name="ChannelAlerts" select="$sample/ChannelAlerts"/>
          <xsl:with-param name="nAcceptReqd" select="$AcceptanceLocus"/>
          <xsl:with-param name="nReviewReqd" select="$ReviewLocus"/>
          <xsl:with-param name="messages" select="$messages"/>
        </xsl:call-template>
      </xsl:otherwise>

    </xsl:choose>

  </xsl:template>

  <func:function name="os:getMessages">
    <xsl:param name="doc"/>
    <xsl:choose>
      <xsl:when test="$doc and $doc/OsirisAnalysisReport">
        <xsl:variable name="rtn"
          select="$doc/OsirisAnalysisReport/Messages/Message[not(Hidden = 'true') and not(Enabled = 'false')]/MessageNumber"/>
        <func:result select="$rtn"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="exsl:node-set('')"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  <func:function name="os:getNodesFromType">
    <xsl:param name="sFrom"/>
    <xsl:param name="nodesPrimary"/>
    <xsl:param name="nodesSecondary"/>
    <xsl:choose>
      <xsl:when test="$sFrom = 'Secondary'">
        <func:result select="$nodesSecondary"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="$nodesPrimary"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>
  
  <xsl:template name="AnySampleNeedsAttn">
    <xsl:param name="docPrimary" select="/"/>
    <xsl:param name="docSecondary" select="false()"/>
    <xsl:param name="limsxml"/>

    <xsl:variable name="RevAccCounts"
      select="$docPrimary/OsirisAnalysisReport/Heading/LabSettings/ReviewAcceptanceCounts"/>
    <xsl:variable name="nCountNames"
      select="count($limsxml/directory/sample)"/>
    <xsl:if test="$RevAccCounts and ($nCountNames &gt; 0)">

      <xsl:variable name="AcceptanceDefault"
        select="$RevAccCounts/Acceptance"/>
      <xsl:variable name="AcceptanceLocus"
        select="os:getNumber($RevAccCounts/Acceptance1,$AcceptanceDefault)"/>
      <xsl:variable name="AcceptanceChannel"
        select="os:getNumber($RevAccCounts/Acceptance2,$AcceptanceDefault)"/>
      <xsl:variable name="AcceptanceILS"
        select="os:getNumber($RevAccCounts/Acceptance3,$AcceptanceDefault)"/>
      <xsl:variable name="AcceptanceSample"
        select="os:getNumber($RevAccCounts/Acceptance4,$AcceptanceDefault)"/>
      <!--xsl:variable name="AcceptanceDir"
           select="os:getNumber($RevAccCounts/Acceptance5,$AcceptanceDefault)"/-->

      <xsl:variable name="ReviewDefault"
        select="$RevAccCounts/Reviews"/>
      <xsl:variable name="ReviewLocus"
        select="os:getNumber($RevAccCounts/Reviews1,$ReviewDefault)"/>
      <xsl:variable name="ReviewChannel"
        select="os:getNumber($RevAccCounts/Reviews2,$ReviewDefault)"/>
      <xsl:variable name="ReviewILS"
        select="os:getNumber($RevAccCounts/Reviews3,$ReviewDefault)"/>
      <xsl:variable name="ReviewSample"
        select="os:getNumber($RevAccCounts/Reviews4,$ReviewDefault)"/>
      <!--xsl:variable name="ReviewDir"
           select="os:getNumber($RevAccCounts/Reviews5,$ReviewDefault)"/-->

      <xsl:variable name="messagesPrimary"
        select="os:getMessages($docPrimary)"/>
      <xsl:variable name="messagesSecondary"
        select="os:getMessages($docSecondary)"/>

      <xsl:variable name="strList1">
        <xsl:for-each select="$limsxml/directory/sample">
          <xsl:variable name="sfName" select="string(@file)"/>
          <xsl:variable name="sFrom" select="string(@from)"/>
          <xsl:variable name="doc"
            select="os:getNodesFromType($sFrom, $docPrimary,$docSecondary)"/>
          <xsl:variable name="messages"
            select="os:getNodesFromType($sFrom, $messagesPrimary, $messagesSecondary)"/>
          <xsl:variable name="docSample"
            select="$doc/OsirisAnalysisReport/Table/Sample[Name = $sfName]"/>
          <xsl:choose>
            <xsl:when test="$docSample">
              <xsl:variable name="nNeedsAttn">
                <xsl:if test="not(os:hasUD12(.))">
                  <!--  OK to export if UD1 and UD2 are present -->
                  <xsl:call-template name="ThisSampleNeedsAttn">
                    <xsl:with-param name="sample" select="$docSample"/>
                    <xsl:with-param name="sampleLims" select="."/>
                    <xsl:with-param name="AcceptanceLocus" select="$AcceptanceLocus"/>
                    <xsl:with-param name="AcceptanceChannel" select="$AcceptanceChannel"/>
                    <xsl:with-param name="AcceptanceILS" select="$AcceptanceILS"/>
                    <xsl:with-param name="AcceptanceSample" select="$AcceptanceSample"/>
                    <!--xsl:with-param name="AcceptanceDir" select="$AcceptanceDir"/-->
                    <xsl:with-param name="ReviewLocus" select="$ReviewLocus"/>
                    <xsl:with-param name="ReviewChannel" select="$ReviewChannel"/>
                    <xsl:with-param name="ReviewILS" select="$ReviewILS"/>
                    <xsl:with-param name="ReviewSample" select="$ReviewSample"/>
                    <!--xsl:with-param name="ReviewDir" select="$ReviewDir"/-->
                    <xsl:with-param name="messages" select="$messages"/>
                  </xsl:call-template>
                </xsl:if>
              </xsl:variable>
              <xsl:if test="$nNeedsAttn = '1'">
                <xsl:value-of select="$sfName"/>
                <xsl:if test="$sFrom = 'Secondary'">
                  <xsl:text> (alternate report)</xsl:text>
                </xsl:if>
                <xsl:value-of select="$attnEOL"/>
              </xsl:if>
            </xsl:when>
            <xsl:otherwise>
              <xsl:message>
                <xsl:text>Cannot find sample for </xsl:text>
                <xsl:value-of select="$sfName"/>
              </xsl:message>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
      </xsl:variable>
      <xsl:variable name="nLen" select="string-length($strList1) - $lenattnEOL"/>
      <xsl:variable name="strList">
        <xsl:if test="$nLen &gt; 0">
          <xsl:value-of select="substring($strList1,1,$nLen)"/>
        </xsl:if>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$nLen &lt; 1"/>
        <xsl:when test="contains($strList,$attnEOL)">
          <xsl:text>The following samples need attention:</xsl:text>
          <xsl:value-of select="$attnEOL"/>
          <xsl:value-of select="$attnEOL"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>The following sample needs attention:</xsl:text>
          <xsl:value-of select="$attnEOL"/>
          <xsl:value-of select="$attnEOL"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:value-of select="$strList1"/>
    </xsl:if>
  </xsl:template>
  <xsl:template match="/">
    <xsl:variable name="url" select="os:encode-file-uri($xmlfile)"/>
    <xsl:variable name="xml" select="document($url)"/>
    <xsl:call-template name="AnySampleNeedsAttn">
      <xsl:with-param name="limsxml" select="$xml"/>
    </xsl:call-template>
    <xsl:value-of select="$attnEOL"/>
    <xsl:text>done</xsl:text>
    <xsl:value-of select="$attnEOL"/>
  </xsl:template>

</xsl:stylesheet>
