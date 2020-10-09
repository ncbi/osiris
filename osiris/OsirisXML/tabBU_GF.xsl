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
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 101</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 101</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 101</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 102</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 102</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 102</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 103</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 103</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 103</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 104</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 104</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 104</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 105</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 105</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 105</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 106</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 106</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 106</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 107</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 107</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 107</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 108</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 108</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 108</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 109</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 109</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 109</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 110</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 110</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 110</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 111</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 111</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 111</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 112</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 112</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 112</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 113</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 113</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 113</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 114</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 114</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 114</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 115</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 115</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 115</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 116</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 116</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 116</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 117</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 117</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 117</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 118</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 118</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 118</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 119</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 119</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 119</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 120</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 120</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 120</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 121</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 121</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 121</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 122</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 122</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 122</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 123</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 123</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 123</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 124</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 124</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 124</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 125</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 125</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 125</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 126</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 126</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 126</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 127</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 127</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 127</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 128</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 128</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 128</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 129</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 129</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 129</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 130</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 130</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 130</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 131</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 131</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 131</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 132</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 132</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 132</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 133</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 133</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 133</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 134</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 134</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 134</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 135</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 135</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 135</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 136</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 136</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 136</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 137</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 137</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 137</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 138</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 138</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 138</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 139</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 139</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 139</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 140</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 140</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 140</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 141</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 141</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 141</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 142</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 142</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 142</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 143</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 143</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 143</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 144</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 144</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 144</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 145</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 145</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 145</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 146</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 146</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 146</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 147</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 147</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 147</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 148</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 148</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 148</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 149</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 149</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 149</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 150</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 150</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 150</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 151</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 151</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 151</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 152</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 152</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 152</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 153</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 153</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 153</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 154</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 154</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 154</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 155</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 155</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 155</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 156</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 156</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 156</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 157</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 157</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 157</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 158</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 158</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 158</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 159</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 159</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 159</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 160</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 160</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 160</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 161</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 161</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 161</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 162</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 162</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 162</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 163</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 163</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 163</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 164</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 164</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 164</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 165</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 165</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 165</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 166</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 166</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 166</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 167</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 167</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 167</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 168</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 168</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 168</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 169</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 169</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 169</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 170</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 170</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 170</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 171</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 171</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 171</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 172</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 172</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 172</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 173</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 173</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 173</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 174</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 174</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 174</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 175</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 175</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 175</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 176</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 176</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 176</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 177</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 177</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 177</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 178</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 178</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 178</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 179</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 179</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 179</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 180</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 180</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 180</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 181</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 181</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 181</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 182</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 182</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 182</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 183</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 183</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 183</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 184</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 184</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 184</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 185</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 185</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 185</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 186</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 186</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 186</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 187</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 187</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 187</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 188</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 188</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 188</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 189</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 189</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 189</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 190</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 190</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 190</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 191</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 191</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 191</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 192</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 192</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 192</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 193</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 193</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 193</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 194</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 194</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 194</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 195</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 195</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 195</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 196</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 196</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 196</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 197</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 197</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 197</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 198</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 198</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 198</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 199</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 199</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 199</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 200</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 200</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 200</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 201</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 201</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 201</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 202</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 202</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 202</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 203</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 203</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 203</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 204</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 204</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 204</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 205</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 205</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 205</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 206</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 206</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 206</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 207</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 207</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 207</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 208</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 208</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 208</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 209</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 209</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 209</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 210</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 210</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 210</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 211</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 211</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 211</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 212</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 212</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 212</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 213</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 213</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 213</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 214</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 214</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 214</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 215</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 215</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 215</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 216</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 216</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 216</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 217</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 217</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 217</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 218</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 218</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 218</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 219</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 219</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 219</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 220</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 220</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 220</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 221</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 221</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 221</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 222</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 222</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 222</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 223</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 223</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 223</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 224</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 224</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 224</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 225</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 225</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 225</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 226</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 226</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 226</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 227</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 227</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 227</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 228</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 228</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 228</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 229</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 229</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 229</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 230</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 230</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 230</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 231</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 231</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 231</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 232</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 232</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 232</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 233</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 233</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 233</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 234</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 234</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 234</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 235</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 235</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 235</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 236</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 236</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 236</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 237</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 237</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 237</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 238</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 238</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 238</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 239</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 239</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 239</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 240</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 240</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 240</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 241</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 241</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 241</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 242</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 242</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 242</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 243</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 243</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 243</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 244</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 244</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 244</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 245</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 245</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 245</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 246</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 246</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 246</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 247</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 247</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 247</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 248</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 248</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 248</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 249</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 249</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 249</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 250</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 250</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 250</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 251</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 251</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 251</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 252</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 252</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 252</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 253</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 253</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 253</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 254</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 254</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 254</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 255</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 255</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 255</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 256</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 256</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 256</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 257</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 257</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 257</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 258</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 258</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 258</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 259</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 259</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 259</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 260</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 260</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 260</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 261</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 261</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 261</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 262</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 262</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 262</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 263</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 263</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 263</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 264</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 264</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 264</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 265</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 265</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 265</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 266</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 266</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 266</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 267</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 267</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 267</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 268</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 268</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 268</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 269</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 269</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 269</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 270</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 270</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 270</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 271</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 271</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 271</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 272</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 272</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 272</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 273</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 273</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 273</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 274</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 274</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 274</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 275</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 275</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 275</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 276</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 276</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 276</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 277</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 277</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 277</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 278</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 278</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 278</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 279</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 279</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 279</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 280</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 280</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 280</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 281</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 281</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 281</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 282</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 282</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 282</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 283</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 283</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 283</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 284</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 284</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 284</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 285</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 285</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 285</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 286</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 286</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 286</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 287</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 287</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 287</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 288</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 288</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 288</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 289</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 289</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 289</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 290</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 290</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 290</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 291</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 291</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 291</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 292</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 292</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 292</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 293</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 293</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 293</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 294</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 294</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 294</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 295</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 295</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 295</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 296</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 296</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 296</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 297</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 297</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 297</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 298</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 298</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 298</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 299</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 299</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 299</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Allele 300</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Size 300</xsl:text>
    <xsl:value-of select="$TAB"/>
    <xsl:text>Height 300</xsl:text>
    <xsl:value-of select="$TAB"/>

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
      <xsl:text>hid</xsl:text>
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
      <xsl:for-each select="$sample/Locus[string(./LocusName) = $LocusName]/Allele[os:isAlleleEnabled(.)]">
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
