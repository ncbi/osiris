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
#  FileName: SpreadSheetUtils.xsl
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

  <xsl:import href="SampleLadderData.xsl"/>
  <xsl:import href="util.xsl"/>
  <xsl:output method="text"/>

  <xsl:variable name="StyleLineByPeak" select="1"/>
  <xsl:variable name="StyleLineByLocus" select="2"/>
  <xsl:variable name="StyleLineBySample" select="3"/>

  <xsl:param name="p05Allele" select="1"/>
  <xsl:param name="p10BPS" select="3"/>
  <xsl:param name="p12ILS_BPS" select="4"/>
  <xsl:param name="p15RFU" select="2"/>
  <xsl:param name="p20Time" select="5"/>
  <xsl:param name="p25Area" select="0"/>
  <xsl:param name="p27Fit" select="0"/>
  <xsl:param name="p28Residual" select="0"/>

  <xsl:param name="p30Style" select="'One Locus Per Row'"/>
  <xsl:param name="p35Collate" select="1"/>

  <xsl:param name="p40ShowChannelNr" select="1"/>
  <xsl:param name="p02ShowFileName" select="1"/>

  <xsl:param name="p55IncludePosControl" select="0"/>
  <xsl:param name="p60IncludeNegControl" select="0"/>
  <xsl:param name="p65IncludeLadder" select="0"/>
  <xsl:param name="p70IncludeDisabled" select="0"/>

  <xsl:variable name="pTest" select="0"/>
  <xsl:template name="OsirisExport">
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
  

  <xsl:variable name="DELIM" select="'&#9;'"/>
  <xsl:variable name="COMMA" select="', '"/>
  <xsl:variable name="lenDELIM" select="string-length($DELIM)"/>

  <xsl:template name="tEOL">
    <xsl:text>&#13;&#10;</xsl:text>
  </xsl:template>

  <!-- BEGIN placeholder templates to be overridden by the including xsl script, if needed -->
  <xsl:template name="tBeforeSampleName">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <!-- placeholder which can be overridden in XSL script that includes this file.
         Each line for this sample will begin with the output of this template.
         Unless there is no output, a delimiter (currently tab) will be appended
      -->
  </xsl:template>
  <xsl:template name="tAfterSampleName">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <!-- placeholder which can be overridden in XSL script that includes this file.
         Each line for this sample will include the output of this template
         after writing sample name and/or sample file name
     -->
  </xsl:template>
  <xsl:template name="tEndOfSampleLine">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <xsl:param name="nLine"/>
    <!-- placeholder which can be overridden in XSL script that includes this file
         Each line for this sample will include the output of this template

         $nLine is the nth line for this sample, starting with 1
      -->
  </xsl:template>

  <xsl:template name="tBeforeHeaderSampleName">
    <xsl:param name="nStyle"/>
    <!-- placeholder which can be overridden in XSL script that includes this file
         The header line for this sample will include the output of this template
    -->
  </xsl:template>

  <xsl:template name="tAfterHeaderSampleName">
    <xsl:param name="nStyle"/>
    <!-- placeholder which can be overridden in XSL script that includes this file
         The header line for this sample will include the output of this template
    -->
  </xsl:template>

  <xsl:template name="tEndOfHeaderSampleLine">
    <xsl:param name="nStyle"/>
    <xsl:param name="nLine"/>
    <!-- placeholder which can be overridden in XSL script that includes this file
         The header line for this sample will include the output of this template
    -->
  </xsl:template>

  <!-- END placeholder templates to be overridden by the including xsl script, if needed -->

  <!-- BEGIN Other templates that may need to be overridden -->

  <!-- BEGIN column headers -->
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

  <!-- END column headers -->

  <xsl:template name="tAfterWriteHeaders">
    <xsl:param name="nStyle"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="nMaxPeakCount"/>
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:param name="collate"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includePosControl"/>
    <xsl:param name="includeNegControl"/>
  </xsl:template>


  <!-- END Other templates that may need to be overridden -->

  <xsl:template name="tDelimAndColumn">
    <xsl:param name="s"/>
    <xsl:variable name="nLen" select="string-length($s)"/>
    <xsl:choose>
      <xsl:when test="$nLen = 0"/>
      <xsl:when test="$nLen &lt; $lenDELIM"/>
      <xsl:when test="substring($s,1, $lenDELIM) != $DELIM">
        <xsl:value-of select="$DELIM"/>
      </xsl:when>
    </xsl:choose>
    <xsl:value-of select="$s"/>
  </xsl:template>

  <xsl:template name="tColumnAndDelim">
    <xsl:param name="s"/>
    <xsl:variable name="nLen" select="string-length($s)"/>
    <xsl:value-of select="$s"/>
    <xsl:choose>
      <xsl:when test="$nLen = 0"/>
      <xsl:when test="$nLen &lt; $lenDELIM"/>
      <xsl:when test="substring($s,$nLen - $lenDELIM + 1, $lenDELIM) != $DELIM">
        <xsl:value-of select="$DELIM"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="tCount">
    <xsl:param name="s"/>
    <xsl:param name="search"/>
    <xsl:param name="runningCount" select="0"/>
    <xsl:choose>
      <xsl:when test="contains($s,$search)">
        <xsl:call-template name="tCount">
          <xsl:with-param name="s" select="substring-after($s,$search)"/>
          <xsl:with-param name="search" select="$search"/>
          <xsl:with-param name="runningCount" select="$runningCount + 1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$runningCount"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="tXmlCount">
    <xsl:param name="start"/>
    <xsl:param name="stop"/>
    <xsl:param name="level" select="1"/>
    <xsl:choose>
      <xsl:when test="$level &gt; 20">
        <xsl:message>
          <xsl:text>tXmlCount level &gt; 20</xsl:text>
        </xsl:message>
      </xsl:when>
      <xsl:when test ="$start &lt; $stop">
        <!-- ascending divide and conquer -->
        <xsl:variable name="mid" select="floor(($start + $stop) * 0.5)"/>
        <xsl:call-template name="tXmlCount">
          <xsl:with-param name="start" select="$start"/>
          <xsl:with-param name="stop" select="$mid"/>
          <xsl:with-param name="level" select="$level + 1"/>
        </xsl:call-template>
        <xsl:call-template name="tXmlCount">
          <xsl:with-param name="start" select="$mid + 1"/>
          <xsl:with-param name="stop" select="$stop"/>
          <xsl:with-param name="level" select="$level + 1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$start = $stop">
        <x><xsl:value-of select="$start"/></x>
      </xsl:when>
      <xsl:otherwise>
        <!-- descending divide and conquer -->
        <xsl:variable name="mid" select="floor(($start + $stop) * 0.5)"/>
        <xsl:call-template name="tXmlCount">
          <xsl:with-param name="start" select="$start"/>
          <xsl:with-param name="stop" select="$mid + 1"/>
          <xsl:with-param name="level" select="$level + 1"/>
        </xsl:call-template>
        <xsl:call-template name="tXmlCount">
          <xsl:with-param name="start" select="$mid"/>
          <xsl:with-param name="stop" select="$stop"/>
          <xsl:with-param name="level" select="$level + 1"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:EndsWith">
    <xsl:param name="str"/>
    <xsl:param name="substr"/>
    <xsl:variable name="nLen" select="string-length($str)"/>
    <xsl:variable name="nLenSub" select="string-length($substr)"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="$nLenSub &gt; $nLen"/>
        <xsl:when test="not($nLen) or not($nLenSub)"/>
        <xsl:when test="substring($str,$nLen - $nLenSub + 1) = $substr">1</xsl:when>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(string-length($rtn))"/>
  </func:function>
  <xsl:template name="tChop">
    <xsl:param name="str"/>
    <xsl:param name="substr"/>
    <xsl:param name="all" select="0"/>

    <xsl:choose>
      <xsl:when test="$str = $substr"/><!-- done -->
      <xsl:when test="not(os:EndsWith($str,$substr))">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="$all">
        <xsl:variable name="nLen" select="string-length($str) - string-length($substr)"/>
        <xsl:call-template name="tChop">
          <xsl:with-param name="str" select="substring($str,1,$nLen)"/>
          <xsl:with-param name="substr" select="$substr"/>
          <xsl:with-param name="all" select="$all"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="nLen" select="string-length($str) - string-length($substr)"/>
        <xsl:value-of select="substring($str,1,$nLen)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <func:function name="os:Chop">
    <xsl:param name="str"/>
    <xsl:param name="substr"/>
    <xsl:variable name="rtn">
      <xsl:call-template name="tChop">
        <xsl:with-param name="str" select="$str"/>
        <xsl:with-param name="substr" select="$substr"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:CounterXML">
    <xsl:param name="start"/>
    <xsl:param name="stop"/>
    <!-- return list xml/x from start to stop incrementing by 1 or -1 -->
    <xsl:variable name="x">
      <xml>
        <xsl:call-template name="tXmlCount">
          <xsl:with-param name="start" select="$start"/>
          <xsl:with-param name="stop" select="$stop"/>
        </xsl:call-template>
      </xml>
    </xsl:variable>
    <func:result select="exsl:node-set($x)//x"/>
  </func:function>

  <func:function name="os:FindCount">
    <xsl:param name="s"/>
    <xsl:param name="search"/>
    <xsl:variable name="x">
      <xsl:call-template name="tCount">
        <xsl:with-param name="s" select="$s"/>
        <xsl:with-param name="search" select="$search"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="number($x)"/>
  </func:function>

  <xsl:template name="tRepeat">
    <!-- write a variable n times, usually for empty table cells -->
    <xsl:param name="nCount"/>
    <xsl:param name="s"/>
    <xsl:choose>
      <xsl:when test="$nCount = 1">
        <xsl:value-of select="$s"/>
      </xsl:when>
      <xsl:when test="$nCount &gt; 1">
        <!-- split in half and call twice -->
        <xsl:variable name="nMid" select="floor($nCount * 0.5)"/>
        <xsl:call-template name="tRepeat">
          <xsl:with-param name="nCount" select="$nMid"/>
          <xsl:with-param name="s" select="$s"/>
        </xsl:call-template>
        <xsl:call-template name="tRepeat">
          <xsl:with-param name="nCount" select="$nCount - $nMid"/>
          <xsl:with-param name="s" select="$s"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:UseSample">
    <xsl:param name="xSample"/>
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeNegControl"/>
    <xsl:param name="includePosControl"/>
    <xsl:variable name="type" select="string($xSample/Type)"/>
    <xsl:variable name="rtn">
      <xsl:text>0</xsl:text>
      <xsl:choose>
        <xsl:when test="not($includeDisabled) and not(os:IsEnabled($xSample))"/>
        <xsl:when test="not($includeLadder) and ($type = 'Ladder')"/>
        <xsl:when test="not($includePosControl) and ($type = '+Control')"/>
        <xsl:when test="not($includeNegControl) and ($type = '-Control')"/>
        <xsl:otherwise>1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <func:function name="os:ExcludedSamples">
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeNegControl"/>
    <xsl:param name="includePosControl"/>
    <func:result select="/OsirisAnalysisReport/Table/Sample[not(os:UseSample(., $includeDisabled, $includeLadder, $includeNegControl, $includePosControl))]"/>
  </func:function>


  <func:function name="os:IncludedSamples">
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeNegControl"/>
    <xsl:param name="includePosControl"/>

    <xsl:variable name="xSamples" select="/OsirisAnalysisReport/Table/Sample[os:UseSample(., $includeDisabled, $includeLadder, $includeNegControl, $includePosControl)]"/>

    <xsl:choose>
      <xsl:when test="$includeLadder &gt; 1">
        <xsl:variable name="tmpSamples">
          <x>
            <xsl:for-each select="$xSamples">
              <xsl:choose>
                <xsl:when test="Type = 'Ladder'">
                  <xsl:call-template name="SampleLadderData"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:copy-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </x>
        </xsl:variable>
        <xsl:variable name="xlSamples" select="exsl:node-set($tmpSamples)/x/Sample"/>
        <func:result select="$xlSamples"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="$xSamples"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  <xsl:template name="tIncludedSamples">
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeNegControl"/>
    <xsl:param name="includePosControl"/>
    <x>
      <xsl:copy-of select="os:IncludedSamples($includeDisabled,$includeLadder,$includeNegControl,$includePosControl)"/>
    </x>
  </xsl:template>
  <xsl:template name="tExcludedSamples">
    <xsl:param name="includeDisabled"/>
    <xsl:param name="includeLadder"/>
    <xsl:param name="includeNegControl"/>
    <xsl:param name="includePosControl"/>
    <x>
      <xsl:copy-of select="os:ExcludedSamples($includeDisabled,$includeLadder,$includeNegControl,$includePosControl)"/>
    </x>
  </xsl:template>


  <xsl:template name="tMaxLocusPeakCount">
    <xsl:param name="xSamples"/>
    <xsl:variable name="xCount">
      <xml>
        <xsl:for-each select="$xSamples">
          <xsl:for-each select="Locus">
            <x>
              <xsl:value-of select="count(Allele[not(Disabled = 'true')])"/>
            </x>
          </xsl:for-each>
        </xsl:for-each>
      </xml>
    </xsl:variable>
    <xsl:value-of select="os:max(exsl:node-set($xCount)//x)"/>
  </xsl:template>

  <func:function name="os:MaxLocusPeakCount">
    <xsl:param name="xSamples"/>
    <xsl:variable name="x">
      <xsl:call-template name="tMaxLocusPeakCount">
        <xsl:with-param name="xSamples" select="$xSamples"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="os:Number0($x)"/>
  </func:function>

  <xsl:template name="tPeakCountByLocus">
    <xsl:param name="xSamples"/>
    <xml>
      <xsl:for-each select="/OsirisAnalysisReport/Heading/Channel/LocusName">
        <xsl:variable name="locusName" select="string(.)"/>
        <xsl:variable name="channel" select="../ChannelNr"/>
        <xsl:variable name="tmpLocusCount">
          <locus>
            <xsl:for-each select="$xSamples">
              <x>
                <xsl:value-of select="count(Locus[LocusName = $locusName]/Allele[not(Disabled = 'true')])"/>
              </x>
            </xsl:for-each>
          </locus>
        </xsl:variable>
        <xsl:variable name="nCount" select="os:max(exsl:node-set($tmpLocusCount)//x)"/>
        <locus name="{$locusName}" count="{$nCount}" channel="{$channel}"/>
      </xsl:for-each>
    </xml>
  </xsl:template>
  <xsl:template name="tChannelByLocus">
    <xml>
      <xsl:for-each select="/OsirisAnalysisReport/Heading/Channel/LocusName">
        <xsl:variable name="locusName" select="string(.)"/>
        <xsl:variable name="channel" select="../ChannelNr"/>
        <locus name="{$locusName}" channel="{$channel}"/>
      </xsl:for-each>
    </xml>
  </xsl:template>

  <xsl:template name="tBuildFieldOrder">
    <xsl:param name="allele" select="0"/>
    <xsl:param name="bps" select="0"/>
    <xsl:param name="ilsbps" select="0"/>
    <xsl:param name="rfu" select="0"/>
    <xsl:param name="area" select="0"/>
    <xsl:param name="width" select="0"/>
    <xsl:param name="time" select="0"/>
    <xsl:param name="fit" select="0"/>
    <xsl:param name="residual" select="0"/>

    <xsl:variable name="alleleHdr">
      <xsl:call-template name="tHeaderAllele"/>
    </xsl:variable>
    <xsl:variable name="ilsBpsHdr">
      <xsl:call-template name="tHeaderILSBPS"/>
    </xsl:variable>
    <xsl:variable name="bpsHdr">
      <xsl:call-template name="tHeaderBPS"/>
    </xsl:variable>
    <xsl:variable name="rfuHdr">
      <xsl:call-template name="tHeaderRFU"/>
    </xsl:variable>
    <xsl:variable name="areaHdr">
      <xsl:call-template name="tHeaderPeakArea"/>
    </xsl:variable>
    <xsl:variable name="widthHdr">
      <xsl:call-template name="tHeaderPeakWidth"/>
    </xsl:variable>
    <xsl:variable name="timeHdr">
      <xsl:call-template name="tHeaderTime"/>
    </xsl:variable>
    <xsl:variable name="fitHdr">
      <xsl:call-template name="tHeaderFit"/>
    </xsl:variable>
    <xsl:variable name="residualHdr">
      <xsl:call-template name="tHeaderResidual"/>
    </xsl:variable>

    <xsl:variable name="try1">
      <xml>
        <xsl:if test="$allele &gt; 0">
          <x name="allele" value="{$allele}" header="{$alleleHdr}"/>
        </xsl:if>
        <xsl:if test="$bps &gt; 0">
          <x name="bps" value="{$bps}" header="{$bpsHdr}"/>
        </xsl:if>
        <xsl:if test="$ilsbps &gt; 0">
          <x name="ilsbps" value="{$ilsbps}" header="{$ilsBpsHdr}"/>
        </xsl:if>
        <xsl:if test="$rfu &gt; 0">
          <x name="rfu" value="{$rfu}" header="{$rfuHdr}"/>
        </xsl:if>
        <xsl:if test="$area &gt; 0">
          <x name="area" value="{$area}" header="{$areaHdr}"/>
        </xsl:if>
        <xsl:if test="$width &gt; 0">
          <x name="width" value="{$width}" header="{$widthHdr}"/>
        </xsl:if>
        <xsl:if test="$time &gt; 0">
          <x name="time" value="{$time}" header="{$timeHdr}"/>
        </xsl:if>
        <xsl:if test="$fit &gt; 0">
          <x name="fit" value="{$fit}" header="{$fitHdr}"/>
        </xsl:if>
        <xsl:if test="$residual &gt; 0">
          <x name="residual" value="{$residual}" header="{$residualHdr}"/>
        </xsl:if>
      </xml>
    </xsl:variable>
    <xml>
      <xsl:for-each select="exsl:node-set($try1)//x[@value &gt; 0]">
        <xsl:sort select="@value" data-type="number"/>
        <xsl:sort select="@name"/>
        <xsl:copy-of select="."/>
      </xsl:for-each>
    </xml>
  </xsl:template>

  <xsl:template name="tWriteOneValue">
    <xsl:param name="xField"/>
    <xsl:param name="xAllele"/>
    <xsl:variable name="name" select="$xField/@name"/>

    <xsl:choose>
      <xsl:when test="$xField/@value &lt; 1"/>
      <xsl:when test="$name = 'allele'">
        <xsl:value-of select="os:DisplayAllele($xAllele/Name, $xAllele/../LocusName)"/>
      </xsl:when>
      <xsl:when test="$name = 'bps'">
        <xsl:value-of select="round($xAllele/BPS)"/>
      </xsl:when>
      <xsl:when test="$name = 'ilsbps'">
        <xsl:value-of select="$xAllele/meanbps"/>
      </xsl:when>
      <xsl:when test="$name = 'rfu'">
        <xsl:value-of select="$xAllele/RFU"/>
      </xsl:when>
      <xsl:when test="$name = 'area'">
        <xsl:value-of select="$xAllele/PeakArea"/>
      </xsl:when>
      <xsl:when test="$name = 'width'">
        <xsl:value-of select="$xAllele/Width"/>
      </xsl:when>
      <xsl:when test="$name = 'time'">
        <xsl:value-of select="$xAllele/Time"/>
      </xsl:when>
      <xsl:when test="$name = 'fit'">
        <xsl:value-of select="$xAllele/Fit"/>
      </xsl:when>
      <xsl:when test="$name = 'residual'">
        <xsl:value-of select="$xAllele/BPS - round($xAllele/BPS)"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="tLocusName">
    <xsl:param name="sName"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:value-of select="$sName"/>
    <xsl:if test="$nShowChannelNr = 1">
      <xsl:variable name="n" select="$xLocusInfo/locus[@name = $sName]/@channel"/>
      <xsl:if test="string-length($n)">
        <xsl:if test="string-length($sName)">
          <xsl:text>-</xsl:text>
        </xsl:if>
        <xsl:value-of select="$n"/>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template name="tWritePeak">
    <!-- write all desired fields for a peak or an Allele element from an oar/oer file -->
    <xsl:param name="xAllele"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:for-each select="$xFieldOrder//x">
      <xsl:value-of select="$DELIM"/>
      <xsl:call-template name="tWriteOneValue">
        <xsl:with-param name="xAllele" select="$xAllele"/>
        <xsl:with-param name="xField" select="."/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="tWriteLocus">
    <!-- write all peaks for each desired field of a particular locus
         eg Allele1, Allele2, Allele3, RFU1, RFU2, RFU3, etc. ($collate=0 or -1)
            Allele1, RFU1, Time1,... Allele2, RFU2, Time2,... ($collate=1)

         xLocus - Sample/Locus element
         xFieldOrder - list of fields in order of display (x name="field-name" value="sort-order")
         nPeakCount - min. number of peak columns to fill.  0 for no fill
         collate - see above
    -->
    <xsl:param name="xLocus"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="nPeakCount" select="0"/>
    <xsl:param name="collate" select="0"/>

    <xsl:variable name="xAlleles" select="$xLocus/Allele[not(Disabled = 'true')]"/>
    <xsl:variable name="nPeaks" select="count($xAlleles)"/>

    <xsl:choose>
      <xsl:when test="$collate = 1">
        <xsl:for-each select="$xAlleles">
          <xsl:call-template name="tWritePeak">
            <xsl:with-param name="xAllele" select="."/>
            <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          </xsl:call-template>
        </xsl:for-each>
        <xsl:call-template name="tRepeat">
          <!-- write the empty table cells -->
          <xsl:with-param name="nCount" select="($nPeakCount - $nPeaks) * count($xFieldOrder//x)"/>
          <xsl:with-param name="s" select="$DELIM"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$collate = -1">
        <xsl:for-each select="$xFieldOrder//x">
          <xsl:variable name="xField" select="."/>
          <xsl:value-of select="$DELIM"/>
          <xsl:for-each select="$xAlleles">
            <xsl:if test="position() &gt; 1">
              <xsl:value-of select="$COMMA"/>
            </xsl:if>
            <xsl:call-template name="tWriteOneValue">
              <xsl:with-param name="xAllele" select="."/>
              <xsl:with-param name="xField" select="$xField"/>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="$xFieldOrder//x">
          <xsl:variable name="xField" select="."/>
          <xsl:for-each select="$xAlleles">
            <xsl:value-of select="$DELIM"/>
            <xsl:call-template name="tWriteOneValue">
              <xsl:with-param name="xAllele" select="."/>
              <xsl:with-param name="xField" select="$xField"/>
            </xsl:call-template>
          </xsl:for-each>
          <xsl:call-template name="tRepeat">
            <!-- write the empty table cells -->
            <xsl:with-param name="nCount" select="$nPeakCount - $nPeaks"/>
            <xsl:with-param name="s" select="$DELIM"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="tWriteSampleByPeak">
    <xsl:param name="xSample"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="sLinePrefix" select="''"/>
    <xsl:param name="showLocusName" select="1"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:for-each select="$xSample/Locus/Allele[not(Disabled = 'true')]">
      <xsl:value-of select="$sLinePrefix"/>
      <xsl:if test="$showLocusName">
        <xsl:value-of select="$DELIM"/>
        <xsl:call-template name="tLocusName">
          <xsl:with-param name="sName" select="../LocusName"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:call-template name="tWritePeak">
        <xsl:with-param name="xAllele" select="."/>
        <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
      </xsl:call-template>
      <xsl:call-template name="tWriteEndOfSampleLine">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$StyleLineByPeak"/>
        <xsl:with-param name="nLine" select="position()"/>
      </xsl:call-template>
      <xsl:call-template name="tEOL"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="tWriteEndOfSampleLine">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <xsl:param name="nLine"/>

    <xsl:variable name="s">
      <xsl:call-template name="tEndOfSampleLine">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$nStyle"/>
        <xsl:with-param name="nLine" select="$nLine"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="tDelimAndColumn">
      <xsl:with-param name="s" select="$s"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="tWriteSampleLocus">
    <xsl:param name="xSample"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="nMaxPeakCount"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="sLinePrefix"/>
    <xsl:param name="collate"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:param name="showLocusName" select="1"/>

    <xsl:for-each select="$xSample/Locus">
      <xsl:value-of select="$sLinePrefix"/>
      <xsl:if test="$showLocusName">
        <xsl:value-of select="$DELIM"/>
        <xsl:call-template name="tLocusName">
          <xsl:with-param name="sName" select="LocusName"/>
          <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:call-template name="tWriteLocus">
        <xsl:with-param name="xLocus" select="."/>
        <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
        <xsl:with-param name="nPeakCount" select="$nMaxPeakCount"/>
        <xsl:with-param name="collate" select="$collate"/>
      </xsl:call-template>
      <xsl:call-template name="tWriteEndOfSampleLine">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$StyleLineByLocus"/>
        <xsl:with-param name="nLine" select="position()"/>
      </xsl:call-template>
      <xsl:call-template name="tEOL"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="tWriteSampleFull">
    <xsl:param name="xSample"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="sLinePrefix"/>
    <xsl:param name="collate"/>


    <xsl:value-of select="$sLinePrefix"/>
    <xsl:for-each select="$xSample/Locus">
      <xsl:variable name="locusName" select="LocusName"/>
      <xsl:variable name="nPeakCount" select="$xLocusInfo//locus[@name=$locusName]/@count"/>
      <xsl:call-template name="tWriteLocus">
        <xsl:with-param name="xLocus" select="."/>
        <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
        <xsl:with-param name="nPeakCount" select="$nPeakCount"/>
        <xsl:with-param name="collate" select="$collate"/>
      </xsl:call-template>
    </xsl:for-each>
    <xsl:call-template name="tWriteEndOfSampleLine">
      <xsl:with-param name="xSample" select="$xSample"/>
      <xsl:with-param name="nStyle" select="$StyleLineBySample"/>
      <xsl:with-param name="nLine" select="1"/>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
  </xsl:template>

  <xsl:template name="tHeaderPrefix">
    <xsl:param name="nStyle"/>
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>

    <xsl:variable name="sBegin">
      <xsl:call-template name="tBeforeHeaderSampleName">
        <xsl:with-param name="nStyle" select="$nStyle"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sEnd">
      <xsl:call-template name="tAfterHeaderSampleName">
        <xsl:with-param name="nStyle" select="$nStyle"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sSampleName">
      <xsl:choose>
        <xsl:when test="($nShowFileName &gt; 0) and ($nShowSampleName &gt; 0) and ($nShowSampleName &lt; $nShowFileName)">
          <xsl:call-template name="tHeaderSampleName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
          <xsl:value-of select="$DELIM"/>
          <xsl:call-template name="tHeaderFileName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="($nShowFileName &gt; 0) and ($nShowSampleName &gt; 0)">
          <xsl:call-template name="tHeaderFileName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
          <xsl:value-of select="$DELIM"/>
          <xsl:call-template name="tHeaderSampleName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$nShowFileName &gt; 0">
          <xsl:call-template name="tHeaderFileName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$nShowSampleName &gt; 0">
          <xsl:call-template name="tHeaderSampleName">
            <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
            <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="sHeaderPrefix">
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sBegin"/>
      </xsl:call-template>
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sSampleName"/>
      </xsl:call-template>
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sEnd"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="os:Chop($sHeaderPrefix,$DELIM)"/>
  </xsl:template>

  <xsl:template name="tSamplePrefix">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>

    <xsl:variable name="sBegin">
      <xsl:call-template name="tBeforeSampleName">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$nStyle"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sEnd">
      <xsl:call-template name="tAfterSampleName">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$nStyle"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sSampleName">
      <xsl:choose>
        <xsl:when test="($nShowFileName &gt; 0) and ($nShowSampleName &gt; 0) and ($nShowSampleName &lt; $nShowFileName)">
          <xsl:value-of select="$xSample/SampleName"/>
          <xsl:value-of select="$DELIM"/>
          <xsl:value-of select="$xSample/Name"/>
        </xsl:when>
        <xsl:when test="($nShowFileName &gt; 0) and ($nShowSampleName &gt; 0)">
          <xsl:value-of select="$xSample/Name"/>
          <xsl:value-of select="$DELIM"/>
          <xsl:value-of select="$xSample/SampleName"/>
        </xsl:when>
        <xsl:when test="$nShowFileName &gt; 0">
          <xsl:value-of select="$xSample/Name"/>
        </xsl:when>
        <xsl:when test="$nShowSampleName &gt; 0">
          <xsl:value-of select="$xSample/SampleName"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="rtn">
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sBegin"/>
      </xsl:call-template>
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sSampleName"/>
      </xsl:call-template>
      <xsl:call-template name="tColumnAndDelim">
        <xsl:with-param name="s" select="$sEnd"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="os:Chop($rtn,$DELIM)"/>
  </xsl:template>

  <xsl:template name="tWriteSample">
    <xsl:param name="xSample"/>
    <xsl:param name="nStyle"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="nMaxPeakCount"/>
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>
    <xsl:param name="collate"/>
    <xsl:param name="nShowChannelNr"/>

    <xsl:variable name="sLinePrefix">
      <xsl:call-template name="tSamplePrefix">
        <xsl:with-param name="xSample" select="$xSample"/>
        <xsl:with-param name="nStyle" select="$nStyle"/>
        <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
        <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$nStyle = $StyleLineByPeak">
        <xsl:call-template name="tWriteSampleByPeak">
          <xsl:with-param name="xSample" select="$xSample"/>
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$nStyle = $StyleLineByLocus">
        <xsl:call-template name="tWriteSampleLocus">
          <xsl:with-param name="xSample" select="$xSample"/>
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="nMaxPeakCount" select="$nMaxPeakCount"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
          <xsl:with-param name="collate" select="$collate"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="tWriteSampleFull">
          <xsl:with-param name="xSample" select="$xSample"/>
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
          <xsl:with-param name="collate" select="$collate"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="tWriteHeaderSampleByPeak">
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="sLinePrefix"/>

    <xsl:value-of select="$sLinePrefix"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:call-template name="tHeaderLocusName"/>
    <xsl:for-each select="$xFieldOrder//x">
      <xsl:value-of select="$DELIM"/>
      <xsl:value-of select="@header"/>
    </xsl:for-each>
    <xsl:call-template name="tDelimAndColumn">
      <xsl:with-param name="s">
        <xsl:call-template name="tEndOfHeaderSampleLine"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
  </xsl:template>

  <xsl:template name="tWriteHeaderSampleFull">
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="sLinePrefix"/>
    <xsl:param name="collate"/>
    <xsl:param name="nShowChannelNr"/>

    <xsl:variable name="nFieldCount" select="count($xFieldOrder//x)"/>

    <!-- if one field, use locus name as column header
         otherwise use two rows, locus then everything else
     -->
    <xsl:if test="$nFieldCount &gt; 1">
      <!-- more than one field, first header row is loci -->
      <xsl:call-template name="tRepeat">
        <!-- empty table cells above prefix columns -->
        <xsl:with-param name="s" select="$DELIM"/>
        <xsl:with-param name="nCount" select="os:FindCount($sLinePrefix,$DELIM)"/>
      </xsl:call-template>
      <xsl:for-each select="$xLocusInfo//locus">
        <xsl:variable name="nCount">
          <xsl:choose>
            <xsl:when test="$collate &lt; 0">
              <xsl:value-of select="$nFieldCount"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$nFieldCount * @count"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="sLocusName">
          <xsl:call-template name="tLocusName">
            <xsl:with-param name="sName" select="@name"/>
            <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
            <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="sLocusRepeat">
          <xsl:call-template name="tRepeat">
            <xsl:with-param name="s" select="concat($DELIM,$sLocusName)"/>
            <xsl:with-param name="nCount" select="$nCount"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$sLocusRepeat"/>
      </xsl:for-each>
      <xsl:call-template name="tEOL"/>
    </xsl:if>

    <!-- header row -->
    <xsl:value-of select="$sLinePrefix"/>
    <xsl:choose>
      <xsl:when test="$nFieldCount = 1 and $collate &gt; -1">
        <!-- when there is only one field, the column header is the locus name -->
        <xsl:for-each select="$xLocusInfo//locus">
          <xsl:variable name="locus">
            <xsl:call-template name="tLocusName">
              <xsl:with-param name="sName" select="@name"/>
              <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
              <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:for-each select="os:CounterXML(1,@count)">
            <xsl:value-of select="$DELIM"/>
            <xsl:value-of select="$locus"/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select="position()"/>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$nFieldCount = 1 and $collate = -1">
        <!-- when there is only one field and collate = minus 1, the column header
          is the locus name and all allele are comma delimited in one cell -->
        <xsl:for-each select="$xLocusInfo//locus">
          <xsl:value-of select="$DELIM"/>
          <xsl:call-template name="tLocusName">
            <xsl:with-param name="sName" select="@name"/>
            <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
            <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$collate = 1">
        <xsl:for-each select="$xLocusInfo//locus">
          <xsl:for-each select="os:CounterXML(1,@count)">
            <xsl:variable name="n" select="position()"/>
            <xsl:for-each select="$xFieldOrder//x">
              <xsl:value-of select="$DELIM"/>
              <xsl:value-of select="@header"/>
              <xsl:text>-</xsl:text>
              <xsl:value-of select="$n"/>
            </xsl:for-each>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$collate = 0">
        <xsl:for-each select="$xLocusInfo//locus">
          <xsl:variable name="xCounter" select="os:CounterXML(1,@count)"/>
          <xsl:for-each select="$xFieldOrder//x">
            <xsl:variable name="sHeader" select="@header"/>
            <xsl:for-each select="$xCounter">
              <xsl:value-of select="$DELIM"/>
              <xsl:value-of select="$sHeader"/>
              <xsl:text>-</xsl:text>
              <xsl:value-of select="position()"/>
            </xsl:for-each>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$collate = -1">
        <!-- collate = minus 1 one column per field (comma separated list in column) -->
        <xsl:for-each select="$xLocusInfo//locus">
          <xsl:for-each select="$xFieldOrder//x">
            <xsl:value-of select="$DELIM"/>
            <xsl:value-of select="@header"/>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>
    <xsl:call-template name="tEOL"/>
  </xsl:template>

  <xsl:template name="tWriteHeaderSampleLocus">
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="nMaxPeakCount"/>
    <xsl:param name="sLinePrefix"/>
    <xsl:param name="collate"/>

    <xsl:value-of select="$sLinePrefix"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:call-template name="tHeaderLocusName"/>
    <xsl:variable name="xCounter" select="os:CounterXML(1,$nMaxPeakCount)"/>

    <xsl:choose>
      <xsl:when test="$collate = 1">
        <xsl:for-each select="$xCounter">
          <xsl:variable name="nPos" select="position()"/>
          <xsl:for-each select="$xFieldOrder//x">
            <xsl:value-of select="$DELIM"/>
            <xsl:value-of select="@header"/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select="$nPos"/>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$collate = -1">
        <xsl:for-each select="$xFieldOrder//x">
          <xsl:value-of select="$DELIM"/>
          <xsl:value-of select="@header"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:for-each select="$xFieldOrder//x">
          <xsl:variable name="header" select="@header"/>
          <xsl:for-each select="$xCounter">
            <xsl:value-of select="$DELIM"/>
            <xsl:value-of select="$header"/>
            <xsl:text>-</xsl:text>
            <xsl:value-of select="position()"/>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:call-template name="tDelimAndColumn">
      <xsl:with-param name="s">
        <xsl:call-template name="tEndOfHeaderSampleLine"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
  </xsl:template>

  <xsl:template name="tWriteHeaders">
    <xsl:param name="nStyle"/>
    <xsl:param name="xFieldOrder"/>
    <xsl:param name="xLocusInfo"/>
    <xsl:param name="nMaxPeakCount"/>
    <xsl:param name="nShowFileName"/>
    <xsl:param name="nShowSampleName"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:param name="collate"/>

    <xsl:variable name="sLinePrefix">
      <xsl:call-template name="tHeaderPrefix">
        <xsl:with-param name="nStyle" select="$nStyle"/>
        <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
        <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$nStyle = $StyleLineByPeak">
        <xsl:call-template name="tWriteHeaderSampleByPeak">
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$nStyle = $StyleLineByLocus">
        <xsl:call-template name="tWriteHeaderSampleLocus">
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="nMaxPeakCount" select="$nMaxPeakCount"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
          <xsl:with-param name="collate" select="$collate"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="tWriteHeaderSampleFull">
          <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
          <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
          <xsl:with-param name="sLinePrefix" select="$sLinePrefix"/>
          <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
          <xsl:with-param name="collate" select="$collate"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="tExportAll">
    <xsl:param name="pStyle"/>
    <xsl:param name="ShowFileName"/>
    <xsl:param name="nShowChannelNr"/>
    <xsl:param name="pCollate"/>
    <xsl:param name="includeLadder" select="0"/>
    <xsl:param name="includeDisabled" select="0"/>
    <xsl:param name="includePosControl" select="0"/>
    <xsl:param name="includeNegControl" select="0"/>

    <xsl:param name="allele" select="0"/>
    <xsl:param name="bps" select="0"/>
    <xsl:param name="ilsbps" select="0"/>
    <xsl:param name="rfu" select="0"/>
    <xsl:param name="area" select="0"/>
    <xsl:param name="width" select="0"/>
    <xsl:param name="time" select="0"/>
    <xsl:param name="fit" select="0"/>
    <xsl:param name="residual" select="0"/>

    <xsl:variable name="tmpShowFileSampleName">
      <xsl:variable name="sl" select="os:lower($ShowFileName)"/>
      <xsl:variable name="qFile" select="contains($sl,'file')"/>
      <xsl:variable name="qSample" select="contains($sl,'sample')"/>
      <xsl:variable name="qBoth" select="$qFile and $qSample"/>
      <xsl:choose>
        <xsl:when test="$qFile and not($qBoth)">10</xsl:when>
        <xsl:when test="$qSample and not($qBoth)">01x</xsl:when>
        <xsl:when test="$qBoth and contains(substring-before($sl,'sample'),'file')">12</xsl:when>
        <xsl:when test="$qBoth">21</xsl:when>
        <xsl:when test="os:Number0($ShowFileName) != 0">10</xsl:when>
        <xsl:otherwise>01x</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="nShowFileName" select="number(substring($tmpShowFileSampleName,1,1))"/>
    <xsl:variable name="nShowSampleName" select="number(substring($tmpShowFileSampleName,2,1))"/>
    <xsl:variable name="collate">
      <xsl:variable name="sl" select="os:lower($pCollate)"/>
      <xsl:choose>
        <xsl:when test="contains($sl,'comma')">-1</xsl:when>
        <xsl:when test="contains($sl,'peak')">1</xsl:when>
        <xsl:otherwise><xsl:value-of select="os:Number0($pCollate)"/></xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="nStyle">
      <xsl:variable name="sl" select="os:lower($pStyle)"/>
      <xsl:choose>
        <xsl:when test="contains($sl,'sample')"><xsl:value-of select="$StyleLineBySample"/></xsl:when>
        <xsl:when test="contains($sl,'locus')"><xsl:value-of select="$StyleLineByLocus"/></xsl:when>
        <xsl:when test="contains($sl,'peak')"><xsl:value-of select="$StyleLineByPeak"/></xsl:when>
        <xsl:when test="$pStyle = $StyleLineBySample"><xsl:value-of select="$pStyle"/></xsl:when>
        <xsl:when test="$pStyle = $StyleLineByLocus"><xsl:value-of select="$pStyle"/></xsl:when>
        <xsl:when test="$pStyle = $StyleLineBySample"><xsl:value-of select="$pStyle"/></xsl:when>
        <xsl:otherwise><xsl:value-of select="$StyleLineByLocus"/></xsl:otherwise>
      </xsl:choose>
    </xsl:variable>


    <xsl:variable name="tmpFieldOrder">
      <xsl:call-template name="tBuildFieldOrder">
        <xsl:with-param name="allele" select="$allele"/>
        <xsl:with-param name="bps" select="$bps"/>
        <xsl:with-param name="ilsbps" select="$ilsbps"/>
        <xsl:with-param name="rfu" select="$rfu"/>
        <xsl:with-param name="area" select="$area"/>
        <xsl:with-param name="width" select="$width"/>
        <xsl:with-param name="time" select="$time"/>
        <xsl:with-param name="fit" select="$fit"/>
        <xsl:with-param name="residual" select="$residual"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="xFieldOrder" select="exsl:node-set($tmpFieldOrder)/xml"/>
    <xsl:variable name="xSamples"
      select="os:IncludedSamples($includeDisabled, $includeLadder, $includeNegControl, $includePosControl)"/>
    <xsl:variable name="tmpLocusInfo">
      <xsl:choose>
        <xsl:when test="$nStyle = $StyleLineBySample">
          <xsl:call-template name="tPeakCountByLocus">
            <xsl:with-param name="xSamples" select="$xSamples"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="tChannelByLocus"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="xLocusInfo" select="exsl:node-set($tmpLocusInfo)/xml"/>
    <xsl:variable name="nMaxPeakCount">
      <xsl:choose>
        <xsl:when test="$nStyle = $StyleLineByLocus">
          <xsl:value-of select="os:MaxLocusPeakCount($xSamples)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:call-template name="tWriteHeaders">
      <xsl:with-param name="nStyle" select="$nStyle"/>
      <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
      <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
      <xsl:with-param name="nMaxPeakCount" select="$nMaxPeakCount"/>
      <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
      <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
      <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
      <xsl:with-param name="collate" select="$collate"/>
    </xsl:call-template>

    <xsl:call-template name="tAfterWriteHeaders">
      <xsl:with-param name="nStyle" select="$nStyle"/>
      <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
      <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
      <xsl:with-param name="nMaxPeakCount" select="$nMaxPeakCount"/>
      <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
      <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
      <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
      <xsl:with-param name="collate" select="$collate"/>
      <xsl:with-param name="includeLadder" select="$includeLadder"/>
      <xsl:with-param name="includeDisabled" select="$includeDisabled"/>
      <xsl:with-param name="includePosControl" select="$includePosControl"/>
      <xsl:with-param name="includeNegControl" select="$includeNegControl"/>
    </xsl:call-template>

    <xsl:for-each select="$xSamples">
      <xsl:call-template name="tWriteSample">
        <xsl:with-param name="xSample" select="."/>
        <xsl:with-param name="nStyle" select="$nStyle"/>
        <xsl:with-param name="xFieldOrder" select="$xFieldOrder"/>
        <xsl:with-param name="xLocusInfo" select="$xLocusInfo"/>
        <xsl:with-param name="nMaxPeakCount" select="$nMaxPeakCount"/>
        <xsl:with-param name="nShowFileName" select="$nShowFileName"/>
        <xsl:with-param name="nShowSampleName" select="$nShowSampleName"/>
        <xsl:with-param name="nShowChannelNr" select="$nShowChannelNr"/>
        <xsl:with-param name="collate" select="$collate"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="TEST_STR_AND_LEN">
    <xsl:param name="s"/>
    <xsl:param name="name" select="'s'"/>
    <xsl:value-of select="$name"/>
    <xsl:text>=&quot;</xsl:text>
    <xsl:value-of select="$s"/>
    <xsl:text>&quot;; length=</xsl:text>
    <xsl:value-of select="string-length($s)"/>
  </xsl:template>

  <xsl:template name="TEST_CHOP">
    <xsl:text>TEST_CHOP</xsl:text>
    <xsl:call-template name="tEOL"/>
    <xsl:call-template name="tEOL"/>
    <xsl:variable name="s">
      <xsl:text>This ends with DELIM</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:value-of select="$DELIM"/>
      <xsl:value-of select="$DELIM"/>
    </xsl:variable>
    <xsl:variable name="sChop">
      <xsl:call-template name="tChop">
        <xsl:with-param name="str" select="$s"/>
        <xsl:with-param name="substr" select="$DELIM"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="sChopAll">
      <xsl:call-template name="tChop">
        <xsl:with-param name="str" select="$s"/>
        <xsl:with-param name="substr" select="$DELIM"/>
        <xsl:with-param name="all" select="1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="TEST_STR_AND_LEN">
      <xsl:with-param name="s" select="$s"/>
      <xsl:with-param name="name" select="'s       '"/>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
    <xsl:call-template name="TEST_STR_AND_LEN">
      <xsl:with-param name="s" select="$sChop"/>
      <xsl:with-param name="name" select="'sChop   '"/>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
    <xsl:call-template name="TEST_STR_AND_LEN">
      <xsl:with-param name="s" select="$sChopAll"/>
      <xsl:with-param name="name" select="'sChopAll'"/>
    </xsl:call-template>
    <xsl:call-template name="tEOL"/>
    <xsl:call-template name="tEOL"/>
  </xsl:template>

  <xsl:template name="TEST">
    <xsl:call-template name="TEST_CHOP"/>
  </xsl:template>

  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="$pTest">
        <xsl:call-template name="TEST"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="tExportAll">
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
          <xsl:with-param name="time" select="$p20Time"/>
          <xsl:with-param name="fit" select="$p27Fit"/>
          <xsl:with-param name="residual" select="$p28Residual"/>

        </xsl:call-template>
      </xsl:otherwise>

    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
