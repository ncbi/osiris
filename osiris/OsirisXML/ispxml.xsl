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
#  FileName: ispxml.xsl
#  Author:   Douglas Hoffman
#  Description:  Export DATA for the Illinois State Police STaCS LIMS
#    in preliminary XML format.  This export is NOT the tab delimited text,
#    but is imported into isptext.xsl, the xslt that transforms the xml
#    from this template into tab delimited text
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

  <xsl:import href="util.xsl"/>
  <xsl:import href="SampleName.xsl"/>
  <xsl:output method="xml" encoding="UTF-8"
    indent="yes" standalone="yes" version="1.0"/>

  <xsl:variable name="varSample" select="'$(Sample)'"/>
  <xsl:variable name="varAlleles" select="'$(Alleles)'"/>
  <xsl:variable name="varLocus" select="'$(Locus)'"/>
  <xsl:variable name="varChannel" select="'$(Channel)'"/>
  <xsl:variable name="verificationSampleXML">
    <udset>
      <UD1 priority="1" msgName="smVerificationSample"></UD1>
      <UD2 priority="1" msgName="smVerificationSample"></UD2>
      <UD3 priority="1" msgName="smVerificationSample">Verification</UD3>
    </udset>
  </xsl:variable>
  <xsl:variable name="VerificationSampleNodes"
    select="exsl:node-set($verificationSampleXML)"/>

  <!--  ranges for each locus; -->
  <xsl:variable name="tmpLocusRange">
    <locusRange>
      <locus name="CSF1PO" min="6" max="15"/>
      <locus name="D13S317" min="8" max="15"/>
      <locus name="D16S539" min="5" max="15"/>
      <locus name="D18S51" min="9" max="26"/>
      <locus name="D21S11" min="24.2" max="38"/>
      <locus name="D3S1358" min="12" max="19"/>
      <locus name="D5S818" min="7" max="16"/>
      <locus name="D7S820" min="6" max="14"/>
      <locus name="D8S1179" min="8" max="19"/>
      <locus name="FGA" min="18" max="30"/>
      <locus name="Penta D" min="2.2" max="17"/>
      <locus name="Penta E" min="5" max="24"/>
      <locus name="TH01" min="5" max="10"/>
      <locus name="TPOX" min="6" max="13"/>
      <locus name="vWA" min="11" max="21"/>
      <locus name="_default" min="0" max="99999"/>
    </locusRange>
  </xsl:variable>
  <xsl:variable name="LocusRange" select="exsl:node-set($tmpLocusRange)"/>
  <xsl:variable name="LocusDefault" select="$LocusRange/locusRange/locus[@name = '_default']"/>

  <func:function name="os:GetExportMessages">
    <xsl:param name="top"/>
    <xsl:variable name="rtn"
      select="$top/OsirisAnalysisReport/Messages/Message[(not(Hidden) or (string(Hidden) != 'true')) and string(MsgName)]"/>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:GetExports">
    <xsl:param name="top"/>
    <xsl:variable name="rtn"
      select="$top/OsirisAnalysisReport/ExportSpecifications/MsgExport"/>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:FindMessage">
    <xsl:param name="messages"/>
    <xsl:param name="n"/>
    <xsl:variable name="rtn" 
      select="$messages[MessageNumber = $n]"/>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:FindExportByName">
    <xsl:param name="exports"/>
    <xsl:param name="name"/>
    <xsl:variable name="rtn"
      select="$exports[MsgName = $name]"/>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:FindExport">
    <xsl:param name="messages"/>
    <xsl:param name="exports"/>
    <xsl:param name="msgNumber"/>

    <xsl:variable name="msg" select="os:FindMessage($messages,$msgNumber)"/>
    <xsl:choose>
      <xsl:when test="$msg">
        <xsl:variable name="rtn" 
          select="os:FindExportByName($exports,$msg/MsgName)"/>
        <func:result select="$rtn"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="false()"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>
  
  <func:function name="os:GetLocusRange">
    <xsl:param name="name"/>
    <xsl:variable name="rtn" select="$LocusRange/locusRange/locus[@name = $name]"/>
    <xsl:choose>
      <xsl:when test="$rtn">
        <func:result select="$rtn"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="$LocusDefault"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>


  <func:function name="os:ChannelFromLocus">
    <xsl:param name="topNode"/>
    <xsl:param name="locusName"/>
    <xsl:variable name="node" 
      select="$topNode/OsirisAnalysisReport/Heading/Channel[LocusName = $locusName]"/>
    <xsl:variable name="sRtn">
      <xsl:choose>
        <xsl:when test="$node and not(os:isNaN($node/ChannelNr))">
          <xsl:value-of select="$node/ChannelNr"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="number($sRtn)"/>
  </func:function>

  <func:function name="os:IsPristine">
    <xsl:param name="xml"/>

    <!-- sample is output from template RunSample in this file -->

    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="not($xml)">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="$xml//udset/*[not(@msgName = 'smVerificationSample')]">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <func:function name="os:IsPristineSample">
    <xsl:param name="sample"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="not($sample)">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="not($sample//locus)">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="os:IsPristine($sample)">
          <xsl:value-of select="1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <func:function name="os:IsVerificationSample">
    <xsl:param name="sample"/>
    <xsl:param name="sample2"/>

    <!-- sample is output from template RunSample in this file -->
    <xsl:variable name="rtn"
      select="count($sample//udset/*[ @msgName = 'smVerificationSample' ])"/>
    <xsl:variable name="rtn2Text">
      <xsl:choose>
        <xsl:when test="$sample2">
          <xsl:value-of
            select="count($sample2//udset/*[ @msgName = 'smVerificationSample' ])"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="nRtn" select="$rtn + number($rtn2Text)"/>
    <func:result select="boolean($nRtn)"/>
  </func:function>

  <xsl:template name="formatExportText">
    <xsl:param name="text"/>
    <xsl:param name="channelNr"/>
    <xsl:param name="sampleName"/>
    <xsl:param name="locusName"/>
    <xsl:param name="alleles"/>
    
    <xsl:variable name="s1">
      <xsl:call-template name="str-replace">
        <xsl:with-param name="str" select="$text"/>
        <xsl:with-param name="kill" select="$varChannel"/>
        <xsl:with-param name="replace" select="$channelNr"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="s2">
      <xsl:call-template name="str-replace">
        <xsl:with-param name="str" select="$s1"/>
        <xsl:with-param name="kill" select="$varSample"/>
        <xsl:with-param name="replace" select="$sampleName"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="s3">
      <xsl:call-template name="str-replace">
        <xsl:with-param name="str" select="$s2"/>
        <xsl:with-param name="kill" select="$varLocus"/>
        <xsl:with-param name="replace" select="$locusName"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="s4">
      <xsl:call-template name="str-replace">
        <xsl:with-param name="str" select="$s3"/>
        <xsl:with-param name="kill" select="$varAlleles"/>
        <xsl:with-param name="replace" select="$alleles"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$s4"/>
  </xsl:template>

  <xsl:template name="getUD123">
    <xsl:param name="nodeset"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <xsl:param name="ILSchannelNr" select="0"/>
    <xsl:param name="checkChannel" select="0"/>
    <xsl:param name="defaultChannel" select="0"/>
    <xsl:param name="sampleName"/>
    <xsl:param name="locusName"/>
    <xsl:param name="alleles"/>

    <xsl:for-each select="$nodeset">
      
      <xsl:variable name="export"
        select="os:FindExport($messages,$exports,.)"/>
      <xsl:if test="$export">
        <xsl:variable name="channelNr">
          <xsl:choose>
            <xsl:when test="$checkChannel = 2">
              <xsl:value-of select="../../ChannelNr"/>
            </xsl:when>
            <xsl:when test="$checkChannel = 1">
              <xsl:value-of select="../ChannelNr"/>
            </xsl:when>
            <xsl:when test="name(..) = 'ILSAlerts'">
              <xsl:value-of
                select="$ILSchannelNr"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$defaultChannel"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="priority" select="$export/Protocol/Priority"/>
        <xsl:variable name="noexport" select="$export/Protocol/DoNotExport"/>
        <xsl:variable name="msgName" select="$export/MsgName"/>
        <xsl:for-each select="$export/Protocol/TextLoc">
          <xsl:if test="Text and (Location != '')">
            <xsl:element name="{Location}">
              <xsl:attribute name="priority">
                <xsl:value-of select="$priority"/>
              </xsl:attribute>
              <xsl:attribute name="msgName">
                <xsl:value-of select="$msgName"/>
              </xsl:attribute>
              <xsl:if test="$noexport = 'true'">
                <xsl:attribute name="noexport">
                  <xsl:value-of select="1"/>
                </xsl:attribute>
              </xsl:if>
              <xsl:call-template name="formatExportText">
                <xsl:with-param name="text" select="Text"/>
                <xsl:with-param name="channelNr" select="$channelNr"/>
                <xsl:with-param name="sampleName" select="$sampleName"/>
                <xsl:with-param name="locusName" select="$locusName"/>
                <xsl:with-param name="alleles" select="$alleles"/>
              </xsl:call-template>
            </xsl:element>
          </xsl:if>
        </xsl:for-each>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="sampleUD">
    <xsl:param name="sample"/>
    <xsl:param name="sampleName"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <xsl:param name="ILSchannelNr"/>

    <udset>
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset" select="$sample/*/MessageNumber"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
        <xsl:with-param name="ILSchannelNr" select="$ILSchannelNr"/>
        <xsl:with-param name="sampleName" select="$sampleName"/>
      </xsl:call-template>

      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/ChannelAlerts/Channel/MessageNumber"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
        <xsl:with-param name="checkChannel" select="1"/>
        <xsl:with-param name="sampleName" select="$sampleName"/>
      </xsl:call-template>

      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/ChannelAlerts/Channel/Artifact[not(Disabled) or (Disabled != 'true')]/MessageNumber"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
        <xsl:with-param name="checkChannel" select="2"/>
        <xsl:with-param name="sampleName" select="$sampleName"/>
      </xsl:call-template>

      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/InterlocusAlerts/Alert/MessageNumber"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
        <xsl:with-param name="sampleName" select="$sampleName"/>
      </xsl:call-template>

    </udset>

  </xsl:template>


  <xsl:template name="IsEnabled">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when test="$sample/EnableHistory">
        <xsl:for-each select="$sample/EnableHistory/Enable">
          <xsl:sort select="Time" data-type="number" order="descending"/>
          <xsl:choose>
            <xsl:when test="position() != 1"/>
            <xsl:when test="string(Enabled) = 'false'">
              <xsl:value-of select="0"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="AlleleName">
    <xsl:param name="name"/>
    <xsl:param name="locus"/>
    <xsl:param name="offLadder"/>
    <xsl:choose>
      <xsl:when test="not($locus = 'AMEL')">
        <xsl:variable name="minmax" select="os:GetLocusRange($locus)"/>
        <xsl:variable name="min" select="$minmax/@min"/>
        <xsl:variable name="max" select="$minmax/@max"/>
        <xsl:if test="max &gt; 9000">
          <xsl:message>
            <xsl:text>Could not find range for locus: </xsl:text>
            <xsl:value-of select="$locus"/>
          </xsl:message>
        </xsl:if>

        <xsl:variable name="nAllele" select="number($name)"/>

        <xsl:choose>
          <xsl:when test="$nAllele &lt; $min">
            <xsl:text>&lt;</xsl:text>
            <xsl:value-of select="$min"/>
          </xsl:when>
          <xsl:when test="$nAllele &gt; $max">
            <xsl:text>&gt;</xsl:text>
            <xsl:value-of select="$max"/>
          </xsl:when>
          <xsl:when test="$offLadder = 'true'">
            <xsl:text>OL</xsl:text>
            <xsl:value-of select="$name"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$name"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$name = '1'">
        <xsl:text>X</xsl:text>
      </xsl:when>
      <xsl:when test="$name = '2'">
        <xsl:text>Y</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$name"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="directoryUD">
    <xsl:param name="xml"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <udset>
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$xml/OsirisAnalysisReport/DirectoryAlerts/MessageNumber"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
      </xsl:call-template>
    </udset>
  </xsl:template>

  <xsl:template name="RunSample">
    <xsl:param name="xml"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <xsl:param name="sample"/>
    <xsl:param name="ILSchannelNr"/>
    <xsl:param name="loci"/>
    <xsl:param name="UD123dir"/>
    <xsl:param name="countUD123dir"/>
    <xsl:param name="isVerification" select="false()"/>


    <xsl:variable name="enabled">
      <xsl:call-template name="IsEnabled">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="$enabled != 0">
      <xsl:variable name="sampleName" select="os:GetSampleName($sample)"/>
      <xsl:variable name="uname" select="os:GetUniqueSampleName($sample)"/>
      <xsl:variable name="tmpUD123sample">
        <xsl:call-template name="sampleUD">
          <xsl:with-param name="sample" select="$sample"/>
          <xsl:with-param name="sampleName" select="$sampleName"/>
          <xsl:with-param name="exports" select="$exports"/>
          <xsl:with-param name="messages" select="$messages"/>
          <xsl:with-param name="ILSchannelNr" select="$ILSchannelNr"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="UD123sample" select="exsl:node-set($tmpUD123sample)"/>
      
      <xsl:if test="not($UD123sample) or not($UD123sample//*/@noexport)">
        <xsl:variable name="countUD123sample" select="count($UD123sample/udset/*)"/>
        <sample name="{$sampleName}" file="{$sample/Name}" uname="{$uname}">
          <xsl:if test="$countUD123dir or $countUD123sample or $isVerification">
            <udset>
              <xsl:if test="$countUD123dir">
                <xsl:copy-of select="$UD123dir/udset/*"/>
              </xsl:if>
              <xsl:if test="$countUD123sample">
                <xsl:copy-of select="$UD123sample/udset/*"/>
              </xsl:if>
              <xsl:if test="$isVerification and not($UD123sample/udset/*[@msgName = 'smVerificationSample'])">
                <xsl:copy-of select="$VerificationSampleNodes/udset/*"/>
              </xsl:if>
            </udset>
          </xsl:if>
          <xsl:for-each select="$loci">
            <xsl:variable name="locusName" select="."/>
            <xsl:variable name="locus" select="$sample/Locus[LocusName = $locusName]"/>
            <xsl:variable name="alleles"
              select="$locus/Allele[not(Disabled) or (Disabled != 'true')]"/>
            <xsl:variable name="strAlleles">
              <xsl:for-each select="$alleles">
                <xsl:if test="position() != 1">
                  <xsl:text>, </xsl:text>
                </xsl:if>
                <xsl:variable name="name">
                  <xsl:call-template name="AlleleName">
                    <xsl:with-param name="name" select="Name"/>
                    <xsl:with-param name="locus" select="$locusName"/>
                    <xsl:with-param name="offLadder" select="OffLadder"/>
                  </xsl:call-template>
                </xsl:variable>
                <xsl:value-of select="Name"/>
              </xsl:for-each>
            </xsl:variable>
            
            <xsl:variable name="channelNr"
              select="os:ChannelFromLocus($xml,$locusName)"/>
            <locus channel="{$channelNr}" name="{$locusName}">
              <xsl:variable name="locusMessages"
                select="$locus/LocusAlerts/MessageNumber"/>
              <xsl:if test="$locusMessages">
                <udset>
                  <xsl:call-template name="getUD123">
                    <xsl:with-param name="nodeset"
                      select="$locusMessages"/>
                    <xsl:with-param name="exports"
                      select="$exports"/>
                    <xsl:with-param name="messages"
                      select="$messages"/>
                    <xsl:with-param name="locusName"
                      select="$locusName"/>
                    <xsl:with-param name="alleles"
                      select="$strAlleles"/>
                    <xsl:with-param name="defaultChannel"
                      select="$channelNr"/>
                  </xsl:call-template>
                </udset>
              </xsl:if>
              <xsl:for-each select="$alleles">
                <xsl:variable name="AlleleName">
                  <!-- do not check off ladder -->
                  <xsl:call-template name="AlleleName">
                    <xsl:with-param name="name" select="Name"/>
                    <xsl:with-param name="locus" select="$locusName"/>
                    <xsl:with-param name="offLadder" select="OffLadder"/>
                  </xsl:call-template>
                </xsl:variable>
                <allele name="{$AlleleName}">
                  <xsl:if test="MessageNumber">
                    <udset>
                      <xsl:call-template name="getUD123">
                        <xsl:with-param name="nodeset"
                          select="./MessageNumber"/>
                        <xsl:with-param name="exports" 
                          select="$exports"/>
                        <xsl:with-param name="locusName"
                          select="$locusName"/>
                        <xsl:with-param name="alleles"
                          select="AlleleName"/>
                        <xsl:with-param name="defaultChannel"
                          select="$channelNr"/>
                      </xsl:call-template>
                    </udset>
                  </xsl:if>
                </allele>
              </xsl:for-each>
            </locus>
          </xsl:for-each>
        </sample>
      </xsl:if><!-- xsl:if test="not($UD123sample//*/@noexport)" -->
    </xsl:if><!-- xsl:if test="$enabled != 0" -->
  </xsl:template>



  <xsl:template name="isprun">
    <xsl:param name="xml"/>
    <xsl:param name="exports"/>
    <xsl:param name="messages"/>
    <xsl:param name="SkipList" select="false()"/>
    <xsl:param name="VerificationList" select="false()"/>
    <xsl:param name="userMessages" select="''"/>

    <xsl:variable name="samples" 
      select="$xml/OsirisAnalysisReport/Table/Sample[ not($SkipList and os:InList($SkipList,os:GetUniqueSampleName(.))) ]"/>
    <xsl:variable name="ILSchannelNr"
        select="$xml/OsirisAnalysisReport/Heading/ILSchannelNr"/>
    <xsl:variable name="loci"
      select="$xml/OsirisAnalysisReport/Heading/Channel/LocusName"/>

    <xsl:variable name="tmpUD123dir">
      <xsl:call-template name="directoryUD">
        <xsl:with-param name="xml" select="$xml"/>
        <xsl:with-param name="exports" select="$exports"/>
        <xsl:with-param name="messages" select="$messages"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="UD123dir" select="exsl:node-set($tmpUD123dir)"/>
    <xsl:variable name="countUD123dir" select="count($UD123dir/udset/*)"/>
    <xsl:variable name="noexport" select="$exports//Protocol[DoNotExport = 'true' and string-length(Notification) != 0]"/>
    <xsl:if test="$noexport">
      <xsl:message>
        <xsl:for-each select="$noexport">
          <xsl:if test="position() != 0">&#10;</xsl:if>
          <xsl:value-of select="Level"/>
          <xsl:text> level message: </xsl:text>
          <xsl:value-of select="Notification"/>
        </xsl:for-each>
      </xsl:message>
    </xsl:if>
    <directory>

      <xsl:if test="$countUD123dir">
        <xsl:copy-of select="$UD123dir"/>
      </xsl:if>

      <xsl:if test="not($UD123dir//*/@noexport) and $samples">

        <header>
          <column>Sample Name</column>
          <column>Marker</column>
          <column>Allele 1</column>
          <column>Allele 2</column>
          <column>Allele 3</column>
          <column>Allele 4</column>
          <column>UD1</column>
          <column>UD2</column>
          <column>UD3</column>
          <column></column>
          <column>Sample File</column>
          <xsl:if test="string-length($userMessages)">
            <column>
              <xsl:value-of select="$userMessages"/>
            </column>
          </xsl:if>
        </header>

        <xsl:for-each select="$samples">
          <xsl:variable name="isVerification"
            select="os:InList($VerificationList,os:GetUniqueSampleName(.))"/>
          <xsl:call-template name="RunSample">
            <xsl:with-param name="xml" select="$xml"/>
            <xsl:with-param name="exports" select="$exports"/>
            <xsl:with-param name="messages" select="$messages"/>
            <xsl:with-param name="sample" select="."/>
            <xsl:with-param name="ILSchannelNr" select="$ILSchannelNr"/>
            <xsl:with-param name="loci" select="$loci"/>
            <xsl:with-param name="UD123dir" select="$UD123dir"/>
            <xsl:with-param name="isVerification" select="$isVerification"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:if>
    </directory>
  </xsl:template>

  <xsl:template name="isprunBase">
    <xsl:param name="xml"/>
    <xsl:param name="userMessages" select="''"/>
    <xsl:call-template name="isprun">
      <xsl:with-param name="xml" select="$xml"/>
      <xsl:with-param name="exports"
        select="os:GetExports($xml)"/>
      <xsl:with-param name="messages"
        select="os:GetExportMessages($xml)"/>
      <xsl:with-param name="userMessages" select="$userMessages"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/">
    <xsl:call-template name="isprunBase">
      <xsl:with-param name="xml" select="/"/>
    </xsl:call-template>
  </xsl:template>

</xsl:stylesheet>
