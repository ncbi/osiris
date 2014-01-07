/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
*  FileName: nwxXmlCMFtest.cpp
*  Author:   Douglas Hoffman
*
*   methods in nwxXmlCMF class for unit testing
*   using example file from Appendix A in
*   CODIS Interface Specification (CMF 3.2) Revision 9
*   CODIS Operations and Maintenance Support Services
*   DOJ-FBI-2005-C-2431
*/
#ifdef _DEBUG

#include <wx/mstream.h>
#include <stdio.h>
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxString.h"

// xml generated from file with xml2cstr.pl
static const char sXML[] =
/*    1 */  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
/*    2 */  "<CODISImportFile xmlns=\"urn:CODISImportFile-schema\">\n"
/*    3 */  "  <HEADERVERSION>3.2</HEADERVERSION>\n"
/*    4 */  "  <MESSAGETYPE>Import</MESSAGETYPE>\n"
/*    5 */  "  <DESTINATIONORI>IADCI0000</DESTINATIONORI>\n"
/*    6 */  "  <SOURCELAB>IADCI0000</SOURCELAB>\n"
/*    7 */  "  <SUBMITBYUSERID>KEllis</SUBMITBYUSERID>\n"
/*    8 */  "  <SUBMITDATETIME>2002-02-14T21:51:44</SUBMITDATETIME>\n"
/*    9 */  "  <BATCHID>GEL2004_10_04_101</BATCHID>\n"
/*   10 */  "  <KIT>PowerPlex 16</KIT>\n"
/*   11 */  "  <SPECIMEN SOURCEID=\"Yes\" CASEID=\"FL2004_10_04_ABC\" PARTIAL=\"true\">\n"
/*   12 */  "    <SPECIMENID>IMP_0001A</SPECIMENID>\n"
/*   13 */  "    <SPECIMENCATEGORY>Forensic, Unknown</SPECIMENCATEGORY>\n"
/*   14 */  "    <SPECIMENCOMMENT>Off-ladder allele value observed for FGA.</SPECIMENCOMMENT>\n"
/*   15 */  "    <LOCUS BATCHID=\"GEL2004_10_04_100\">\n"
/*   16 */  "      <LOCUSNAME>CSF1PO</LOCUSNAME>\n"
/*   17 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   18 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*   19 */  "      <ALLELE ALLELEREQUIRED=\"true\">\n"
/*   20 */  "        <ALLELEVALUE>10</ALLELEVALUE>\n"
/*   21 */  "      </ALLELE>\n"
/*   22 */  "      <ALLELE>\n"
/*   23 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*   24 */  "      </ALLELE>\n"
/*   25 */  "    </LOCUS>\n"
/*   26 */  "    <LOCUS BATCHID=\"GEL2004_10_05_67\" KIT=\"PowerPlex 1.2\">\n"
/*   27 */  "      <LOCUSNAME>D13S317</LOCUSNAME>\n"
/*   28 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   29 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*   30 */  "      <ALLELE>\n"
/*   31 */  "        <ALLELEVALUE>8</ALLELEVALUE>\n"
/*   32 */  "      </ALLELE>\n"
/*   33 */  "      <ALLELE ALLELEREQUIRED=\"true\">\n"
/*   34 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*   35 */  "      </ALLELE>\n"
/*   36 */  "    </LOCUS>\n"
/*   37 */  "    <LOCUS>\n"
/*   38 */  "      <LOCUSNAME>D16S539</LOCUSNAME>\n"
/*   39 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   40 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*   41 */  "      <ALLELE>\n"
/*   42 */  "        <ALLELEVALUE>6</ALLELEVALUE>\n"
/*   43 */  "      </ALLELE>\n"
/*   44 */  "      <ALLELE>\n"
/*   45 */  "        <ALLELEVALUE>7</ALLELEVALUE>\n"
/*   46 */  "      </ALLELE>\n"
/*   47 */  "    </LOCUS>\n"
/*   48 */  "    <LOCUS>\n"
/*   49 */  "      <LOCUSNAME>D18S51</LOCUSNAME>\n"
/*   50 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   51 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*   52 */  "      <ALLELE>\n"
/*   53 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*   54 */  "      </ALLELE>\n"
/*   55 */  "      <ALLELE>\n"
/*   56 */  "        <ALLELEVALUE>12</ALLELEVALUE>\n"
/*   57 */  "      </ALLELE>\n"
/*   58 */  "    </LOCUS>\n"
/*   59 */  "    <LOCUS>\n"
/*   60 */  "      <LOCUSNAME>D21S11</LOCUSNAME>\n"
/*   61 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   62 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*   63 */  "      <ALLELE>\n"
/*   64 */  "        <ALLELEVALUE>28</ALLELEVALUE>\n"
/*   65 */  "      </ALLELE>\n"
/*   66 */  "      <ALLELE ALLELEREQUIRED=\"true\">\n"
/*   67 */  "        <ALLELEVALUE>29</ALLELEVALUE>\n"
/*   68 */  "      </ALLELE>\n"
/*   69 */  "    </LOCUS>\n"
/*   70 */  "    <LOCUS>\n"
/*   71 */  "      <LOCUSNAME>D3S1358</LOCUSNAME>\n"
/*   72 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   73 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*   74 */  "      <ALLELE>\n"
/*   75 */  "        <ALLELEVALUE>14</ALLELEVALUE>\n"
/*   76 */  "      </ALLELE>\n"
/*   77 */  "      <ALLELE>\n"
/*   78 */  "        <ALLELEVALUE>15</ALLELEVALUE>\n"
/*   79 */  "      </ALLELE>\n"
/*   80 */  "    </LOCUS>\n"
/*   81 */  "    <LOCUS>\n"
/*   82 */  "      <LOCUSNAME>D5S818</LOCUSNAME>\n"
/*   83 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   84 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*   85 */  "      <ALLELE>\n"
/*   86 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*   87 */  "      </ALLELE>\n"
/*   88 */  "      <ALLELE>\n"
/*   89 */  "        <ALLELEVALUE>10</ALLELEVALUE>\n"
/*   90 */  "      </ALLELE>\n"
/*   91 */  "    </LOCUS>\n"
/*   92 */  "    <LOCUS>\n"
/*   93 */  "      <LOCUSNAME>D7S820</LOCUSNAME>\n"
/*   94 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*   95 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*   96 */  "      <ALLELE>\n"
/*   97 */  "        <ALLELEVALUE>8</ALLELEVALUE>\n"
/*   98 */  "      </ALLELE>\n"
/*   99 */  "      <ALLELE>\n"
/*  100 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*  101 */  "      </ALLELE>\n"
/*  102 */  "    </LOCUS>\n"
/*  103 */  "    <LOCUS>\n"
/*  104 */  "      <LOCUSNAME>D8S1179</LOCUSNAME>\n"
/*  105 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  106 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  107 */  "      <ALLELE>\n"
/*  108 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*  109 */  "      </ALLELE>\n"
/*  110 */  "      <ALLELE>\n"
/*  111 */  "        <ALLELEVALUE>12</ALLELEVALUE>\n"
/*  112 */  "      </ALLELE>\n"
/*  113 */  "    </LOCUS>\n"
/*  114 */  "    <LOCUS>\n"
/*  115 */  "      <LOCUSNAME>FGA</LOCUSNAME>\n"
/*  116 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  117 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  118 */  "      <ALLELE>\n"
/*  119 */  "        <ALLELEVALUE>24</ALLELEVALUE>\n"
/*  120 */  "      </ALLELE>\n"
/*  121 */  "      <ALLELE>\n"
/*  122 */  "        <ALLELEVALUE>25</ALLELEVALUE>\n"
/*  123 */  "      </ALLELE>\n"
/*  124 */  "    </LOCUS>\n"
/*  125 */  "    <LOCUS>\n"
/*  126 */  "      <LOCUSNAME>TH01</LOCUSNAME>\n"
/*  127 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  128 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  129 */  "      <ALLELE>\n"
/*  130 */  "        <ALLELEVALUE>8</ALLELEVALUE>\n"
/*  131 */  "      </ALLELE>\n"
/*  132 */  "      <ALLELE>\n"
/*  133 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*  134 */  "      </ALLELE>\n"
/*  135 */  "    </LOCUS>\n"
/*  136 */  "    <LOCUS>\n"
/*  137 */  "      <LOCUSNAME>TPOX</LOCUSNAME>\n"
/*  138 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  139 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  140 */  "      <ALLELE>\n"
/*  141 */  "        <ALLELEVALUE>10</ALLELEVALUE>\n"
/*  142 */  "      </ALLELE>\n"
/*  143 */  "      <ALLELE>\n"
/*  144 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*  145 */  "      </ALLELE>\n"
/*  146 */  "    </LOCUS>\n"
/*  147 */  "    <LOCUS>\n"
/*  148 */  "      <LOCUSNAME>vWA</LOCUSNAME>\n"
/*  149 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  150 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  151 */  "      <ALLELE>\n"
/*  152 */  "        <ALLELEVALUE>15</ALLELEVALUE>\n"
/*  153 */  "      </ALLELE>\n"
/*  154 */  "      <ALLELE>\n"
/*  155 */  "        <ALLELEVALUE>16</ALLELEVALUE>\n"
/*  156 */  "      </ALLELE>\n"
/*  157 */  "    </LOCUS>\n"
/*  158 */  "  </SPECIMEN>\n"
/*  159 */  "  <SPECIMEN SOURCEID=\"No\" CASEID=\"FL2004_10_04_DEF\" PARTIAL=\"true\">\n"
/*  160 */  "    <SPECIMENID>IMP_0001B</SPECIMENID>\n"
/*  161 */  "    <SPECIMENCATEGORY>Forensic, Unknown</SPECIMENCATEGORY>\n"
/*  162 */  "    <SPECIMENCOMMENT>A possible peak was observed at CSF1PO that was not called due to minumum peak threshold.</SPECIMENCOMMENT>\n"
/*  163 */  "    <LOCUS BATCHID=\"GEL2004_10_05_68\" KIT=\"PowerPlex 1.1\">\n"
/*  164 */  "      <LOCUSNAME>CSF1PO</LOCUSNAME>\n"
/*  165 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  166 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*  167 */  "      <ALLELE ALLELEREQUIRED=\"true\">\n"
/*  168 */  "        <ALLELEVALUE>&lt;6</ALLELEVALUE>\n"
/*  169 */  "      </ALLELE>\n"
/*  170 */  "      <ALLELE>\n"
/*  171 */  "        <ALLELEVALUE>8.2</ALLELEVALUE>\n"
/*  172 */  "      </ALLELE>\n"
/*  173 */  "    </LOCUS>\n"
/*  174 */  "    <LOCUS>\n"
/*  175 */  "      <LOCUSNAME>D13S317</LOCUSNAME>\n"
/*  176 */  "      <READINGBY>BKNOLL</READINGBY>\n"
/*  177 */  "      <READINGDATETIME>2001-03-02T11:50:42</READINGDATETIME>\n"
/*  178 */  "      <ALLELE>\n"
/*  179 */  "        <ALLELEVALUE>13</ALLELEVALUE>\n"
/*  180 */  "      </ALLELE>\n"
/*  181 */  "      <ALLELE>\n"
/*  182 */  "        <ALLELEVALUE>&gt;15</ALLELEVALUE>\n"
/*  183 */  "      </ALLELE>\n"
/*  184 */  "    </LOCUS>\n"
/*  185 */  "    <LOCUS>\n"
/*  186 */  "      <LOCUSNAME>D16S539</LOCUSNAME>\n"
/*  187 */  "      <READINGBY>CLEE</READINGBY>\n"
/*  188 */  "      <READINGDATETIME>2001-01-01T02:50:42</READINGDATETIME>\n"
/*  189 */  "      <ALLELE>\n"
/*  190 */  "        <ALLELEVALUE>6</ALLELEVALUE>\n"
/*  191 */  "      </ALLELE>\n"
/*  192 */  "      <ALLELE>\n"
/*  193 */  "        <ALLELEVALUE>7</ALLELEVALUE>\n"
/*  194 */  "      </ALLELE>\n"
/*  195 */  "      <ALLELE>\n"
/*  196 */  "        <ALLELEVALUE>13.1</ALLELEVALUE>\n"
/*  197 */  "      </ALLELE>\n"
/*  198 */  "      <ALLELE>\n"
/*  199 */  "        <ALLELEVALUE>14.2</ALLELEVALUE>\n"
/*  200 */  "      </ALLELE>\n"
/*  201 */  "    </LOCUS>\n"
/*  202 */  "    <LOCUS>\n"
/*  203 */  "      <LOCUSNAME>D18S51</LOCUSNAME>\n"
/*  204 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  205 */  "      <READINGDATETIME>2002-02-13T21:50:42</READINGDATETIME>\n"
/*  206 */  "      <ALLELE>\n"
/*  207 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*  208 */  "      </ALLELE>\n"
/*  209 */  "    </LOCUS>\n"
/*  210 */  "    <LOCUS>\n"
/*  211 */  "      <LOCUSNAME>D21S11</LOCUSNAME>\n"
/*  212 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  213 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  214 */  "      <ALLELE>\n"
/*  215 */  "        <ALLELEVALUE>&lt;25</ALLELEVALUE>\n"
/*  216 */  "      </ALLELE>\n"
/*  217 */  "      <ALLELE>\n"
/*  218 */  "        <ALLELEVALUE>28</ALLELEVALUE>\n"
/*  219 */  "      </ALLELE>\n"
/*  220 */  "      <ALLELE>\n"
/*  221 */  "        <ALLELEVALUE>29</ALLELEVALUE>\n"
/*  222 */  "      </ALLELE>\n"
/*  223 */  "    </LOCUS>\n"
/*  224 */  "    <LOCUS>\n"
/*  225 */  "      <LOCUSNAME>D3S1358</LOCUSNAME>\n"
/*  226 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  227 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  228 */  "      <ALLELE>\n"
/*  229 */  "        <ALLELEVALUE>14</ALLELEVALUE>\n"
/*  230 */  "      </ALLELE>\n"
/*  231 */  "      <ALLELE>\n"
/*  232 */  "        <ALLELEVALUE>15</ALLELEVALUE>\n"
/*  233 */  "      </ALLELE>\n"
/*  234 */  "    </LOCUS>\n"
/*  235 */  "    <LOCUS>\n"
/*  236 */  "      <LOCUSNAME>D5S818</LOCUSNAME>\n"
/*  237 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  238 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  239 */  "      <ALLELE>\n"
/*  240 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*  241 */  "      </ALLELE>\n"
/*  242 */  "      <ALLELE>\n"
/*  243 */  "        <ALLELEVALUE>10</ALLELEVALUE>\n"
/*  244 */  "      </ALLELE>\n"
/*  245 */  "    </LOCUS>\n"
/*  246 */  "    <LOCUS>\n"
/*  247 */  "      <LOCUSNAME>D7S820</LOCUSNAME>\n"
/*  248 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  249 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  250 */  "      <ALLELE>\n"
/*  251 */  "        <ALLELEVALUE>8</ALLELEVALUE>\n"
/*  252 */  "      </ALLELE>\n"
/*  253 */  "      <ALLELE>\n"
/*  254 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*  255 */  "      </ALLELE>\n"
/*  256 */  "    </LOCUS>\n"
/*  257 */  "    <LOCUS>\n"
/*  258 */  "      <LOCUSNAME>D8S1179</LOCUSNAME>\n"
/*  259 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  260 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  261 */  "      <ALLELE>\n"
/*  262 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*  263 */  "      </ALLELE>\n"
/*  264 */  "      <ALLELE>\n"
/*  265 */  "        <ALLELEVALUE>12</ALLELEVALUE>\n"
/*  266 */  "      </ALLELE>\n"
/*  267 */  "    </LOCUS>\n"
/*  268 */  "    <LOCUS BATCHID=\"GEL2004_10_05_69\" KIT=\"PowerPlex 1.2\">\n"
/*  269 */  "      <LOCUSNAME>FGA</LOCUSNAME>\n"
/*  270 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  271 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  272 */  "      <ALLELE>\n"
/*  273 */  "        <ALLELEVALUE>24</ALLELEVALUE>\n"
/*  274 */  "      </ALLELE>\n"
/*  275 */  "      <ALLELE>\n"
/*  276 */  "        <ALLELEVALUE>25</ALLELEVALUE>\n"
/*  277 */  "      </ALLELE>\n"
/*  278 */  "    </LOCUS>\n"
/*  279 */  "    <LOCUS>\n"
/*  280 */  "      <LOCUSNAME>TH01</LOCUSNAME>\n"
/*  281 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  282 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  283 */  "      <ALLELE>\n"
/*  284 */  "        <ALLELEVALUE>8</ALLELEVALUE>\n"
/*  285 */  "      </ALLELE>\n"
/*  286 */  "      <ALLELE>\n"
/*  287 */  "        <ALLELEVALUE>9</ALLELEVALUE>\n"
/*  288 */  "      </ALLELE>\n"
/*  289 */  "    </LOCUS>\n"
/*  290 */  "    <LOCUS>\n"
/*  291 */  "      <LOCUSNAME>TPOX</LOCUSNAME>\n"
/*  292 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  293 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  294 */  "      <ALLELE>\n"
/*  295 */  "        <ALLELEVALUE>10</ALLELEVALUE>\n"
/*  296 */  "      </ALLELE>\n"
/*  297 */  "      <ALLELE>\n"
/*  298 */  "        <ALLELEVALUE>11</ALLELEVALUE>\n"
/*  299 */  "      </ALLELE>\n"
/*  300 */  "    </LOCUS>\n"
/*  301 */  "    <LOCUS>\n"
/*  302 */  "      <LOCUSNAME>vWA</LOCUSNAME>\n"
/*  303 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  304 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  305 */  "      <ALLELE>\n"
/*  306 */  "        <ALLELEVALUE>15</ALLELEVALUE>\n"
/*  307 */  "      </ALLELE>\n"
/*  308 */  "      <ALLELE>\n"
/*  309 */  "        <ALLELEVALUE>16</ALLELEVALUE>\n"
/*  310 */  "      </ALLELE>\n"
/*  311 */  "    </LOCUS>\n"
/*  312 */  "    <LOCUS>\n"
/*  313 */  "      <LOCUSNAME>Amelogenin</LOCUSNAME>\n"
/*  314 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  315 */  "      <READINGDATETIME>2002-02-13T21:50:47</READINGDATETIME>\n"
/*  316 */  "      <ALLELE>\n"
/*  317 */  "        <ALLELEVALUE>X</ALLELEVALUE>\n"
/*  318 */  "      </ALLELE>\n"
/*  319 */  "      <ALLELE>\n"
/*  320 */  "        <ALLELEVALUE>Y</ALLELEVALUE>\n"
/*  321 */  "      </ALLELE>\n"
/*  322 */  "    </LOCUS>\n"
/*  323 */  "    <LOCUS>\n"
/*  324 */  "      <LOCUSNAME>Penta D</LOCUSNAME>\n"
/*  325 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  326 */  "      <READINGDATETIME>2002-02-13T09:51:48</READINGDATETIME>\n"
/*  327 */  "      <ALLELE>\n"
/*  328 */  "        <ALLELEVALUE>7.3</ALLELEVALUE>\n"
/*  329 */  "      </ALLELE>\n"
/*  330 */  "      <ALLELE>\n"
/*  331 */  "        <ALLELEVALUE>&gt;17</ALLELEVALUE>\n"
/*  332 */  "      </ALLELE>\n"
/*  333 */  "    </LOCUS>\n"
/*  334 */  "    <LOCUS>\n"
/*  335 */  "      <LOCUSNAME>Penta E</LOCUSNAME>\n"
/*  336 */  "      <READINGBY>KELLIS</READINGBY>\n"
/*  337 */  "      <READINGDATETIME>2002-02-13T00:00:00</READINGDATETIME>\n"
/*  338 */  "      <ALLELE>\n"
/*  339 */  "        <ALLELEVALUE>15</ALLELEVALUE>\n"
/*  340 */  "      </ALLELE>\n"
/*  341 */  "      <ALLELE>\n"
/*  342 */  "        <ALLELEVALUE>16</ALLELEVALUE>\n"
/*  343 */  "      </ALLELE>\n"
/*  344 */  "    </LOCUS>\n"
/*  345 */  "  </SPECIMEN>\n"
/*  346 */  "</CODISImportFile>\n"
;
static const size_t XML_SIZE = (sizeof(sXML) / sizeof(sXML[0])) - 1;

static int nTestCount = 0;

static void MY_ASSERT(
  bool b, 
  const char *ps, 
  const wxChar *ps2 = _T(""), 
  const wxChar *ps3 = _T(""))
{
  if(!b)
  {
    wxString s;
    s.Printf(_T("nwxXmlCMF::UnitTest()[%d] "),nTestCount);
    s.Append(_T(ps));
    s.Append(ps2);
    s.Append(ps3);
    wxASSERT_MSG(0,s);
  }
}

/*
static bool DumpFile()
{
  FILE *pF = fopen("foo.xml","wb");
  size_t nToWrite = (int)XML_SIZE;
  size_t n;
  const char *pBuffer = sXML;
  bool bRtn = true;
  if(pF == NULL)
  {
    MY_ASSERT(0,"DumpFile() fopen() failed");
    nToWrite = 0; // prevent loop below
    bRtn = false;
  }
  while(nToWrite)
  {
    n = fwrite(pBuffer,sizeof(char),nToWrite,pF);
    if(!n)
    {
      MY_ASSERT(0,"DumpFile() write failed");
      nToWrite = 0; // loop exit
      bRtn = false;
    }
    else
    {
      nToWrite -= n;
      pBuffer += n;
    }
  }
  if(pF != NULL)
  {
    fclose(pF);
  }
  return bRtn;
}
*/

void nwxXmlCMF::UnitTest()
{
  nwxXmlCMF x;
  nwxXmlCMF xx;
  wxString sFileName(_T("foo.xml"));
  wxMemoryInputStream instr(sXML,XML_SIZE);
  MY_ASSERT(instr.IsOk(),"wxMemoryInputStream::IsOk() failed");
  MY_ASSERT(instr.CanRead(),"wxMemoryInputStream::CanRead() failed");
  MY_ASSERT(instr.TellI() == (off_t)0,"wxMemoryInputStream::TellI() != 0");
  if(!x.LoadStream(instr))
  {
    MY_ASSERT(0,"Could not load input stream");
    return;
  }
  UnitTest1(x);
  if(!x.SaveFile(sFileName))
  {
    MY_ASSERT(0,"Cannot save file ", sFileName);
  }
  else if(!xx.LoadFile(sFileName))
  {
    MY_ASSERT(0,"Cannot load file ", sFileName);
  }
  else
  {
    UnitTest1(xx);
  }
}
void nwxXmlCMF::UnitTest1(const nwxXmlCMF &x)
{
  wxString s;
  size_t nCount;

  nTestCount++;

  MY_ASSERT(x.GetHeaderVersion() == 3.2," header version");
  MY_ASSERT(x.GetMessageType() == _T("Import"), " message type ");
  MY_ASSERT(x.GetDestination() == _T("IADCI0000"), "destination");
  MY_ASSERT(x.GetSourceLab() == _T("IADCI0000"), "source lab");
  MY_ASSERT(x.GetSubmitByUserID() == _T("KEllis"),"submit user id");
  s = nwxString::FormatDateTimeXML(x.GetSubmitDateTime());
  MY_ASSERT(s == _T("2002-02-14T21:51:44"),"submit date time: ", s.c_str());
  MY_ASSERT(x.GetBatchId() == _T("GEL2004_10_04_101"),"batch id");
  MY_ASSERT(x.GetKit() == _T("PowerPlex 16"),"kit");
  nCount = x.GetSpecimenCount();
  if(nCount != 2)
  {
    s = nwxString::FormatNumber((int)nCount);
    MY_ASSERT(0,"specimen count != 2 -- value is ",s);
    return;
  }
  const nwxXmlCMFSpecimen &spec1(x.GetSpecimen(0));
  const nwxXmlCMFSpecimen &spec2(x.GetSpecimen(1));

  // first specimen

#define UTS "SPECIMEN[1] "
  MY_ASSERT(spec1.GetSourceID() == _T("Yes"),UTS "source ID ", spec1.GetSourceID());
  MY_ASSERT(spec1.GetCaseID() == _T("FL2004_10_04_ABC"), UTS "case id ",spec1.GetCaseID());
  MY_ASSERT(spec1.GetPartial(), UTS "GetPartial() is false");
  MY_ASSERT(spec1.GetSpecimenID() == _T("IMP_0001A"),UTS "Specimen ID ",spec1.GetSpecimenID());
  MY_ASSERT(spec1.GetSpecimenCategory() == _T("Forensic, Unknown"),UTS "Specimen category: ",spec1.GetSpecimenCategory());
  MY_ASSERT(spec1.GetSpecimenComment() == _T("Off-ladder allele value observed for FGA."),UTS "Specimen comment: ",spec1.GetSpecimenComment());
  nCount = spec1.GetLocusCount();
  if(nCount != 13)
  {
    s = nwxString::FormatNumber((int)nCount);
    MY_ASSERT(0, UTS "Locus Count:",s);
  }
  if(nCount >= 10)
  {
    const nwxXmlCMFLocus &locus2 = spec1.GetLocus(1);
    const nwxXmlCMFLocus &locus10 = spec1.GetLocus(9);
    size_t nAllele2 = locus2.GetAlleleCount();
    size_t nAllele10 = locus10.GetAlleleCount();
    if(nAllele2 != 2)
    {
      MY_ASSERT(0,
        UTS "Locus2 allele count ", nwxString::FormatNumber((int)nAllele2) );
    }
    if(nAllele10 != 2)
    {
      MY_ASSERT(0,
        UTS "Locus10 allele count ", nwxString::FormatNumber((int)nAllele10) );
    }

    MY_ASSERT(locus2.GetBatchID() == _T("GEL2004_10_05_67"),
      UTS "Locus2 batchID ", locus2.GetBatchID());
    MY_ASSERT(locus2.GetKit() == _T("PowerPlex 1.2"),
      UTS "Locus2 kit ", locus2.GetKit());
    MY_ASSERT(locus2.GetLocusName() == _T("D13S317"),
      UTS "Locus2 name ", locus2.GetLocusName());
    MY_ASSERT(locus2.GetReadingBy() == _T("KELLIS"),
      UTS "Locus2 Reading By ",locus2.GetReadingBy());
    s = nwxString::FormatDateTimeXML(locus2.GetReadingDateTime());
    MY_ASSERT(s == _T("2002-02-13T21:50:42"), 
      UTS "Locus2 reading date/time ",s);
    MY_ASSERT(locus10.GetLocusName() == _T("FGA"),
      UTS "Locus10 name", locus10.GetLocusName());
    s = nwxString::FormatDateTimeXML(locus10.GetReadingDateTime());
    MY_ASSERT(s == _T("2002-02-13T21:50:47"),
      UTS "Locus10 reading date/time ",s);
    if(nAllele2 >= 2)
    {
      const nwxXmlCMFAllele &allele1(locus2.GetAllele(0));
      const nwxXmlCMFAllele &allele2(locus2.GetAllele(1));
      MY_ASSERT(allele2.IsRequired(),
        UTS " Locus2 Allele2 IsRequired() == false");
      s = allele1.GetAlleleValue();
      MY_ASSERT(s == _T("8"), UTS "Locus2 Allele1 value ",s);
      s = allele2.GetAlleleValue();
      MY_ASSERT(s == _T("9"), UTS "Locus2 Allele2 value ",s);
    }
    if(nAllele10 >= 2)
    {
      const nwxXmlCMFAllele &allele1(locus10.GetAllele(0));
      const nwxXmlCMFAllele &allele2(locus10.GetAllele(1));
      s = allele1.GetAlleleValue();
      MY_ASSERT(s == _T("24"), UTS "Locus10 Allele1 value ",s);
      s = allele2.GetAlleleValue();
      MY_ASSERT(s == _T("25"), UTS "Locus10 Allele2 value ",s);
    }
  }
  // end of first specimen
  // begin second specimen
#undef UTS
#define UTS "SPECIMEN[2] "

  MY_ASSERT(spec2.GetSourceID() == _T("No"),UTS "source ID ", spec2.GetSourceID());
  MY_ASSERT(spec2.GetCaseID() == _T("FL2004_10_04_DEF"), UTS "case id ",spec2.GetCaseID());
  MY_ASSERT(spec2.GetPartial(), UTS "GetPartial() is false");
  MY_ASSERT(spec2.GetSpecimenID() == _T("IMP_0001B"),UTS "Specimen ID ",spec2.GetSpecimenID());
  MY_ASSERT(spec2.GetSpecimenCategory() == _T("Forensic, Unknown"),UTS "Specimen category: ",spec2.GetSpecimenCategory());
  MY_ASSERT(spec2.GetSpecimenComment() == 
    _T("A possible peak was observed at CSF1PO that "
    "was not called due to minumum peak threshold."),
      UTS "Specimen comment: ",spec2.GetSpecimenComment());
  nCount = spec2.GetLocusCount();
  if(nCount != 16)
  {
    s = nwxString::FormatNumber((int)nCount);
    MY_ASSERT(0, UTS "Locus Count:",s);
  }
  if(nCount >= 10)
  {
    const nwxXmlCMFLocus &locus3 = spec2.GetLocus(2);
    const nwxXmlCMFLocus &locus10 = spec2.GetLocus(9);
    size_t nAllele3 = locus3.GetAlleleCount();
    size_t nAllele10 = locus10.GetAlleleCount();
    if(nAllele3 != 4)
    {
      MY_ASSERT(0,
        UTS "Locus2 allele count ", nwxString::FormatNumber((int)nAllele3) );
    }
    if(nAllele10 != 2)
    {
      MY_ASSERT(0,
        UTS "Locus10 allele count ", nwxString::FormatNumber((int)nAllele10) );
    }
    MY_ASSERT(locus10.GetBatchID() == _T("GEL2004_10_05_69"),
      UTS "Locus10 batchID ", locus10.GetBatchID());
    MY_ASSERT(locus10.GetKit() == _T("PowerPlex 1.2"),
      UTS "Locus10 kit ", locus10.GetKit());

    MY_ASSERT(locus3.GetLocusName() == _T("D16S539"),
      UTS "Locus3 name ", locus3.GetLocusName());
    MY_ASSERT(locus3.GetReadingBy() == _T("CLEE"),
      UTS "Locus3 Reading By ",locus3.GetReadingBy());
    s = nwxString::FormatDateTimeXML(locus3.GetReadingDateTime());
    MY_ASSERT(s == _T("2001-01-01T02:50:42"), 
      UTS "Locus3 reading date/time ",s);

    MY_ASSERT(locus10.GetLocusName() == _T("FGA"),
      UTS "Locus10 name", locus10.GetLocusName());
    MY_ASSERT(locus10.GetReadingBy() == _T("KELLIS"),
      UTS "Locus3 Reading By ",locus3.GetReadingBy());
    s = nwxString::FormatDateTimeXML(locus10.GetReadingDateTime());
    MY_ASSERT(s == _T("2002-02-13T21:50:47"),
      UTS "Locus10 reading date/time ",s);

    if(nAllele3 >= 4)
    {

      const nwxXmlCMFAllele &allele1(locus3.GetAllele(0));
      const nwxXmlCMFAllele &allele2(locus3.GetAllele(1));
      const nwxXmlCMFAllele &allele3(locus3.GetAllele(2));
      const nwxXmlCMFAllele &allele4(locus3.GetAllele(3));
      s = allele1.GetAlleleValue();
      MY_ASSERT(s == _T("6"),UTS "Locus3 allele1 ",s);
      s = allele2.GetAlleleValue();
      MY_ASSERT(s == _T("7"),UTS "Locus3 allele2 ",s);
      s = allele3.GetAlleleValue();
      MY_ASSERT(s == _T("13.1"),UTS "Locus3 allele3 ",s);
      s = allele4.GetAlleleValue();
      MY_ASSERT(s == _T("14.2"),UTS "Locus3 allele4 ",s);
    }
    if(nAllele10 >= 2)
    {
      const nwxXmlCMFAllele &allele1(locus10.GetAllele(0));
      const nwxXmlCMFAllele &allele2(locus10.GetAllele(1));
      s = allele1.GetAlleleValue();
      MY_ASSERT(s == _T("24"),UTS "Locus10 allele1 ",s);
      s = allele2.GetAlleleValue();
      MY_ASSERT(s == _T("25"),UTS "Locus10 allele2 ",s);
    }
  }  
}


#endif

