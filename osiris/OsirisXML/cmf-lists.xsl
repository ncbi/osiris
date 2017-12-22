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
#  FileName: cmf-lists.xsl
#  Author:   Douglas Hoffman
#  Description:  Create a list of valid kit and locus names for CMF files
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
  exclude-result-prefixes="xsl xsd"
  extension-element-prefixes="xsl xsd"
  version="1.1">

  <xsl:output
    method="xml"
    standalone="yes"
    indent="yes"
    encoding="UTF-8"
    version="1.0"/>

  <xsl:template name="write-list">
    <xsl:param name="type"/>
    <xsl:variable name="list"
      select="/xsd:schema/xsd:simpleType[@name = $type]/xsd:restriction/xsd:enumeration"/>
    <xsl:element name="{$type}">
      <xsl:for-each select="$list">
        <Item><xsl:value-of select="@value"/></Item>
      </xsl:for-each>
    </xsl:element>
  </xsl:template>
    
  <xsl:template match="/">
    <CmfList>
      <xsl:call-template name="write-list">
        <xsl:with-param name="type" select="'KitType'"/>
      </xsl:call-template>
      <xsl:call-template name="write-list">
        <xsl:with-param name="type" select="'SpecimenCategoryType'"/>
      </xsl:call-template>
      <xsl:call-template name="write-list">
        <xsl:with-param name="type" select="'LocusNameType'"/>
      </xsl:call-template>
    </CmfList>
  </xsl:template>

</xsl:stylesheet>
