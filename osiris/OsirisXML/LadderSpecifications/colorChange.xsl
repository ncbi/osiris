<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml" indent="yes" standalone="yes"/>
  <xsl:param name="pct" select="0.9"/>
  <xsl:strip-space elements="*"/>
  <xsl:param name="npct" select="number($pct)"/>
  <xsl:param name="spct" select="string($npct)"/>

  <xsl:template name="fixNumbers">
    <xsl:param name="ss"/>
    <xsl:variable name="comma" select="contains($ss,',')"/>
    <xsl:variable name="sn">
      <xsl:choose>
        <xsl:when test="$comma">
          <xsl:value-of select="substring-before($ss,',')"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$ss"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="n" select="number($sn)"/>
    <xsl:variable name="n2">
      <xsl:choose>
        <xsl:when test="string($n) = 'NaN'">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="round($n * $npct)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$n2 &lt; 0">
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:when test="$n2 &gt; 255">
        <xsl:value-of select="255"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$n2"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="$comma">
      <xsl:text>,</xsl:text>
      <xsl:call-template name="fixNumbers">
        <xsl:with-param name="ss" select="substring-after($ss,',')"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match="analyzed|raw|ladder" priority="20">
    <xsl:copy>
      <xsl:variable name="s" select="string(.)"/>
      <xsl:variable name="nLen" select="string-length($s)"/>
      <xsl:choose>
        <xsl:when test="($spct != 'NaN') and 
                        ($nLen &gt; 5) and
                        (substring($s,1,4) = 'RGB(') and 
                        (substring($s,$nLen,1) = ')')">
          <xsl:variable name="numbers" select="substring($s,5,$nLen - 5)"/>
          <xsl:text>RGB(</xsl:text>
          <xsl:call-template name="fixNumbers">
            <xsl:with-param name="ss" select="$numbers"/>
          </xsl:call-template>
          <xsl:text>)</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$s"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:copy>
  </xsl:template>
  <xsl:template match="*" priority="10">
    <xsl:copy>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
