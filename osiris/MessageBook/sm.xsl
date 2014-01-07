<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text" indent="no"/>
  <xsl:strip-space elements="*"/>
  <xsl:template match="/">
    <xsl:for-each select="/ConditionalTestUnits/ConditionalTestUnit/MessageList/SmartMessage/*[name() = 'CountingDataInfo' or name() = 'PercentDataInfo']">
      <xsl:value-of select="position()"/>
      <xsl:text>: </xsl:text>
      <xsl:value-of select="name()"/>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>Name: </xsl:text>
      <xsl:value-of select="../Name"/>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>Number: </xsl:text>
      <xsl:value-of select="../MessageNumber"/>
      <xsl:text>&#10;</xsl:text>
      <xsl:if test="../MessageDescription">
        <xsl:text>Description: </xsl:text>
        <xsl:value-of select="../MessageDescription"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>
      <xsl:if test="../MessageType">
        <xsl:text>Type: </xsl:text>
        <xsl:value-of select="../MessageType"/>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>
      <xsl:for-each select="*">
        <xsl:text>  </xsl:text>
        <xsl:value-of select="name()"/>
        <xsl:text>: </xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>&#10;</xsl:text>
      </xsl:for-each>
      <xsl:text>&#10;</xsl:text>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
