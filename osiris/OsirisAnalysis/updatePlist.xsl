<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:param name="version" select="''"/>
  <xsl:output method="xml" omit-xml-declaration="yes"/>

  <xsl:template match="string[preceding-sibling::*[1]/text() = 'CFBundleVersion' or preceding-sibling::*[1]/text() = 'CFBundleShortVersionString']" priority="20">
    <xsl:message>
      <xsl:text>short version found: </xsl:text>
      <xsl:value-of select="."/>
    </xsl:message>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:value-of select="$version"/>
    </xsl:copy>
  </xsl:template>
  <xsl:template match="string[starts-with(string(.),'OSIRIS version')]" priority="20">
    <xsl:message>
      <xsl:text>version found: </xsl:text>
      <xsl:value-of select="$version"/>
    </xsl:message>
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:choose>
        <xsl:when test="string-length($version) != 0">
          <xsl:text>OSIRIS version </xsl:text>
          <xsl:value-of select="$version"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:copy>
  </xsl:template>
  <xsl:template match="*" priority="10">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
