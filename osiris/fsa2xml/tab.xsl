<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text" indent="no" encoding="iso-8859-1"/>
  <xsl:template name="removeQuot">
    <xsl:param name="c"/>
    <xsl:variable name="nLen" select="string-length($c)"/>
    <xsl:variable name="q" select="'&quot;'"/>
    <xsl:choose>
      <xsl:when test="$nLen &lt; 2">
        <xsl:value-of select="$c"/>
      </xsl:when>
      <xsl:when test="substring($c,1,1) = $q and substring($c,$nLen,1) = $q">
        <xsl:value-of select="substring($c,2,$nLen - 2)"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$c"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="$nLen &gt; 1">
      
    </xsl:if>
  </xsl:template>
  <xsl:template match="/">
    <xsl:variable name="EOL" select="'&#13;&#10;'"/>
    <xsl:variable name="DEL" select="'&#9;'"/>
    <xsl:text>Name</xsl:text>
    <xsl:value-of select="$DEL"/>
    <xsl:text>ID</xsl:text>
    <xsl:value-of select="$DEL"/>
    <xsl:text>Size</xsl:text>
    <xsl:value-of select="$DEL"/>
    <xsl:text>Type</xsl:text>
    <xsl:value-of select="$DEL"/>
    <xsl:text>Elements</xsl:text>
    <xsl:value-of select="$DEL"/>
    <xsl:text>Value</xsl:text>
    <xsl:value-of select="$EOL"/>
    <xsl:for-each select="/AB_Root/Data/Tag">
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="Name"/>
      </xsl:call-template>
      <xsl:value-of select="$DEL"/>
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="ID"/>
      </xsl:call-template>
      <xsl:value-of select="$DEL"/>
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="Size"/>
      </xsl:call-template>
      <xsl:value-of select="$DEL"/>
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="Type"/>
      </xsl:call-template>
      <xsl:value-of select="$DEL"/>
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="Elements"/>
      </xsl:call-template>
      <xsl:value-of select="$DEL"/>
      <xsl:call-template name="removeQuot">
        <xsl:with-param name="c" select="Value"/>
      </xsl:call-template>
      <xsl:value-of select="$EOL"/>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
