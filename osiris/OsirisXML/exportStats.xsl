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
#  FileName: exportStats.xsl
#  Author:   Douglas Hoffman
#  Description:  Export detailed allele info for each sample/allele
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:exsl="http://exslt.org/common"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  exclude-result-prefixes="exsl os"
  extension-element-prefixes="exsl os"
  version="1.0">

  <xsl:import href="util.xsl"/>

  <xsl:output method="text"/>
  <xsl:param name="useDisabled" select="0"/>
  <xsl:variable name="EOL" select="'&#13;&#10;'"/>
  <xsl:variable name="DELIM" select="'&#9;'"/>
  <xsl:template name="OsirisExport">
  <export>
    <name>Detailed Allele Spreadsheet</name>
    <file-extension>tab</file-extension>
    <file-extension>txt</file-extension>
    <extension-override>false</extension-override>
    <default-location>*A</default-location>
    <xsl-params>
      <param>
        <name>useDisabled</name>
        <description>Include Disabled Samples</description>
        <type>checkbox</type>
        <checked-value>1</checked-value>
        <unchecked-value>0</unchecked-value>
      </param>
    </xsl-params>
  </export>
  </xsl:template>

  <xsl:template name="max2">
    <xsl:param name="n1"/>
    <xsl:param name="n2"/>
    <xsl:variable name="s1" select="string($n1)"/>
    <xsl:variable name="s2" select="string($n2)"/>
    <xsl:choose>
      <xsl:when test="$s1 = 'NaN' and $s2 = 'NaN'"/>
      <xsl:when test="$s1 = 'NaN'">
        <xsl:value-of select="$s2"/>
      </xsl:when>
      <xsl:when test="$s2 = 'NaN'">
        <xsl:value-of select="$s1"/>
      </xsl:when>
      <xsl:when test="$n1 &gt; $n2">
        <xsl:value-of select="$n1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$n2"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="maxNode">
    <xsl:param name="nodes"/>
    <xsl:variable name="count" select="count($nodes)"/>
    <xsl:choose>
      <xsl:when test="$count = 0"/>
      <xsl:when test="$count = 1">
        <xsl:value-of select="$nodes"/>
      </xsl:when>
      <xsl:when test="$count = 2">
        <xsl:call-template name="max2">
          <xsl:with-param name="n1" select="number($nodes[1])"/>
          <xsl:with-param name="n2" select="number($nodes[2])"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="half" select="floor($count * 0.5)"/>
        <xsl:variable name="n1">
          <xsl:call-template name="maxNode">
            <xsl:with-param name="nodes" select="$nodes[position() &lt;= $half]"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="n2">
          <xsl:call-template name="maxNode">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; $half]"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:call-template name="max2">
          <xsl:with-param name="n1" select="number($n1)"/>
          <xsl:with-param name="n2" select="number($n2)"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="isSampleEnabled">
    <xsl:param name="sample"/>
    <xsl:if test="$useDisabled = 0">
      <xsl:for-each select="$sample/EnableHistory/Enable">
        <xsl:sort select="Time" order="descending" data-type="number"/>
        <xsl:if test="position() = 1">
          <xsl:value-of select="Enabled"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>

  <xsl:template name="maxAllele">
    <xsl:param name="table" select="/OsirisAnalysisReport/Table"/>
    <!-- param used in preparation for multiple files -->
    <xsl:variable name="counts_">
        <xsl:for-each select="$table/Sample">
          <xsl:variable name="enabled">
            <xsl:call-template name="isSampleEnabled">
              <xsl:with-param name="sample" select="."/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$enabled != 'false'">
            <xsl:for-each select="Locus">
              <count>
                <xsl:value-of select="count(Allele[not(Disabled = 'true')])"/>
              </count>
            </xsl:for-each>
          </xsl:if>
        </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="counts" select="exsl:node-set($counts_)"/>
    <xsl:call-template name="maxNode">
      <xsl:with-param name="nodes" select="$counts/count"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="repeat">
    <xsl:param name="str"/>
    <xsl:param name="count" select="0"/>
    <xsl:choose>
      <xsl:when test="os:isNaN($count)"/>
      <xsl:when test="$count &lt; 1"/>
      <xsl:when test="$count &lt; 4">
        <xsl:value-of select="$str"/>
        <xsl:call-template name="repeat">
          <xsl:with-param name="str" select="$str"/>
          <xsl:with-param name="count" select="$count - 1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="half" select="floor($count * 0.5)"/>
        <xsl:call-template name="repeat">
          <xsl:with-param name="str" select="$str"/>
          <xsl:with-param name="count" select="$half"/>
        </xsl:call-template>
        <xsl:call-template name="repeat">
          <xsl:with-param name="str" select="$str"/>
          <xsl:with-param name="count" select="$count - $half"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="AlleleColumns1">
    <xsl:param name="alleleNr" select="1"/>
    <xsl:param name="nAlleles" select="2"/>
    <xsl:if test="$alleleNr &lt;= $nAlleles">
      <xsl:variable name="colHeader">
        <xsl:value-of select="$DELIM"/>
        <xsl:text>allele</xsl:text>
        <xsl:value-of select="$alleleNr"/>
      </xsl:variable>
      <xsl:call-template name="repeat">
        <xsl:with-param name="str" select="$colHeader"/>
        <xsl:with-param name="count" select="6"/>
      </xsl:call-template>
      <xsl:call-template name="AlleleColumns1">
        <xsl:with-param name="alleleNr" select="$alleleNr + 1"/>
        <xsl:with-param name="nAlleles" select="$nAlleles"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="writeAllele">
    <xsl:param name="allele"/>
    <!-- 
         column order, needs to be consistent with template headerRows
          Call
          RFU
          BPS
          Residual
          ILS BPS
          Fit
     -->
    <xsl:variable name="nBPS" select="round($allele/BPS)"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$allele/Name"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$allele/RFU"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$nBPS"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$allele/BPS - $nBPS"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$allele/meanbps"/>
    <xsl:value-of select="$DELIM"/>
    <xsl:value-of select="$allele/Fit"/>
  </xsl:template>

  <xsl:template name="SampleName">
    <xsl:param name="sample"/>
    <xsl:choose>
      <xsl:when test="string-length($sample/SampleName)">
        <xsl:value-of select="$sample/SampleName"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$sample/Name"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="sampleLoop">
    <xsl:param name="table" select="/OsirisAnalysisReport/Table"/>
    <!-- param used in preparation for multiple files -->

    <xsl:for-each select="$table/Sample">
      <xsl:variable name="enabled">
        <xsl:call-template name="isSampleEnabled">
          <xsl:with-param name="sample" select="."/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:if test="$enabled != 'false'">
        <xsl:variable name="sampleName">
          <xsl:call-template name="SampleName">
            <xsl:with-param name="sample" select="."/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:for-each select="Locus">
          <xsl:value-of select="$sampleName"/>
          <xsl:value-of select="$DELIM"/>
          <xsl:value-of select="LocusName"/>
          <xsl:for-each select="Allele[not(Disabled = 'true')]">
            <xsl:call-template name="writeAllele">
              <xsl:with-param name="allele" select="."/>
            </xsl:call-template>
          </xsl:for-each>
          <xsl:value-of select="$EOL"/>
        </xsl:for-each>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="headerRows">
    <xsl:param name="nAlleles"/>
    <!-- row 1 -->
    <xsl:value-of select="$DELIM"/>
    <xsl:call-template name="AlleleColumns1">
      <xsl:with-param name="nAlleles" select="$nAlleles"/>
    </xsl:call-template>
    <xsl:value-of select="$EOL"/>

    <!-- row 2 -->
    <xsl:text>Sample Number</xsl:text>
    <xsl:value-of select="$DELIM"/>
    <xsl:text>Locus</xsl:text>

    <xsl:variable name="cols">
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Call</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>RFU</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>BPS</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Residual</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>ILS BPS</xsl:text>
      <xsl:value-of select="$DELIM"/>
      <xsl:text>Fit</xsl:text>
    </xsl:variable>

    <xsl:call-template name="repeat">
      <xsl:with-param name="str" select="$cols"/>
      <xsl:with-param name="count" select="$nAlleles"/>
    </xsl:call-template>
    <xsl:value-of select="$EOL"/>
    
  </xsl:template>

  <xsl:template match="/">
    <xsl:variable name="maxAlleleCount">
      <xsl:call-template name="maxAllele"/>
    </xsl:variable>

    <xsl:call-template name="headerRows">
      <xsl:with-param name="nAlleles" select="$maxAlleleCount"/>
    </xsl:call-template>
    
    <xsl:call-template name="sampleLoop"/>
  </xsl:template>


</xsl:stylesheet>
