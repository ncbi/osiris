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
#  FileName: tabBU.xsl
#  Author:   Douglas Hoffman
#  Description:  Export allele info from OSIRIS analysis file to a
#   tab-delimited text file
#
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:func="http://exslt.org/functions"
  xmlns:exsl="http://exslt.org/common"
  xmlns:os="http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/"
  extension-element-prefixes="func exsl os"
  version="1.0">

  <xsl:output method="text"/>

  <xsl:variable name="TAB" select="'&#9;'"/>
  <xsl:variable name="EOL" select="'&#10;'"/>
  <xsl:variable name="header">
    <xsl:text>Sample File</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Run Name</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Marker</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Dye</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 1</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 1</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 1</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 2</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 2</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 2</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 3</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 3</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 3</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 4</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 4</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 4</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 5</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 5</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 5</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 6</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 6</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 6</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 7</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 7</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 7</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 8</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 8</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 8</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 9</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 9</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 9</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 10</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 10</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 10</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 11</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 11</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 11</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 12</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 12</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 12</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 13</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 13</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 13</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 14</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 14</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 14</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 15</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 15</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 15</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 16</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 16</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 16</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 17</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 17</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 17</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 18</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 18</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 18</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 19</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 19</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 19</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 20</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 20</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 20</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 21</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 21</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 21</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 22</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 22</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 22</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 23</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 23</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 23</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 24</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 24</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 24</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 25</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 25</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 25</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 26</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 26</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 26</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 27</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 27</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 27</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 28</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 28</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 28</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 29</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 29</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 29</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 30</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 30</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 30</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 31</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 31</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 31</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 32</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 32</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 32</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 33</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 33</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 33</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 34</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 34</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 34</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 35</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 35</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 35</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 36</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 36</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 36</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 37</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 37</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 37</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 38</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 38</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 38</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 39</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 39</xsl:text>
    <xsl:value-of select="$TAB"/>
   <xsl:text>Height 39</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 40</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 40</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 40</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 41</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 41</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 41</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 42</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 42</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 42</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 43</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 43</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 43</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 44</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 44</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 44</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 45</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 45</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 45</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 46</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 46</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 46</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 47</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 47</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 47</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 48</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 48</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 48</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 49</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 49</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 49</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 50</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 50</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 50</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 51</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 51</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 51</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 52</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 52</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 52</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 53</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 53</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 53</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 54</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 54</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 54</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 55</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 55</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 55</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 56</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 56</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 56</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 57</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 57</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 57</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 58</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 58</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 58</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 59</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 59</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 59</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 60</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 60</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 60</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 61</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 61</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 61</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 62</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 62</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 62</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 63</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 63</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 63</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 64</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 64</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 64</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 65</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 65</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 65</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 66</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 66</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 66</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 67</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 67</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 67</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 68</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 68</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 68</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 69</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 69</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 69</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 70</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 70</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 70</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 71</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 71</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 71</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 72</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 72</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 72</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 73</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 73</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 73</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 74</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 74</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 74</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 75</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 75</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 75</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 76</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 76</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 76</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 77</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 77</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 77</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 78</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 78</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 78</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 79</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 79</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 79</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 80</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 80</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 80</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 81</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 81</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 81</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 82</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 82</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 82</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 83</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 83</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 83</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 84</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 84</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 84</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 85</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 85</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 85</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 86</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 86</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 86</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 87</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 87</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 87</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 88</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 88</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 88</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 89</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 89</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 89</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 90</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 90</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 90</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 91</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 91</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 91</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 92</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 92</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 92</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 93</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 93</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 93</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 94</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 94</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 94</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 95</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 95</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 95</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 96</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 96</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 96</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 97</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 97</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 97</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 98</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 98</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 98</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 99</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 99</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 99</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 100</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 100</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 100</xsl:text>
    <xsl:value-of select="$EOL"/>
  </xsl:variable>

  <xsl:variable name="tmpKitColor">
    <kits>
      <kit name="Cofiler">
        <channel nr="1" dye="5-FAM" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="ROX" color="Red"/>
      </kit>
      <kit name="ProfilerPlus">
        <channel nr="1" dye="5-FAM" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="ROX" color="Red"/>
      </kit>
      <kit name="Identifiler">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="IdentifilerPlus">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="TAZ" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex 16">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR" color="Yellow"/>
        <channel nr="4" dye="CXR" color="Red"/>
      </kit>
      <kit name="Identifiler Less ILS250">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="IdentifilerPlus Less ILS250">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex 1.2">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="TMR" color="Yellow"/>
        <channel nr="3" dye="CXR" color="Red"/>
      </kit>
      <kit name="Yfiler">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex Y">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR" color="Yellow"/>
        <channel nr="4" dye="CXR" color="Red"/>
      </kit>
      <kit name="SGM Plus">
        <channel nr="1" dye="5-FAM" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="ROX" color="Red"/>
      </kit>
      <kit name="NGM">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex 18D">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="SEfilerPlus">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="NGMSElect">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex 21">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="GlobalFiler">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="SID" color="Purple"/>
        <channel nr="6" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex Fusion">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="PowerPlex Y23">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="PowerPlex ESX17">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="PowerPlex ESX16">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="PowerPlex ESI17">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="PowerPlex ESI16">
        <channel nr="1" dye="FL" color="Blue"/>
        <channel nr="2" dye="JOE" color="Green"/>
        <channel nr="3" dye="TMR-ET" color="Yellow"/>
        <channel nr="4" dye="CXR-ET" color="Red"/>
        <channel nr="5" dye="CC5" color="Orange"/>
      </kit>
      <kit name="iPlex">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="iPlexLIZ500less250">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="PowerPlex Fusion 6C">
        <channel nr="1" dye="FL-6C" color="Blue"/>
        <channel nr="2" dye="JOE-6C" color="Green"/>
        <channel nr="3" dye="TMR-6C" color="Yellow"/>
        <channel nr="4" dye="CXR-6C" color="Red"/>
        <channel nr="5" dye="TOM-6C" color="Purple"/>
        <channel nr="6" dye="WEN-6C" color="Orange"/>
      </kit>
      <kit name="Minifiler">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="VIC" color="Green"/>
        <channel nr="3" dye="NED" color="Yellow"/>
        <channel nr="4" dye="PET" color="Red"/>
        <channel nr="5" dye="LIZ" color="Orange"/>
      </kit>
      <kit name="NIDDKFragileX1">
        <channel nr="1" dye="6-FAM" color="Blue"/>
        <channel nr="2" dye="LIZ" color="Orange"/>
      </kit>
    </kits>
  </xsl:variable>

  <!-- AFTER THE MESS -->

  <xsl:variable name="kitName" select="/OsirisAnalysisReport/Heading/LabSettings/LabMarkerSetSpecifications//MarkerSetName"/>
  <xsl:variable name="kitColors" select="exsl:node-set($tmpKitColor)/kits/kit[@name = $kitName]"/>
  <xsl:variable name="tmpMarkerChannelDye">
    <x>
      <xsl:for-each select="/OsirisAnalysisReport/Heading/Channel">
        <xsl:variable name="nr" select="number(ChannelNr)"/>
        <xsl:variable name="color" select="$kitColors/channel[@nr = $nr]/@color"/>
        <xsl:variable name="dye" select="substring($color,1,1)"/>
        <xsl:for-each select="LocusName">
          <marker name="{.}" channel="{$nr}" dye="{$dye}"/>
        </xsl:for-each>
      </xsl:for-each>
    </x>
  </xsl:variable>
  <xsl:variable name="MarkerChannelDye" select="exsl:node-set($tmpMarkerChannelDye)"/>
  
  <xsl:template name="runName">
    <xsl:param name="s" select="/OsirisAnalysisReport/Heading/CommandLine/argv[substring(.,1,15) = 'InputDirectory=']"/>
    <xsl:variable name="nLen" select="string-length($s)"/>
    <xsl:choose>
      <xsl:when test="$nLen = 0"/>
      <xsl:when test="contains(';/\',substring($s,$nLen,1))">
        <xsl:call-template name="runName">
          <xsl:with-param name="s" select="substring($s,1,$nLen - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($s,'=')">
        <xsl:call-template name="runName">
          <xsl:with-param name="s" select="substring-after($s,'=')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($s,'/')">
        <xsl:call-template name="runName">
          <xsl:with-param name="s" select="substring-after($s,'/')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($s,'\')">
        <xsl:call-template name="runName">
          <xsl:with-param name="s" select="substring-after($s,'\')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$s"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:variable name="RunName">
    <xsl:call-template name="runName"/>
  </xsl:variable>

  <xsl:variable name="fileExt">
    <xsl:variable name="ts" 
      select="string(/OsirisAnalysisReport/LabSettings/Info/DataFileType)"/>
    <xsl:text>.</xsl:text>
    <xsl:value-of select="$ts"/>
    <xsl:if test="string-length($ts) = 0">
      <xsl:text>fsa</xsl:text>
    </xsl:if>
  </xsl:variable>

  <xsl:template name="MarkerDye">
    <xsl:param name="marker"/>
    <xsl:variable name="xmarker" select="$MarkerChannelDye/x/marker[@name = $marker]"/>
    <xsl:value-of select="$xmarker/@dye"/>
  </xsl:template>

  <xsl:template name="isEnabled">
    <xsl:param name="sample"/>
    <xsl:variable name="rtn">
      <xsl:choose>
        <xsl:when test="not($sample)">
          <xsl:value-of select="0"/>
        </xsl:when>
        <xsl:when test="$sample/EnableHistory/Enable">
          <xsl:for-each select="$sample/EnableHistory/Enable">
            <xsl:sort select="Time" order="descending" data-type="number"/>
            <xsl:choose>
              <xsl:when test="position() != 1"/>
              <xsl:when test="Enabled = 'true'">
                <xsl:value-of select="1"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="0"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="$rtn"/>
  </xsl:template>

  <func:function name="os:isEnabled">
    <xsl:param name="sample"/>
    <xsl:variable name="x">
      <xsl:call-template name="isEnabled">
        <xsl:with-param name="sample" select="$sample"/>
      </xsl:call-template>
    </xsl:variable>
    <func:result select="boolean(number($x))"/>
  </func:function>

  <func:function name="os:isAlleleEnabled">
    <xsl:param name="Allele"/>
    <xsl:variable name="x" select="boolean(not($Allele/Disabled) or ($Allele/Disabled != 'true'))"/>
    <func:result select="$x"/>
  </func:function>

  <func:function name="os:hasAlleles">
    <xsl:param name="sample"/>
    <xsl:variable name="x" select="boolean($sample/Locus/Allele[os:isAlleleEnabled(.)])"/>
    <func:result select="$x"/>
  </func:function>


  <xsl:template name="OsirisExport">
  <export>
    <name>Spreadsheet</name>
    <file-extension>tab</file-extension>
    <file-extension>txt</file-extension>
    <extension-override>true</extension-override>
    <default-location>*A</default-location>
    <xsl-params/>
  </export>
  </xsl:template>

  <xsl:template name="GetBool">
    <xsl:param name="s"/>
    <xsl:choose>
      <xsl:when test="$s = ''">
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:when test="contains('FfNn0',substring($s,1,1))">
        <!-- check for false or no -->
        <xsl:value-of select="0"/>
      </xsl:when>
      <xsl:when test="contains('YyTt123456789',substring($s,1,1))">
        <!-- check for true or yes -->
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:when test="boolean($s) = 'true'">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="0"/>
      </xsl:otherwise>
   </xsl:choose>
  </xsl:template>

  <xsl:template name="writeSample">
    <xsl:param name="sample"/>
    <xsl:variable name="SampleFile" select="$sample/Name"/>

    <!-- loop through sample loci -->

    <xsl:for-each select="$MarkerChannelDye/x/marker">
      <xsl:variable name="dye" select="@dye"/>
      <xsl:variable name="LocusName" select="string(@name)"/>
      <xsl:value-of select="concat($SampleFile,$fileExt,$TAB,$RunName,$TAB,$LocusName,$TAB,$dye)"/>
      <xsl:for-each select="$sample/Locus[string(./LocusName) = $LocusName]/Allele">
        <xsl:variable name="AlleleName">
          <xsl:choose>
            <xsl:when test="OffLadder = 'true'">
              <xsl:text>OL</xsl:text>
            </xsl:when>
            <xsl:when test="$LocusName != 'AMEL'">
              <xsl:value-of select="Name"/>
            </xsl:when>
            <xsl:when test="Name = '1'">
              <xsl:text>X</xsl:text>
            </xsl:when>
            <xsl:when test="Name = '2'">
              <xsl:text>Y</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="Name"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:value-of select="concat($TAB,$AlleleName,$TAB,meanbps,$TAB,RFU)"/>
      </xsl:for-each>
      <xsl:value-of select="$EOL"/>
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="writeSample00000">
    <xsl:param name="sample"/>
    <xsl:variable name="SampleFile" select="$sample/Name"/>

    <xsl:for-each select="$sample/Locus">
      <xsl:variable name="Alleles" select="Allele[os:isAlleleEnabled(.)]"/>
      <xsl:if test="$Alleles">
        <xsl:variable name="LocusName" select="LocusName"/>
        <xsl:variable name="dye">
          <xsl:call-template name="MarkerDye">
            <xsl:with-param name="marker" select="$LocusName"/>
          </xsl:call-template>
        </xsl:variable>

        <xsl:value-of select="concat($SampleFile,$TAB,$RunName,$TAB,$LocusName,$TAB,$dye)"/>
        <xsl:for-each select="$Alleles">
          <xsl:variable name="AlleleName">
            <xsl:choose>
              <xsl:when test="OffLadder = 'true'">
                <xsl:text>OL</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="Name"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>

          <xsl:value-of select="concat($TAB,$AlleleName,$TAB,meanbps,$TAB,RFU)"/>
        </xsl:for-each>
        <xsl:value-of select="$EOL"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="/" priority="10">
    <xsl:variable name="samples" select="/OsirisAnalysisReport/Table/Sample[os:isEnabled(.) and os:hasAlleles(.)]"/>
    <xsl:variable name="samples2" select="/OsirisAnalysisReport/Table/Sample"/>
    <xsl:choose>
      <xsl:when test="$samples">
        <xsl:value-of select="$header"/>
        <xsl:for-each select="$samples">
          <xsl:call-template name="writeSample">
            <xsl:with-param name="sample" select="."/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message>
          <xsl:text>No samples found in this file.</xsl:text>
        </xsl:message>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
