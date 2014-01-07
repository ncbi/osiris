<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0"
  exclude-result-prefixes="xsl">

  <xsl:template match="/">
    <xsl:variable name="match" select="/xsl:stylesheet/xsl:template[@name = 'OsirisExport'][1]/export"/>
    <xsl:choose>
      <xsl:when test="$match">
        <xsl:copy-of select="$match"/>
      </xsl:when>
      <xsl:otherwise>
        <export/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
