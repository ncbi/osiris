<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:output method="xml"/>
  <xsl:variable name="quot">'</xsl:variable>

  <xsl:template name="stripquot">
    <xsl:param name="str"/>
    
    <xsl:variable name="nLen" select="string-length($str)"/>
    <xsl:choose>
      <xsl:when test="($nLen &gt; 1) and (substring($str,1,1) = $quot) and (substring($str,$nLen,1) = $quot)">
        <xsl:value-of select="substring($str,2,$nLen - 2)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$str"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/">
    <params>
      <xsl:text>&#10;</xsl:text>
      <xsl:for-each select="/xsl:stylesheet/xsl:param">
        <xsl:variable name="value">
          <xsl:choose>
            <xsl:when test="string-length(.) &gt; 0">
              <xsl:value-of select="."/>
            </xsl:when>
            <xsl:when test="string-length(@select) &gt; 0">
              <xsl:call-template name="stripquot">
                <xsl:with-param name="str" select="@select"/>
              </xsl:call-template>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>
        <xsl:text>&#10;</xsl:text>
        <param name="{@name}"><xsl:value-of select="$value"/></param>
      </xsl:for-each>
      <xsl:text>&#10;</xsl:text>
    </params>
  </xsl:template>

</xsl:stylesheet>
