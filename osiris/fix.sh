#!/bin/sh
function KL()
{
  if test -d "$1" -o -f "$1" ; then
   echo "$1"
   mv "$1" ../junk
   if test "$?" != "0"; then
    exit 1
   fi
  else
   echo "$1 not found"
  fi
}

unzip libs.zip
if test "$?" != "0"; then
    exit 1
fi
if test ! -d ../junk ; then
  mkdir ../junk
  if test ! -d ../junk ; then
    exit
  fi
fi
cd docs
tar zxvf TestAnalysis.tar.gz 
cd ..
KL docs/TestAnalysis.tar.gz
KL libs.zip
KL Artifacts
KL AutoMapSmartMessages
KL CParseLine
KL DirectoryTest
KL Documentation
KL Frank
KL Frankenstein
KL GenerateNotices
KL GenerateSmartNotices
KL Interpretation
KL MultiFSA
KL OsirisExtensionLib2.0
KL OsirisJournalArticle
KL OsirisLib
KL OsirisLib1.3
KL OsirisLib2.0
KL OsirisMsgLib
KL OsirisSmartMessageArticle
KL OsirisSummaryArticle
KL OsirisUML
KL OsirisWeb
KL ParseLine
KL RebuildProject
KL Schema
KL SchemaTools
KL TestAbstractPersistence
KL TestAnalysisDirectoryLC
KL TestAnalysisDirectoryLCv2.0
KL TestAnalysisDirectoryLCv2.1
KL TestCSpline
KL TestDataMoments
KL TestDotProducts
KL TestExpressionCompiler
KL TestFSAData
KL TestFsaDirectory
KL TestGeneticInput
KL TestGenotypeXMLInputOutput
KL TestLadderLocusCentric
KL TestLadders
KL TestLaneStandardLocusCentric
KL TestMessageBook
KL TestNoise
KL TestPeakSearch
KL TestRawDataOffsetCorrection
KL TestSVD
KL TestValidation
KL TestValidationDBDirectory
KL TestValidationDBDirectoryLC
KL TestXML
KL XmlWrap
KL iconv-1.9.2.win32
KL java
KL libxml2-2.6.28.win32
KL tfatsHelp
KL zipFiles
KL zlib-1.2.3.win32
KL fix.sh
KL versionUpdate.txt
