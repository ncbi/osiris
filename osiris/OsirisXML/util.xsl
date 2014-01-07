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
#  FileName: util.xsl
#  Author:   Douglas Hoffman
#  Description:  General purpose utility functions and templates
#   to be imported into other xsl scripts using xsl:import
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:str="http://exslt.org/strings"
  xmlns:func="http://exslt.org/functions"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  xmlns:exsl="http://exslt.org/common"
  exclude-result-prefixes="str xsl func os exsl"
  extension-element-prefixes="str func os exsl"
  version="1.1">

  <xsl:output method="text"/>

  <xsl:variable name="UPPER" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
  <xsl:variable name="lower" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="allowDebugXML" select="0"/>

  <func:function name="os:InList">
    <xsl:param name="nodes"/>
    <xsl:param name="str"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="$nodes">
          <xsl:value-of select="count($nodes[ . = $str])"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($rtn))"/>
  </func:function>

  <func:function name="os:isNaN">
    <xsl:param name="n"/>
    <xsl:choose>
      <xsl:when test="string(number($n)) = 'NaN'">
        <func:result select="true()"/>
      </xsl:when>
      <xsl:otherwise>
        <func:result select="false()"/>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  <func:function name="os:Number">
    <xsl:param name="n"/>
    <xsl:param name="nDefault"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="os:isNaN($n)">
          <xsl:value-of select="$nDefault"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="number($n)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:Number0">
    <xsl:param name="n"/>
    <xsl:variable name="rtn" select="os:Number($n,0)"/>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:min2">
    <xsl:param name="n1"/>
    <xsl:param name="n2"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="$n1 &lt; $n2">
          <xsl:value-of select="$n1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$n2"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:max2">
    <xsl:param name="n1"/>
    <xsl:param name="n2"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="$n1 &lt; $n2">
          <xsl:value-of select="$n2"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$n1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <func:function name="os:str2bool">
    <xsl:param name="x"/>
    <xsl:variable name="s" select="translate(string($x),$UPPER,$lower)"/>
    <xsl:variable name="n">
      <xsl:choose>
        <xsl:when test="$s = ''">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="not(os:isNaN($s))">
          <xsl:value-of select="$s"/>
        </xsl:when>
        <xsl:when test="($s = 'f') or ($s = 'false')">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="boolean(number($n))"/>
  </func:function>


  <func:function name="os:minmax2">
    <xsl:param name="n1"/>
    <xsl:param name="n2"/>
    <xsl:param name="usemin"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="os:str2bool($usemin)">
          <xsl:value-of select="os:min2($n1,$n2)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="os:max2($n1,$n2)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <xsl:template name="str-replace">
    <xsl:param name="str"/>
    <xsl:param name="kill"/>
    <xsl:param name="replace"/>
    <xsl:choose>
      <xsl:when test="string-length($kill) = 0">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="$kill = $replace">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:when test="not(contains($str,$kill))">
        <xsl:value-of select="$str"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="substring-before($str,$kill)"/>
        <xsl:value-of select="$replace"/>
        <xsl:call-template name="str-replace">
          <xsl:with-param name="str" select="substring-after($str,$kill)"/>
          <xsl:with-param name="kill" select="$kill"/>
          <xsl:with-param name="replace" select="$replace"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <func:function name="os:str-replace">
    <xsl:param name="str"/>
    <xsl:param name="kill"/>
    <xsl:param name="replace"/>
    <xsl:variable name="sRtn">
      <xsl:call-template name="str-replace">
        <xsl:with-param name="str" select="$str"/>
        <xsl:with-param name="kill" select="$kill"/>
        <xsl:with-param name="replace" select="$replace"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="$sRtn"/>
  </func:function>

  <func:function name="os:encode-file-uri">
    <xsl:param name="str"/>
    <xsl:variable name="q">
      <xsl:text>'</xsl:text>
    </xsl:variable>
    <xsl:variable name="qq">
      <xsl:text>"</xsl:text>
    </xsl:variable>
    <xsl:variable name="s1" 
      select="concat('file:///',str:encode-uri($str,'UTF-8'))"/>
    <xsl:variable name="s2"
      select="os:str-replace($s1,$q,'%27')"/>
    <xsl:variable name="sRtn"
      select="os:str-replace($s2,$qq,'%22')"/>
    <func:result select="$sRtn"/>
  </func:function>

  <xsl:template name="minmax">
    <xsl:param name="nodes"/>
    <xsl:param name="top" select="1"/>
    <xsl:param name="bottom" select="count($nodes)"/>
    <xsl:param name="useMin" select="1"/>
    <xsl:param name="current" select="''"/>
    <xsl:variable name="nCurrent">
      <xsl:choose>
        <xsl:when test="not(os:isNaN($current))">
          <xsl:value-of select="$current"/>
        </xsl:when>
        <xsl:when test="$bottom &gt; 0">
          <xsl:value-of select="$nodes[$bottom]"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="0"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$bottom &lt; $top">
        <xsl:value-of select="$nCurrent"/>
      </xsl:when>
      <xsl:when test="$bottom = $top">
        <xsl:variable name="n" select="os:Number0($nodes[$top])"/>
        <xsl:variable name="nRtn" select="os:minmax2($n,$nCurrent,$useMin)"/>
        <xsl:value-of select="$nRtn"/>
      </xsl:when>
      <xsl:when test="($bottom - $top) &lt; 11">
        <xsl:variable name="n1" select="os:Number0($nodes[$top])"/>
        <xsl:variable name="n2" select="os:Number0($nodes[$bottom])"/>
        <xsl:variable name="n12" select="os:minmax2($n1,$n2,$useMin)"/>
        <xsl:variable name="nNext" select="os:minmax2($n12,$nCurrent,$useMin)"/>
        <xsl:call-template name="minmax">
          <xsl:with-param name="nodes" select="$nodes"/>
          <xsl:with-param name="top" select="$top + 1"/>
          <xsl:with-param name="bottom" select="$bottom - 1"/>
          <xsl:with-param name="current" select="$nNext"/>
          <xsl:with-param name="useMin" select="$useMin"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="mid" select="floor(($top + $bottom) * 0.5)"/>
        <xsl:variable name="n1">
          <xsl:call-template name="minmax">
            <xsl:with-param name="nodes" select="$nodes"/>
            <xsl:with-param name="top" select="$top"/>
            <xsl:with-param name="bottom" select="$mid"/>
            <xsl:with-param name="current" select="$nCurrent"/>
            <xsl:with-param name="useMin" select="$useMin"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="n2">
          <xsl:call-template name="minmax">
            <xsl:with-param name="nodes" select="$nodes"/>
            <xsl:with-param name="top" select="$mid + 1"/>
            <xsl:with-param name="bottom" select="$bottom"/>
            <xsl:with-param name="current" select="$n1"/>
            <xsl:with-param name="useMin" select="$useMin"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:value-of select="$n2"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <func:function name="os:min">
    <xsl:param name="nodes"/>
    <xsl:variable name="nRtn">
      <xsl:if test="$nodes and count($nodes)">
        <xsl:call-template name="minmax">
          <xsl:with-param name="nodes" select="$nodes"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>
    <func:result select="os:Number0($nRtn)"/>
  </func:function>

  <func:function name="os:max">
    <xsl:param name="nodes"/>
    <xsl:variable name="nRtn">
      <xsl:if test="$nodes and count($nodes)">
        <xsl:call-template name="minmax">
          <xsl:with-param name="nodes" select="$nodes"/>
          <xsl:with-param name="useMin" select="0"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>
    <func:result select="os:Number0($nRtn)"/>
  </func:function>

  <!--           template / - used for testing -->


  <xsl:template name="split">
    <xsl:param name="str"/>
    <xsl:param name="separator"/>
    <xsl:choose>
      <xsl:when test="string-length($separator) and contains($str,$separator)">
        <x>
          <xsl:value-of select="substring-before($str,$separator)"/>
        </x>
        <xsl:call-template name="split">
          <xsl:with-param name="str" select="substring-after($str,$separator)"/>
          <xsl:with-param name="separator" select="$separator"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <x>
          <xsl:value-of select="$str"/>
        </x>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="join">
    <xsl:param name="nodes"/>
    <xsl:param name="delim" select="';'"/>
    <xsl:for-each select="$nodes">
      <xsl:if test="position() &gt; 1">
        <xsl:value-of select="$delim"/>
      </xsl:if>
      <xsl:value-of select="."/>
    </xsl:for-each>
  </xsl:template>

  <func:function name="os:Path">
    <xsl:param name="FileName"/>
    <xsl:variable name="back" select="contains($FileName,'\')"/>
    <xsl:variable name="FileNameSplit">
      <f>
        <xsl:choose>
          <xsl:when test="$back">
            <xsl:call-template name="split">
              <xsl:with-param name="str" select="os:str-replace($FileName,'/','\')"/>
              <xsl:with-param name="separator" select="'\'"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="split">
              <xsl:with-param name="str" select="$FileName"/>
              <xsl:with-param name="separator" select="'/'"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </f>
    </xsl:variable>
    <xsl:variable name="separator">
      <xsl:choose>
        <xsl:when test="$back">
          <xsl:text>\</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>/</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="xml" select="exsl:node-set($FileNameSplit)"/>
    <xsl:variable name="n" select="count($xml/f/x)"/>
    <xsl:variable name="rtn">
      <xsl:variable name="s">
        <xsl:call-template name="join">
          <xsl:with-param name="nodes"
            select="$xml/f/x[position() &lt; $n]"/>
          <xsl:with-param name="delim" select="$separator"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:if test="string-length($s)">
        <xsl:value-of select="$s"/>
        <xsl:value-of select="$separator"/>
      </xsl:if>
    </xsl:variable>
    <func:result select="$rtn"/>
  </func:function>

  <xsl:template name="utilFileToURL">
    <xsl:param name="str"/>
    <xsl:variable name="s1" select="os:str-replace($str,'\','/')"/>
    <xsl:if test="(substring(translate($s1,$UPPER,$lower),1,5) != 'file:')">
      <xsl:text>file:///</xsl:text>
    </xsl:if>
    <xsl:value-of select="$s1"/>
  </xsl:template>

  <xsl:template name="debugXML">
    <xsl:param name="file"/>
    <xsl:param name="xml"/>
    <xsl:if test="string-length($file) and $allowDebugXML">
      <xsl:variable name="furl">
        <xsl:call-template name="utilFileToURL">
          <xsl:with-param name="str" select="$file"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:document href="{$furl}" indent="yes" standalone="yes">
        <xsl:copy-of select="$xml"/>
      </xsl:document>
    </xsl:if>
  </xsl:template>

  <xsl:template name="TESTminmax">

    <!-- 1 to 35 in pseudo random order -->
    <xsl:variable name="tmpNumbers">
      <x>
        <n v="21"/>
        <n v="18"/>
        <n v="4"/>
        <n v="30"/>
        <n v="7"/>
        <n v="15"/>
        <n v="32"/>
        <n v="16"/>
        <n v="14"/>
        <n v="13"/>
        <n v="22"/>
        <n v="10"/>
        <n v="31"/>
        <n v="9"/>
        <n v="5"/>
        <n v="2"/>
        <n v="29"/>
        <n v="26"/>
        <n v="17"/>
        <n v="35"/>
        <n v="12"/>
        <n v="25"/>
        <n v="6"/>
        <n v="24"/>
        <n v="27"/>
        <n v="20"/>
        <n v="34"/>
        <n v="23"/>
        <n v="1"/>
        <n v="11"/>
        <n v="19"/>
        <n v="33"/>
        <n v="8"/>
        <n v="3"/>
        <n v="28"/>
      </x>
    </xsl:variable>
    <xsl:variable name="numbers" select="exsl:node-set($tmpNumbers)"/>
    <xsl:variable name="nodes" select="$numbers/x/*/@v"/>
    <xsl:variable name="nLow" select="os:min($nodes)"/>
    <xsl:variable name="nHi" select="os:max($nodes)"/>
    <xsl:variable name="self" select="'Doug Hoffman -  ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
    <xsl:variable name="selfLower" select="translate($self,$UPPER,$lower)"/>
    <xsl:variable name="selfLowerCut" select="os:str-replace(os:str-replace($selfLower,' - ',','),$lower,'hey')"/>
    <xsl:text>nLow: </xsl:text>
    <xsl:value-of select="$nLow"/>

    <xsl:text>&#10;nHi:  </xsl:text>
    <xsl:value-of select="$nHi"/>

    <xsl:text>&#10;os:InList($nodes,030): </xsl:text>
    <xsl:value-of select="os:InList($nodes,030)"/>

    <xsl:text>&#10;os:InList($nodes,'z'): </xsl:text>
    <xsl:value-of select="os:InList($nodes,'z')"/>

    <xsl:text>&#10;selfLower: </xsl:text>
    <xsl:value-of select="$selfLower"/>

    <xsl:text>&#10;selfLowerCut: </xsl:text>
    <xsl:value-of select="$selfLowerCut"/>

    <xsl:text>&#10;boolean(number('NaN')): </xsl:text>
    <xsl:value-of select="boolean(number('NaN'))"/>

    <xsl:text>&#10;os:isNaN('x'): </xsl:text>
    <xsl:value-of select="os:isNaN('x')"/>

    <xsl:text>&#10;os:isNaN(''): </xsl:text>
    <xsl:value-of select="os:isNaN('')"/>

    <xsl:text>&#10;os:isNaN('12'): </xsl:text>
    <xsl:value-of select="os:isNaN('12')"/>

    <xsl:text>&#10;os:str2bool('true'): </xsl:text>
    <xsl:value-of select="os:str2bool('true')"/>

    <xsl:text>&#10;os:str2bool('faLSE'): </xsl:text>
    <xsl:value-of select="os:str2bool('faLSE')"/>

    <xsl:text>&#10;os:str2bool(''): </xsl:text>
    <xsl:value-of select="os:str2bool('')"/>

    <xsl:text>&#10;os:str2bool('000'): </xsl:text>
    <xsl:value-of select="os:str2bool('000')"/>

    <xsl:text>&#10;os:str2bool('-1'): </xsl:text>
    <xsl:value-of select="os:str2bool('-1')"/>

    <xsl:text>&#10;</xsl:text>

  </xsl:template>
  <xsl:template match="/">
    <!-- used for test -->
    <xsl:call-template name="TESTminmax"/>
    <xsl:variable name="file1">
      <xsl:text>C:\Program Files\Program Name\program.exe</xsl:text>
    </xsl:variable>
    <xsl:variable name="file2">
      <xsl:text>/usr/local/bin/program</xsl:text>
    </xsl:variable>
    <xsl:variable name="file3">
      <xsl:text>\\server\top\second/third/fourth/file.txt</xsl:text>
      
    </xsl:variable>
    <xsl:variable name="path1" select="os:Path($file1)"/>
    <xsl:variable name="path2" select="os:Path($file2)"/>
    <xsl:variable name="path3" select="os:Path($file3)"/>
    <xsl:value-of select="concat($file1,' - ',$path1,'&#10;')"/>
    <xsl:value-of select="concat($file2,' - ',$path2,'&#10;')"/>
    <xsl:value-of select="concat($file3,' - ',$path3,'&#10;')"/>
  </xsl:template>


</xsl:stylesheet>
