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
#  FileName: SpreadSheetTemplate.xsl
#  Author:   Douglas Hoffman
#  Description:  Export allele info from OSIRIS analysis file to a
#   tab-delimited text file.  This stylesheet performs the same
#   as the included template, SpreadSheetUtils.xsl, but its
#   purpose is for customization as needed.  It is recommended
#   that this file be copied to a different file name and the new
#   file be used for customization
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:func="http://exslt.org/functions"
  xmlns:exsl="http://exslt.org/common"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  extension-element-prefixes="func exsl os"
  version="1.0">


  <xsl:import href="SpreadSheetUtils.xsl"/>

  <!-- the names of these top level parameters can be changed to
       reflect the desired order in the user interface as long
       as all references are also changed in two templates below:
       name="OsirisExport" and match="/"

       a parameter can be removed but it is highly recommended
       that a constant be used in its place in the xsl:template
       below identified has match="/".  Also, in the following
       template, name="OsirisExport" its corresponding
       "<param>" node should also be removed -->

  <xsl:param name="p05Allele" select="1"/>
  <xsl:param name="p10BPS" select="3"/>
  <xsl:param name="p12ILS_BPS" select="4"/>
  <xsl:param name="p15RFU" select="2"/>
  <xsl:param name="p20Time" select="5"/>
  <xsl:param name="p25Area" select="0"/>
  <xsl:param name="p27Fit" select="0"/>
  <xsl:param name="p28Residual" select="0"/>

  <xsl:param name="p30Style" select="2"/><!-- by locus -->
  <xsl:param name="p35Collate" select="1"/>

  <xsl:param name="p40ShowChannelNr" select="1"/>
  <xsl:param name="p02ShowFileName" select="1"/>

  <xsl:param name="p55IncludePosControl" select="0"/>
  <xsl:param name="p60IncludeNegControl" select="0"/>
  <xsl:param name="p65IncludeLadder" select="0"/>
  <xsl:param name="p70IncludeDisabled" select="0"/>

  <xsl:template name="OsirisExport">
    <!-- this xsl:template is for providing default settings
         when configuring an export in OSIRIS -->
    <export>
      <name>Spreadsheet</name>
      <file-extension>tab</file-extension>
      <file-extension>txt</file-extension>
      <extension-override>false</extension-override>
      <default-location>*A</default-location>
      <xsl-params>
        <param>
          <name>p02ShowFileName</name>
          <description>Show Sample or File Name</description>
          <type>choice</type>
          <choicelist>
            <value>Show File Name</value>
            <value>Show Sample Name</value>
            <value>Show File then Sample Name</value>
            <value>Show Sample then File Name</value>
          </choicelist>
        </param>
        <param>
          <name>p05Allele</name>
          <description>Show Allele (L-R order by number)</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p10BPS</name>
          <description>Show BPS</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p12ILS_BPS</name>
          <description>Show ILS BPS</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p15RFU</name>
          <description>Show RFU</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p20Time</name>
          <description>Show Time</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p25Area</name>
          <description>Show Area</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p27Fit</name>
          <description>Show Fit</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p28Residual</name>
          <description>Show BPS Residual</description>
          <type>choice</type>
          <choicelist>
            <value></value>
            <value>1</value>
            <value>2</value>
            <value>3</value>
            <value>4</value>
            <value>5</value>
            <value>6</value>
            <value>7</value>
            <value>8</value>
            <value>9</value>
          </choicelist>
          <has-min>true</has-min>
          <min>0</min>
          <has-max>true</has-max>
          <max>100</max>
        </param>
        <param>
          <name>p30Style</name>
          <description>Row Types</description>
          <type>choice</type>
          <choicelist>
            <value>One Sample Per Row</value>
            <value>One Locus Per Row</value>
            <value>One Peak Per Row</value>
          </choicelist>
        </param>
        <param>
          <name>p35Collate</name>
          <description>Arrange selected parameters</description>
          <type>choice</type>
          <choicelist>
            <value>Comma Delimited in table cell</value>
            <value>By peak: Allele-1, RFU-1, Allele-2, RFU-2, ...</value>
            <value>By parameter: Allele-1, Allele-2... RFU-1, RFU-2...</value>
          </choicelist>
        </param>
        <param>
          <name>p40ShowChannelNr</name>
          <description>Show Channel Number In Column Heading</description>
          <type>checkbox</type>
          <checked-value>1</checked-value>
          <unchecked-value>0</unchecked-value>
        </param>
        <param>
          <name>p55IncludePosControl</name>
          <description>Include Pos Controls</description>
          <type>checkbox</type>
          <checked-value>1</checked-value>
          <unchecked-value>0</unchecked-value>
        </param>
        <param>
          <name>p60IncludeNegControl</name>
          <description>Include Neg. Controls</description>
          <type>checkbox</type>
          <checked-value>1</checked-value>
          <unchecked-value>0</unchecked-value>
        </param>
        <param>
          <name>p65IncludeLadder</name>
          <description>Include Ladder Samples</description>
          <type>checkbox</type>
          <checked-value>1</checked-value>
          <unchecked-value>0</unchecked-value>
        </param>
        <param>
          <name>p70IncludeDisabled</name>
          <description>Include Disabled Samples</description>
          <type>checkbox</type>
          <checked-value>1</checked-value>
          <unchecked-value>0</unchecked-value>
        </param>
        <param>
          <name>pTest</name>
          <description>Test</description>
          <type>ignore</type>
        </param>
      </xsl-params>
    </export>
  </xsl:template>
  
  <xsl:template match="/">
    <xsl:call-template name="tExportAll">
      <!-- the values of the following parameters can be overridden with
           a change in the parameter name or a fixed const value -->

      <xsl:with-param name="pStyle" select="$p30Style"/>
      <xsl:with-param name="ShowFileName" select="$p02ShowFileName"/>
      <xsl:with-param name="pCollate" select="$p35Collate"/>
      <xsl:with-param name="nShowChannelNr" select="$p40ShowChannelNr"/>
      <xsl:with-param name="includeLadder" select="$p65IncludeLadder"/>
      <xsl:with-param name="includeDisabled" select="$p70IncludeDisabled"/>
      <xsl:with-param name="includePosControl" select="$p55IncludePosControl"/>
      <xsl:with-param name="includeNegControl" select="$p60IncludeNegControl"/>

      <xsl:with-param name="allele" select="$p05Allele"/>
      <xsl:with-param name="bps" select="$p10BPS"/>
      <xsl:with-param name="ilsbps" select="$p12ILS_BPS"/>
      <xsl:with-param name="rfu" select="$p15RFU"/>
      <xsl:with-param name="area" select="$p25Area"/>
      <xsl:with-param name="fit" select="$p27Fit"/>
      <xsl:with-param name="residual" select="$p28Residual"/>
      <xsl:with-param name="time" select="$p20Time"/>

    </xsl:call-template>
  </xsl:template>

  <!-- customization templates - these are copied from SpreadSheetUtils.xsl
       and if not customized should be removed
    -->

  <xsl:template name="tBeforeSampleName">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <!-- 
         Each line for this sample will begin with the output of this template.
         Unless there is no output, a delimiter (currently tab) will be appended
      -->
  </xsl:template>
  <xsl:template name="tAfterSampleName">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <!-- 
         Each line for this sample will include the output of this template
         after writing sample name and/or sample file name
     -->
  </xsl:template>
  <xsl:template name="tEndOfSampleLine">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <xsl:param name="nLine"/>
    <!-- 
         Each line for this sample will include the output of this template
         $nLine is the nth line for this sample, starting with 1
      -->
  </xsl:template>

  <xsl:template name="tBeforeHeaderSampleName">
    <xsl:param name="nStyle"/>
    <!-- 
         The header line for this sample will include the output of this template
         before all other headers
    -->
  </xsl:template>

  <xsl:template name="tAfterHeaderSampleName">
    <xsl:param name="nStyle"/>
    <!-- 
         The header line will include the output of this template
         after the sample name and or sample file name headers 
    -->
  </xsl:template>

  <xsl:template name="tEndOfHeaderSampleLine">
    <xsl:param name="nStyle"/>
    <xsl:param name="nLine"/>
    <!-- 
         The header line  will include the output of this template
         after all other headers
    -->
  </xsl:template>

  <!--
       Columns headers
    -->
  <xsl:template name="tHeaderFileName">
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>
    <xsl:text>File Name</xsl:text>
  </xsl:template>

  <xsl:template name="tHeaderSampleName">
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>
    <xsl:text>Sample Name</xsl:text>
  </xsl:template>

  <xsl:template name="tHeaderLocusName">
    <xsl:text>Locus</xsl:text>
  </xsl:template>

  <xsl:template name="tHeaderAllele">
    <xsl:text>Allele</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderILSBPS">
    <xsl:text>ILS BPS</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderBPS">
    <xsl:text>BPS</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderRFU">
    <xsl:text>RFU</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderPeakArea">
    <xsl:text>Peak Area</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderPeakWidth">
    <xsl:text>Peak Width</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderTime">
    <xsl:text>Time</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderFit">
    <xsl:text>Fit</xsl:text>
  </xsl:template>
  <xsl:template name="tHeaderResidual">
    <xsl:text>Residual</xsl:text>
  </xsl:template>



  <!-- END placeholder templates to be overridden by the including xsl script, if needed -->

</xsl:stylesheet>
