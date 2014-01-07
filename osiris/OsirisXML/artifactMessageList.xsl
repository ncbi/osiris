<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text"/>
  <xsl:key name="messageFind" match="/OsirisAnalysisReport/Messages/Message[not(Hidden = 'true')]/Text" use="../MessageNumber"/>
  <xsl:param name="CriticalOnly" select="1"/>
  <xsl:param name="IncludeDisabledSamples" select="0"/>
  <xsl:param name="LookupLocus" select="1"/>
  <xsl:param name="MessageText" select="'mixture,three,off ladder'"/>

  <xsl:variable name="EOL" select="'&#13;&#10;'"/>
  <xsl:variable name="TAB" select="'&#9;'"/>
  <xsl:variable name="LF" select="'&#10;'"/>
  <xsl:variable name="MAX_LEVEL" select="19"/>
  <xsl:variable name="SPACES" select="' ,&#9;&#13;&#10;&#160;'"/>
  <xsl:variable name="delim" select="','"/>
  <xsl:variable name="UPPER" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
  <xsl:variable name="lower" select="'abcdefghijklmnopqrstuvwxyz'"/>

  <xsl:variable name="MAX_CHECK">
    <xsl:choose>
      <xsl:when test="$CriticalOnly != 0">
        <xsl:value-of select="19"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="9999"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>


  <xsl:template name="trim">
    <xsl:param name="str"/>
    <xsl:variable name="nLen" select="string-length($str)"/>
    <xsl:choose>
      <xsl:when test="$nLen = 0"/>
      <xsl:when test="contains($SPACES,substring($str,1,1))">
        <!-- begins with space -->
        <xsl:call-template name="trim">
          <xsl:with-param name="str" select="substring($str,2,$nLen - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($SPACES,substring($str,$nLen,1))">
        <!-- ends with space -->
        <xsl:call-template name="trim">
          <xsl:with-param name="str" select="substring($str,1,$nLen - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="trimList">
    <xsl:param name="str"/>

    <xsl:variable name="str1">
      <xsl:call-template name="trim">
        <xsl:with-param name="str" select="$str"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="contains($str1,$delim)">
        <xsl:variable name="leftOfIt">
          <xsl:call-template name="trim">
            <xsl:with-param name="str" select="substring-before($str1,$delim)"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:if test="$leftOfIt != ''">
          <xsl:value-of select="$leftOfIt"/>
          <xsl:value-of select="$delim"/>
        </xsl:if>
        <xsl:call-template name="trimList">
          <xsl:with-param name="str" select="substring-after($str1,$delim)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

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



  <xsl:template name="heading">
    <xsl:text>#</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Sample</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Channel</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Locus</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Time</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Artifact</xsl:text>
    <xsl:if test="$CriticalOnly = 0">
      <xsl:value-of select="$TAB"/>
      <xsl:text>Critical</xsl:text>
    </xsl:if>
    <xsl:value-of select="$EOL"/>
  </xsl:template>

  <xsl:template name="WriteLine">
    <xsl:param name="SamplePosition"/>
    <xsl:param name="Sample"/>
    <xsl:param name="Channel"/>
    <xsl:param name="Locus"/>
    <xsl:param name="AlleleName"/>
    <xsl:param name="Time"/>
    <xsl:param name="ArtifactLabels"/>
    <xsl:param name="Critical"/>

    <xsl:variable name="ArtifactLabel"
      select="substring-before(concat($ArtifactLabels,$LF),$LF)"/>
    <xsl:if test="$ArtifactLabel">
      <xsl:value-of select="$SamplePosition"/>
      <xsl:value-of select="$TAB"/>
      <xsl:value-of select="$Sample"/>
      <xsl:value-of select="$TAB"/>
      <xsl:value-of select="$Channel"/>
      <xsl:value-of select="$TAB"/>
      <xsl:value-of select="$Locus"/>
      <xsl:value-of select="$TAB"/>
      <xsl:value-of select="$AlleleName"/>
      <xsl:value-of select="$TAB"/>
      <xsl:if test="string(number($Time)) != 'NaN'">
        <xsl:value-of
          select="format-number($Time,'0.00')"/>
      </xsl:if>
      <xsl:value-of select="$TAB"/>
      <xsl:value-of select="$ArtifactLabel"/>
      <xsl:if test="$CriticalOnly = 0">
        <xsl:value-of select="$TAB"/>
        <xsl:value-of select="$Critical"/>
      </xsl:if>
      <xsl:value-of select="$EOL"/>
    </xsl:if>
    <xsl:if test="contains($ArtifactLabels,$LF)">
      <xsl:call-template name="WriteLine">
        <xsl:with-param
          name="SamplePosition"
          select="$SamplePosition"/>
        <xsl:with-param
          name="Sample"
          select="$Sample"/>
        <xsl:with-param
          name="Channel"
          select="$Channel"/>
        <xsl:with-param
          name="Locus"
          select="$Locus"/>
        <xsl:with-param
          name="AlleleName"
          select="$AlleleName"/>
        <xsl:with-param
          name="Time"
          select="$Time"/>
        <xsl:with-param
          name="ArtifactLabels"
          select="substring-after($ArtifactLabels,$LF)"/>
        <xsl:with-param
          name="Critical"
          select="$Critical"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="GetLoci">
    <xsl:param name="Channel"/>
    <xsl:param name="Time"/>
    <xsl:param name="plt"/>

    <xsl:variable name="singleLocus" 
      select="$plt/plt/locus[(channel = $Channel) and ($Time &gt;= number(start)) and ($Time &lt;= number(end))]/name"/>
    <xsl:choose>
      <xsl:when test="$singleLocus">
        <xsl:value-of select="$singleLocus"/>
      </xsl:when>
      <xsl:when test="not($plt)">
        <!-- should never happen because -->
        <xsl:message>no plt file</xsl:message>
      </xsl:when>
      <xsl:when test="not($plt/plt/locus)">
        <xsl:message>no /plt/locus in plt file</xsl:message>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="l1">
          <xsl:for-each select="$plt/plt/locus[(channel = $Channel) and ($Time &gt;= number(end))]">
            <xsl:sort select="endExtended" order="descending" data-type="number"/>
            <xsl:if test="position() = 1">
              <xsl:value-of select="name"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>
        <xsl:variable name="l2">
          <xsl:for-each select="$plt/plt/locus[(channel = $Channel) and ($Time &lt;= number(start))]">
            <xsl:sort select="startExtended" order="ascending" data-type="number"/>
            <xsl:if test="position() = 1">
              <xsl:value-of select="name"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>
        <xsl:if test="string($l1) or string($l2)">
          <xsl:value-of select="$l1"/>
          <xsl:text>; </xsl:text>
          <xsl:value-of select="$l2"/>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="WriteLines">
    <xsl:param name="SamplePosition"/>
    <xsl:param name="Artifact"/>
    <xsl:param name="Allele"/>
    <xsl:param name="plt"/>

    <xsl:variable name="Critical">
      <xsl:if test="number($Artifact/Level) &lt;= $MAX_LEVEL">
        <xsl:text>yes</xsl:text>
      </xsl:if>
    </xsl:variable>

    <xsl:if test="($CriticalOnly = 0) or string-length($Critical)">

      <xsl:variable name="Channel" select="$Artifact/../ChannelNr"/>
      <xsl:variable name="Time" select="$Artifact/Time"/>

      <xsl:variable name="AlleleLocus">
        <xsl:choose>
          <xsl:when test="$Allele">
            <xsl:value-of select="$Allele/Name"/>
            <xsl:text>:</xsl:text>
            <xsl:value-of select="$Allele/Locus"/>
          </xsl:when>
          <xsl:when test="$plt">
            <xsl:text>:</xsl:text>
            <xsl:call-template name="GetLoci">
              <xsl:with-param name="Channel" select="$Channel"/>
              <xsl:with-param name="Time" select="$Time"/>
              <xsl:with-param name="plt" select="$plt"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>:</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="AlleleName" 
        select="substring-before($AlleleLocus,':')"/>
      <xsl:variable name="Locus" select="substring-after($AlleleLocus,':')"/>
      <xsl:variable name="Sample" select="$Artifact/../../../Name"/>
      <xsl:variable name="ArtifactLabels" select="$Artifact/Label"/>

      <xsl:call-template name="WriteLine">
        <xsl:with-param
          name="SamplePosition"
          select="$SamplePosition"/>
        <xsl:with-param
          name="Sample"
          select="$Sample"/>
        <xsl:with-param
          name="Channel"
          select="$Channel"/>
        <xsl:with-param
          name="Locus"
          select="$Locus"/>
        <xsl:with-param
          name="AlleleName"
          select="$AlleleName"/>
        <xsl:with-param
          name="Time"
          select="$Time"/>
        <xsl:with-param
          name="ArtifactLabels"
          select="$ArtifactLabels"/>
        <xsl:with-param
          name="Critical"
          select="$Critical"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>


  <xsl:template match="Artifact" priority="100">
    <xsl:param name="SamplePosition"/>
    <xsl:param name="plt"/>
    <xsl:param name="messageSearch"/>

    <!-- xsl:variable name="ArtifactPosition" select="position()"/ -->

    <xsl:if test="not(Disabled) or (Disabled != 'true')">
      <xsl:variable name="Artifact" select="."/>
      <xsl:variable name="ArtifactLabels" select="$Artifact/Label"/>
      <xsl:variable name="ok">
        <xsl:call-template name="checkMessage">
          <xsl:with-param name="messageSearch" select="$messageSearch"/>
          <xsl:with-param name="text" select="$ArtifactLabels"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$ok = 0"/>
        <xsl:when test="Allele">
          <xsl:for-each select="Allele">
            <xsl:call-template name="WriteLines">
              <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
              <xsl:with-param name="Artifact" select="$Artifact"/>
              <xsl:with-param name="Allele" select="."/>
              <xsl:with-param name="plt" select="$plt"/>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="WriteLines">
            <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
            <xsl:with-param name="Artifact" select="$Artifact"/>
            <xsl:with-param name="plt" select="$plt"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="checkMessage">
    <xsl:param name="messageSearch"/>
    <xsl:param name="text"/>

    <xsl:variable name="substr" select="substring-before(concat($messageSearch,$delim),$delim)"/>
    <xsl:choose>
      <xsl:when test="contains($text,$substr)">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when test="contains($messageSearch,$delim)">
        <xsl:call-template name="checkMessage">
          <xsl:with-param name="messageSearch" select="substring-after($messageSearch,$delim)"/>
          <xsl:with-param name="text" select="$text"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match="MessageNumber" priority="100">
    <xsl:param name="messageSearch"/>
    <xsl:param name="SamplePosition"/>
    <xsl:param name="Sample"/>
    <xsl:param name="plt"/>

    <xsl:variable name="text0" select="key('messageFind',.)"/>
    <xsl:if test="string-length($text0) != 0">
      <xsl:variable name="useMessage">
        <xsl:call-template name="checkMessage">
          <xsl:with-param name="messageSearch" select="$messageSearch"/>
          <xsl:with-param name="text" select="translate($text0,$UPPER,$lower)"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:if test="$useMessage = 1">
        <xsl:variable name="Locus" select="string(../../LocusName)"/>
        <xsl:variable name="ChannelNr">
          <xsl:choose>
            <xsl:when test="string-length($Locus)">
              <xsl:value-of select="/OsirisAnalysisReport/Heading/Channel[ LocusName = $Locus ]/ChannelNr"/>
            </xsl:when>
            <xsl:when test="name(..) = 'Channel'">
              <xsl:value-of select="../ChannelNr"/>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="Time">
          <xsl:if test="(string-length($Locus) != 0) and $plt">
            <xsl:value-of select="$plt/plt/locus[name = $Locus]/start"/>
          </xsl:if>
        </xsl:variable>
        <xsl:call-template name="WriteLine">
          <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
          <xsl:with-param name="Sample" select="$Sample/Name"/>
          <xsl:with-param name="Channel" select="$ChannelNr"/>
          <xsl:with-param name="Locus" select="$Locus"/>
          <xsl:with-param name="ArtifactLabels" select="$text0"/>
          <xsl:with-param name="Time" select="$Time"/>
          <xsl:with-param name="Critical" select="'yes'"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:if>

  </xsl:template>

  <xsl:template match="Sample" priority="10">
    <xsl:param name="messageSearch"/>

    <xsl:variable name="SamplePosition" select="position()"/>
    <xsl:variable name="ok">
      <xsl:choose>
        <xsl:when test="$IncludeDisabledSamples = 0">
          <xsl:call-template name="isEnabled">
            <xsl:with-param name="sample" select="."/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="$ok = 1">
      <xsl:choose>
        <!-- check for artifacts to prevent opening the plt file if not necessary -->
        <xsl:when test="$LookupLocus = 0">
          <xsl:if test="string-length($messageSearch)">
            <xsl:apply-templates select=".//MessageNumber[ name(..) != 'Artifact' ]">
              <xsl:with-param name="messageSearch" select="$messageSearch"/>
              <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
              <xsl:with-param name="Sample" select="."/>
            </xsl:apply-templates>
          </xsl:if>
          <xsl:apply-templates select="./ChannelAlerts/Channel/Artifact[Level &lt;= $MAX_CHECK]">
            <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
            <xsl:with-param name="messageSearch" select="$messageSearch"/>
          </xsl:apply-templates>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="plt" select="document(concat(Name,'.plt'),.)"/>
          <xsl:if test="not($plt)">
            <xsl:message>
              <xsl:value-of select="concat('cannot find ',concat(Name,'.plt'))"/>
            </xsl:message>
          </xsl:if>
          <xsl:if test="string-length($messageSearch)">
            <xsl:apply-templates select=".//MessageNumber[ name(..) != 'Artifact' ]">
              <xsl:with-param name="messageSearch" select="$messageSearch"/>
              <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
              <xsl:with-param name="Sample" select="."/>
              <xsl:with-param name="plt" select="$plt"/>
            </xsl:apply-templates>
          </xsl:if>
          <xsl:apply-templates select="./ChannelAlerts/Channel/Artifact[Level &lt;= $MAX_CHECK]">
            <xsl:with-param name="messageSearch" select="$messageSearch"/>
            <xsl:with-param name="SamplePosition" select="$SamplePosition"/>
            <xsl:with-param name="plt" select="$plt"/>
          </xsl:apply-templates>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template match="*" priority="1"/>

  <xsl:template match="/">

    <xsl:variable name="messageSearch">
      <xsl:variable name="s1" select="translate($MessageText,$UPPER,$lower)"/>
      <xsl:call-template name="trimList">
        <xsl:with-param name="str" select="$s1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="heading"/>
    <xsl:apply-templates select="/OsirisAnalysisReport/Table/Sample">
      <xsl:with-param name="messageSearch" select="$messageSearch"/>
    </xsl:apply-templates>
  </xsl:template>

</xsl:stylesheet>
