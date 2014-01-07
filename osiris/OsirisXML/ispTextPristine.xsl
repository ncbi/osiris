<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  exclude-result-prefixes="xsl os"
  extension-element-prefixes="os"
  version="1.0">

  <xsl:import href="isptext.xsl"/>
  <xsl:output method="text" encoding="iso-8859-1"/>

  <xsl:template name="OsirisExport">
  <export>
    <name>STaCS Pristine Only</name>
    <file-extension>tab</file-extension>
    <file-extension>txt</file-extension>
    <file-extension>stx</file-extension>
    <extension-override>true</extension-override>
    <default-location>*A</default-location>
  </export>
  </xsl:template>

  <xsl:template name="SkipSample">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when test="os:IsPristineSample($sample)">
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
