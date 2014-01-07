<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text"/>

  <xsl:key name="message"
    match="/OsirisAnalysisReport/Messages/Message[not(Hidden) or (string(Hidden) != 'true') and string(MsgName)]"
    use="MessageNumber"/>
  <xsl:key name="export"
    match="/OsirisAnalysisReport/ExportSpecifications/MsgExport"
    use="MsgName"/>

  <xsl:variable name="DELIM" select="'&#9;'"/>
  <xsl:variable name="TAB" select="'&#9;'"/>
  <xsl:variable name="EOL" select="'&#10;'"/>
  <xsl:variable name="loci"
    select="/OsirisAnalysisReport/Heading/Channel/LocusName"/>
  <xsl:variable name="HINUMBER" select="20000000"/>
  <xsl:variable name="DEFAULT123">
    <xsl:value-of select="$HINUMBER"/>
    <xsl:text>:</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:value-of select="$HINUMBER"/>
    <xsl:text>:</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:value-of select="$HINUMBER"/>
    <xsl:text>:</xsl:text>
  </xsl:variable>
  <xsl:variable name="DONOTEXPORT">
    <xsl:text>0:X</xsl:text>
  </xsl:variable>
  <xsl:variable name="DONOTEXPORT3">
    <xsl:value-of select="$DONOTEXPORT"/>
    <xsl:value-of select="$TAB"/>
    <xsl:value-of select="$DONOTEXPORT"/>
    <xsl:value-of select="$TAB"/>
    <xsl:value-of select="$DONOTEXPORT"/>
  </xsl:variable>

  <!-- string format for UD text is tab delimited items of priority:message
       where UD1 is first, UD2 is after the first tab, UD3 is after the second tab
 
       Here is an example, with UD3 being empty, others having a priority of 12

       12:Text[tab]12:ud2 text[tab]99999:

       [tab] is not literal

   -->


  <xsl:template name="str-replace">
    <xsl:param name="str"/>
    <xsl:param name="kill"/>
    <xsl:param name="replace"/>
    <xsl:choose>
      <xsl:when test="$kill = ''">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="not(contains($str,$kill))">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="substring-before($str,$kill)"/>
        <xsl:value-of select="$replace"/>
        <xsl:call-template name="str-replace">
          <xsl:with-param name="str" select="substring-after($str,$kill)"/>
          <xsl:with-param name="kill" select="$kill"/>
          <xsl:with-param name="replace" select="$replace"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="substring-after">
    <xsl:param name="str"/>
    <xsl:param name="substr"/>
    <xsl:param name="occurrence" select="$HINUMBER"/>
    <xsl:param name="beforeNext" select="0"/>

    <!-- to get the substring after the last occurrence, 
         omit param occurrence to use a very high number -->

    <xsl:choose>
      <xsl:when test="$substr = ''">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="not(contains($str,$substr))">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="$occurrence &gt; 1">
        <xsl:call-template name="substring-after">
          <xsl:with-param name="str" select="substring-after($str,$substr)"/>
          <xsl:with-param name="substr" select="$substr"/>
          <xsl:with-param name="occurrence" select="$occurrence - 1"/>
          <xsl:with-param name="beforeNext" select="$beforeNext"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when 
        test="string-length($beforeNext) and ($beforeNext != 'false') and ($beforeNext != '0')">
        <xsl:value-of select="substring-before($str,$substr)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="getPriority">

    <!-- extract the priority of a single string of number:text -->

    <xsl:param name="s"/>

    <xsl:variable name="n">
      <xsl:choose>
        <xsl:when test="contains($s,':')">
          <xsl:variable name="n1">
            <xsl:value-of select="number(substring-before($s,':'))"/>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="string($n1) = 'NaN'">
              <xsl:value-of select="$HINUMBER"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$n1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$HINUMBER"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$n"/>
  </xsl:template>

  <xsl:template name="chooseUD">
    <xsl:param name="UD"/>
    <xsl:param name="UDnew"/>
    <xsl:variable name="n">
      <xsl:call-template name="getPriority">
        <xsl:with-param name="s" select="$UD"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="nNew">
      <xsl:call-template name="getPriority">
        <xsl:with-param name="s" select="$UDnew"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$nNew &lt; $n">
        <xsl:value-of select="$UDnew"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$UD"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  
  <xsl:template name="chooseUDs">
    <xsl:param name="UDs"/>
    <xsl:param name="UDsnew"/>
    <xsl:param name="delim" select="$TAB"/>
    <xsl:choose>
      <xsl:when test="$UDs = ''">
        <xsl:value-of select="$UDsnew"/>
      </xsl:when>
      <xsl:when test="$UDsnew = ''">
        <xsl:value-of select="$UDs"/>
      </xsl:when>
      <xsl:when test="contains($UDs,$delim) and contains($UDsnew,$delim)">
        <xsl:call-template name="chooseUD">
          <xsl:with-param name="UD" select="substring-before($UDs,$delim)"/>
          <xsl:with-param name="UDnew" select="substring-before($UDsnew,$delim)"/>
        </xsl:call-template>
        <xsl:value-of select="$delim"/>
        <xsl:call-template name="chooseUDs">
          <xsl:with-param name="UDs" select="substring-after($UDs,$delim)"/>
          <xsl:with-param name="UDsnew" select="substring-after($UDsnew,$delim)"/>
          <xsl:with-param name="delim" select="$delim"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="chooseUD">
          <xsl:with-param name="UD" select="$UDs"/>
          <xsl:with-param name="UDnew" select="$UDsnew"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="chooseUDsPlus">
    <xsl:param name="UDs1"/>
    <xsl:param name="UDs2"/>
    <xsl:param name="UDs3"/>
    <xsl:param name="UDs4"/>
    <xsl:param name="UDs5"/>
    <xsl:param name="UDs6"/>
    <xsl:param name="UDs7"/>
    <xsl:param name="UDs8"/>
    <xsl:param name="UDs9"/>
    <xsl:param name="delim" select="$TAB"/>
    <xsl:param name="count" select="0"/>
    <xsl:choose>
      <xsl:when test="$count = 1">
        <xsl:value-of select="$UDs1"/>
      </xsl:when>
      <xsl:when test="$count = 2">
        <xsl:call-template name="chooseUDs">
          <xsl:with-param name="UDs" select="$UDs1"/>
          <xsl:with-param name="UDsnew" select="$UDs2"/>
          <xsl:with-param name="delim" select="$delim"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$count &gt; 2">
        <xsl:variable name="UDs12">
          <xsl:call-template name="chooseUDs">
            <xsl:with-param name="UDs" select="$UDs1"/>
            <xsl:with-param name="UDsnew" select="$UDs2"/>
            <xsl:with-param name="delim" select="$delim"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:call-template name="chooseUDsPlus">
          <xsl:with-param name="UDs1" select="$UDs12"/>
          <xsl:with-param name="UDs2" select="$UDs3"/>
          <xsl:with-param name="UDs3" select="$UDs4"/>
          <xsl:with-param name="UDs4" select="$UDs5"/>
          <xsl:with-param name="UDs5" select="$UDs6"/>
          <xsl:with-param name="UDs6" select="$UDs7"/>
          <xsl:with-param name="UDs7" select="$UDs8"/>
          <xsl:with-param name="UDs8" select="$UDs9"/>
          <xsl:with-param name="count" select="$count - 1"/>
          <xsl:with-param name="delim" select="$delim"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="exportMessage">
    <xsl:param name="export"/>
    <xsl:param name="location"/>
    <xsl:param name="delim" select="':'"/>

    <xsl:variable name="node" select="$export/Protocol/TextLoc[Location = $location]"/>
    <xsl:choose>
      <xsl:when test="$node">
        <xsl:variable name="priority" select="string($export/Protocol/Priority)"/>
        <xsl:value-of select="$priority"/>
        <xsl:if test="$priority = ''">
          <xsl:value-of select="$HINUMBER"/>
        </xsl:if>
        <xsl:value-of select="$delim"/>
        <xsl:value-of select="$node/Text"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$HINUMBER"/>
        <xsl:value-of select="$delim"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="exportMessages">
    <xsl:param name="export"/>
    <xsl:param name="locations"/>
    <xsl:param name="delim" select="':'"/>
    <xsl:choose>
      <xsl:when test="$locations = ''"/>
      <xsl:when test="contains($locations,$delim)">
        <xsl:variable name="loc" select="substring-before($locations,$delim)"/>
        <xsl:variable name="nextLoc" select="substring-after($locations,$delim)"/>
        <xsl:call-template name="exportMessage">
          <xsl:with-param name="export" select="$export"/>
          <xsl:with-param name="location" select="$loc"/>
        </xsl:call-template>

        <xsl:if test="string-length($nextLoc)">
          <xsl:value-of select="$TAB"/>
          <xsl:call-template name="exportMessages">
            <xsl:with-param name="export" select="$export"/>
            <xsl:with-param name="locations" select="$nextLoc"/>
            <xsl:with-param name="delim" select="$delim"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="exportMessage">
          <xsl:with-param name="export" select="$export"/>
          <xsl:with-param name="location" select="$locations"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="formatExportByMessage">
    <xsl:param name="nMSG"/>
    <xsl:variable name="message" select="key('message',$nMSG)"/>
    <xsl:if test="$message">
      <xsl:variable name="export" select="key('export',$message/MsgName)"/>
      <xsl:if test="$export">
        <xsl:variable name="new1">
          <xsl:choose>
            <xsl:when 
              test="$export/Protocol/DoNotExport and ($export/Protocol/DoNotExport = 'true')">
              <xsl:value-of select="$DONOTEXPORT3"/>
              <xsl:if test="$export/Protocol/Notification">
                <xsl:message>
                  <xsl:value-of select="$export/Protocol/Notification"/>
                </xsl:message>
              </xsl:if>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="exportMessages">
                <xsl:with-param name="export" select="$export"/>
                <xsl:with-param name="locations" select="'UD1:UD2:UD3'"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:value-of select="$new1"/>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template name="getUD123">
    <xsl:param name="UD123" select="$DEFAULT123"/>
    <xsl:param name="ndx" select="1"/>
    <xsl:param name="nodeset"/>
    <xsl:param name="nCount" select="count($nodeset)"/>

    <xsl:choose>
      <xsl:when test="$ndx &gt; $nCount">
        <xsl:value-of select="$UD123"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="check123">
          <xsl:variable name="n" select="$nodeset[$ndx]"/>
          <xsl:call-template name="formatExportByMessage">
            <xsl:with-param name="nMSG" select="$n"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="next123">
          <xsl:choose>
            <xsl:when test="string-length($check123)">
              <xsl:call-template name="chooseUDs">
                <xsl:with-param name="UDs" select="$UD123"/>
                <xsl:with-param name="UDsnew" select="$check123"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$UD123"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:call-template name="getUD123">
          <xsl:with-param name="UD123" select="$next123"/>
          <xsl:with-param name="ndx" select="$ndx + 1"/>
          <xsl:with-param name="nCount" select="$nCount"/>
          <xsl:with-param name="nodeset" select="$nodeset"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="directoryUD">
    <xsl:call-template name="getUD123">
      <xsl:with-param name="nodeset"
        select="/OsirisAnalysisReport/DirectoryAlerts/MessageNumber"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="sampleUD">
    <xsl:param name="sample"/>
    <xsl:variable name="set1">
      <!-- sample and ILS -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/*/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="set2">
      <!-- Channel -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/ChannelAlerts/Channel/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="set3">
      <!-- Channel/Artifact -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/ChannelAlerts/Channel/Artifact[not(Disabled) or (Disabled != 'true')]/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="set4">
      <!-- Interlocus -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$sample/InterlocusAlerts/Alert/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:call-template name="chooseUDsPlus">
      <xsl:with-param name="UDs1" select="$set1"/>
      <xsl:with-param name="UDs2" select="$set2"/>
      <xsl:with-param name="UDs3" select="$set3"/>
      <xsl:with-param name="UDs4" select="$set4"/>
      <xsl:with-param name="count" select="4"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="locusUD">
    <xsl:param name="locus"/>

    <xsl:variable name="set1">
      <!-- LocusAlerts -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$locus/LocusAlerts/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="set2">
      <!-- Allele Alerts -->
      <xsl:call-template name="getUD123">
        <xsl:with-param name="nodeset"
          select="$locus/Allele[not(Disabled) or (Disabled != 'true')]/MessageNumber"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="chooseUDs">
      <xsl:with-param name="UDs" select="$set1"/>
      <xsl:with-param name="UDsnew" select="$set2"/>
    </xsl:call-template>
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

  <xsl:template name="Empty">
    <xsl:param name="count" select="0"/>
    <xsl:if test="$count &gt; 0">
      <xsl:value-of select="$DELIM"/>
      <xsl:call-template name="Empty">
        <xsl:with-param name="count" select="$count - 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>


  <xsl:template name="AlleleName">
    <xsl:param name="name"/>
    <xsl:param name="locus"/>
    <xsl:param name="offLadder"/>
    <xsl:choose>
      <xsl:when test="not($locus = 'AMEL')">
        <xsl:if test="$offLadder = 'true'">
          <xsl:text>OL</xsl:text>
        </xsl:if>
        <xsl:value-of select="$name"/>
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

  <xsl:template name="extractUDs">
    <xsl:param name="UDs"/>
    <xsl:param name="delim" select="$TAB"/>
    <xsl:param name="delimValue" select="':'"/>
    <xsl:choose>
      <xsl:when test="contains($UDs,$delim)">
        <xsl:call-template name="extractUDs">
          <xsl:with-param name="UDs" select="substring-before($UDs,$delim)"/>
          <xsl:with-param name="delim" select="$delim"/>
          <xsl:with-param name="delimValue" select="$delimValue"/>
        </xsl:call-template>
        <xsl:value-of select="$delim"/>
        <xsl:call-template name="extractUDs">
          <xsl:with-param name="UDs" select="substring-after($UDs,$delim)"/>
          <xsl:with-param name="delim" select="$delim"/>
          <xsl:with-param name="delimValue" select="$delimValue"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($UDs,$delimValue)">
        <xsl:value-of select="substring-after($UDs,$delimValue)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$UDs"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="formatUDs">
    <xsl:param name="str"/>
    <xsl:param name="sampleName"/>
    <xsl:param name="locusName"/>
    <xsl:param name="alleles"/>
    
    <xsl:choose>
      <xsl:when test="contains($str,'$(')">
        <xsl:variable name="formatSample">
          <xsl:call-template name="str-replace">
            <xsl:with-param name="str" select="$str"/>
            <xsl:with-param name="kill" select="'$(Sample)'"/>
            <xsl:with-param name="replace" select="$sampleName"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="formatLocus">
          <xsl:call-template name="str-replace">
            <xsl:with-param name="str" select="$formatSample"/>
            <xsl:with-param name="kill" select="'$(Locus)'"/>
            <xsl:with-param name="replace" select="$locusName"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="contains($formatLocus,'$(Alleles)')">
            <xsl:variable name="alleleList">
              <xsl:for-each select="$alleles">
                <xsl:if test="position() != 1">
                  <xsl:text>,</xsl:text>
                </xsl:if>
                <xsl:call-template name="AlleleName">
                  <xsl:with-param name="name" select="Name"/>
                  <xsl:with-param name="locus" select="$locusName"/>
                </xsl:call-template>
              </xsl:for-each>
            </xsl:variable>
            <xsl:call-template name="str-replace">
              <xsl:with-param name="str" select="$formatLocus"/>
              <xsl:with-param name="kill" select="'$(Alleles)'"/>
              <xsl:with-param name="replace" select="$alleleList"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$formatLocus"/>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>


  <xsl:template name="run">


    <xsl:variable name="UD123dir">
      <xsl:call-template name="directoryUD"/>
    </xsl:variable>

    <xsl:if test="not($UD123dir = $DONOTEXPORT3) and /OsirisAnalysisReport/Table/Sample">

      <!-- first line contains run info -->
      <xsl:call-template name="substring-after">
        <xsl:with-param name="str" 
          select="/OsirisAnalysisReport/Table/Sample[1]/Name"/>
        <xsl:with-param name="substr" select="'_'"/>
      </xsl:call-template>
      <xsl:value-of select="$EOL"/>

      <xsl:text>Sample Name</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Marker</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Allele 1</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Allele 2</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Allele 3</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Allele 4</xsl:text>
      <xsl:value-of select="$DELIM"/>

      <xsl:text>UD1</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>UD2</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>UD3</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Sample File</xsl:text>
      <xsl:value-of select="$EOL"/>

      <xsl:for-each select="/OsirisAnalysisReport/Table/Sample">
        <xsl:variable name="sample" select="."/>

        <xsl:variable name="enabled">
          <xsl:call-template name="IsEnabled">
            <xsl:with-param name="sample" select="$sample"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$enabled != 0">
          <xsl:variable name="UD123sample">
            <xsl:variable name="sampleLevel">
              <xsl:call-template name="sampleUD">
                <xsl:with-param name="sample" select="$sample"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:call-template name="chooseUDs">
              <xsl:with-param name="UDs" select="$sampleLevel"/>
              <xsl:with-param name="UDsnew" select="$UD123dir"/>
            </xsl:call-template>
          </xsl:variable>

          <xsl:if test="not($UD123sample = $DONOTEXPORT3)">
            <xsl:variable name="sampleName">
              <xsl:choose>
                <xsl:when 
                  test="SampleName and (string-length(SampleName) &gt; 0)">
                  <xsl:value-of select="SampleName"/>
                </xsl:when>
                <xsl:when test="contains(Name,'_')">
                  <xsl:value-of select="substring-before(Name,'_')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="Name"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:for-each select="$loci">
              <xsl:variable name="locusName" select="."/>
              <xsl:variable name="locus"
                select="$sample/Locus[LocusName = $locusName]"/>
              <xsl:variable name="alleles" 
                select="$sample/Locus[LocusName = $locusName]/Allele[not(Disabled) or (Disabled != 'true')]"/>
              <xsl:variable name="UD123locus">
                <xsl:choose>
                  <xsl:when test="$locus">
                    <xsl:call-template name="locusUD">
                      <xsl:with-param name="locus" select="$locus"/>
                    </xsl:call-template>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$DEFAULT123"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:variable>
              <xsl:if test="not($UD123locus = $DONOTEXPORT3)">
                <xsl:variable name="UDformatted">
                  <xsl:variable name="UDfinal">
                    <xsl:call-template name="chooseUDs">
                      <xsl:with-param name="UDs" select="$UD123locus"/>
                      <xsl:with-param name="UDsnew" select="$UD123sample"/>
                    </xsl:call-template>
                  </xsl:variable>
                  <xsl:variable name="UDextract">
                    <xsl:call-template name="extractUDs">
                      <xsl:with-param name="UDs" select="$UDfinal"/>
                    </xsl:call-template>
                  </xsl:variable>

                  <xsl:call-template name="formatUDs">
                    <xsl:with-param name="str" select="$UDextract"/>
                    <xsl:with-param name="sampleName" select="$sampleName"/>
                    <xsl:with-param name="locusName" select="$locusName"/>
                    <xsl:with-param name="alleles" select="$alleles"/>
                  </xsl:call-template>
                </xsl:variable>

                <!-- Sample Name -->
                <xsl:value-of select="$sampleName"/>
                <xsl:value-of select="$DELIM"/>

                <!-- Marker -->
                <xsl:value-of select="$locusName"/>
                <xsl:value-of select="$DELIM"/>

                <!-- Alleles 1-4 -->
                <xsl:for-each select="$alleles">
                  <xsl:if test="position() &lt; 5">
                    <xsl:call-template name="AlleleName">
                      <xsl:with-param name="name" select="Name"/>
                      <xsl:with-param name="locus" select="$locusName"/>
                      <xsl:with-param name="offLadder" select="OffLadder"/>
                    </xsl:call-template>
                    <xsl:value-of select="$DELIM"/>
                  </xsl:if>
                </xsl:for-each>
                <xsl:variable name="emptyAllele" select="4 - count($alleles)"/>
                <xsl:call-template name="Empty">
                  <xsl:with-param name="count" select="$emptyAllele"/>
                </xsl:call-template>

                <!-- UD 1,2,3  and an empty column -->
                <xsl:value-of select="$UDformatted"/>
                <xsl:value-of select="$DELIM"/>
                <xsl:value-of select="$DELIM"/>                

                <!-- Sample File -->
                <xsl:value-of select="$sample/Name"/>
                <xsl:text>.fsa</xsl:text>

                <xsl:value-of select="$EOL"/>
              </xsl:if><!-- xsl:if test="not($UD123locus = $DONOTEXPORT3)" -->
            </xsl:for-each>
          </xsl:if><!-- xsl:if test="not($UD123sample = $DONOTEXPORT3)" -->
        </xsl:if><!-- xsl:if test="$enabled != 0" -->
      </xsl:for-each>
    </xsl:if><!-- xsl:if test="not($UD123dir = $DONOTEXPORT)" -->

  </xsl:template>


  <xsl:template name="test">
    <xsl:variable name="ud_a"/>
    <xsl:variable name="ud_b">
      <xsl:text>100:number 100&#9;200:number 200&#9;:number NaN</xsl:text>
    </xsl:variable>
    <xsl:variable name="ud_c">
      <xsl:text>50:number 50&#9;201:number 201&#9;xxx:number NaN again</xsl:text>
    </xsl:variable>
    <xsl:variable name="ud_d">
      <xsl:text>55:number 55&#9;202:number 202&#9;400:number 400</xsl:text>
    </xsl:variable>
    <xsl:variable name="ud_e">
      <xsl:text>51:number 51&#9;200:number 200 again&#9;500:number 500</xsl:text>
    </xsl:variable>
    <xsl:variable name="ab">
      <xsl:call-template name="chooseUDs">
        <xsl:with-param name="UDs" select="$ud_a"/>
        <xsl:with-param name="UDsnew" select="$ud_b"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="abc">
      <xsl:call-template name="chooseUDs">
        <xsl:with-param name="UDs" select="$ab"/>
        <xsl:with-param name="UDsnew" select="$ud_c"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="abcd">
      <xsl:call-template name="chooseUDs">
        <xsl:with-param name="UDs" select="$abc"/>
        <xsl:with-param name="UDsnew" select="$ud_d"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="abcde">
      <xsl:call-template name="chooseUDs">
        <xsl:with-param name="UDs" select="$abcd"/>
        <xsl:with-param name="UDsnew" select="$ud_e"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="concat($ab,$EOL,$abc,$EOL,$abcd,$EOL,$abcde,$EOL)"/>
  </xsl:template>

  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="/OsirisAnalysisReport">
        <xsl:call-template name="run"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="test"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
