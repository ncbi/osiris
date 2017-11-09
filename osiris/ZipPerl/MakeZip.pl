#!/usr/bin/perl
use strict 'vars';
use GetVersion;
#
#  this script builds the Osiris for Windows zip distribution
#  or the macintosh tar.gz distribution
#  for Windows it requires cygwin, perl, and 7zip
#  and the cygwin bin must be in the PATH
#
#  the following may need to be changed
#
#
#
my $NO_ZIP_FILE = 0;
my $MAC_TOP_DIR = $ENV{HOME} . "/Applications";
my $VERSION = &GetVersion::Get();
my $CP = "cp -vup ";
my $DRIVE = "/cygdrive/c";
my $VCDIR = "${DRIVE}/Program Files (x86)/Microsoft Visual Studio 10.0/VC";
my $COPYDLL = 1; ## set to 0 if using MS Visual C++ Express
my $PATH7Z = "7z";  ## if 7z.exe is not in the path, set the full DOS path here


sub MKDIR
{
  my $s = shift;
  if(! -d $s)
  {
     (mkdir $s) || die("cannot mkdir ${s}");
     print "mkdir ${s}\n";
  }
}
sub SYSTEM
{
  my $s = shift;
  my $n = system($s);
  $n &&
    die("${s}\nreturn code = ${n}\n");
  0;
}
sub NEWER
{
  my ($testFile,$file) = @_;
  my @s1 = stat($testFile);
  my @s2 = stat($file);
  my $rtn = 0;
  ($#s1 > 8) && ($#s2 > 8) && ($s1[9] > $s2[9]) && ($rtn = 1);
  $rtn;
}
sub TESTFILES
{
  my ($file,$dir) = @_;
  my $rtn = 0;
  ## return 1 if a file in ${dir} is newer than $file

  if(!(-r $file))
  {
    $rtn = 1;
  }
  elsif(-d $dir)
  {
    for my $testFile (glob("${dir}/*"))
    {
      if(-d $testFile)
      {
        &TESTFILES($file,$testFile) && ($rtn = 1);
      }
      else
      {
        &NEWER($testFile,$file) && ($rtn = 1);
      }
      last if $rtn;
    }
  }
  $rtn;
}
sub COPYFILES
{
  my $src = shift;
  my $dest = shift;
  my $destTest = shift;
  my $destXSL = '';
  if($destTest)
  {
    $destXSL = "${destTest}/OsirisXSL";
    &MKDIR($destTest);
    &MKDIR($destXSL);
  }
  else
  {
    $destTest = $dest;
    $destXSL = "${dest}/Config"
  }

  &MKDIR("${dest}");
  &MKDIR("${dest}/Config");
  &MKDIR("${dest}/Config/Volumes");
  &MKDIR("${dest}/Config/xsd");
  &MKDIR("${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} -R ${src}/docs/TestAnalysis ${destTest}");
  for my $sdir (qw/
    CANNABIS
    Cofiler
    GF
    GFHID
    GP10
    ID
    IDNO250
    IDNO250_HID
    IDplus
    IDplusNO250
    ID_HID
    iPLEXSTR
    iPLEXSTRLIZ500less250
    MINIFILER
    NETBIO_FLEXPLEX
    NETBIO_FLEXPLEX_HID
    NFX1
    NGM
    NGMSElect
    POWERPLEXS5
    PP12
    PP16
    PP16_HID
    PP18D
    PP18D_HID
    PP21
    PP21_HID
    PPESI16
    PPESI16_HID
    PPESI17
    PPESI17_HID
    PPESX16
    PPESX16_HID
    PPESX17
    PPESX17_HID
    PPFusion
    PPFusion6C
    PPFusion6C_HID
    PPFusion_HID
    PPY
    PPY23
    PPY23_HID
    PPGenePrint24
    PPGenePrint24_HID
    Profiler
    QIAGEN_ARGUS_x12
    QIAGEN_ARGUS_x12_HID
    QIAGEN_ARGUS_x12_QS
    QIAGEN_ARGUS_x12_QS_HID
    QIAGEN_INVESTIGATOR24PLEX
    QIAGEN_INVESTIGATOR24PLEX_HID
    SEfilerPlus
    SGMPlus
    Yfiler
    YFILERPLUS
    YFILERPLUS_HID
  /)
  {
    my $dest1 = "${dest}/Config/Volumes/${sdir}";
    &MKDIR($dest1);
    &SYSTEM("${CP} ${src}/OsirisXML/NamedxmlConfigurations/${sdir}/*.??? ${dest1}");
  }

  my $config;

  for $config (qw|Common.xsd ArtifactLabelsSchema.xsd LabSettingsV4.xsd LadderSpecifications/ILSAndLadderInfo.xsd LadderSpecifications/MarkerSetV4.xsd StandardSettingsV4.xsd OsirisAnalysisReport-2.0.xsd StandardSettingsType.xsd|)
  {
    &SYSTEM("${CP} ${src}/OsirisXML/${config} ${dest}/Config/xsd");
  }
  &SYSTEM("${CP} ${src}/MessageBook/OsirisMessageBookV4.0.xml ${dest}/Config/LadderSpecifications/MessageBook.xml");
  &SYSTEM("${CP} ${src}/MessageBook/OsirisMessageBook4.0.xsd ${dest}/Config/xsd");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/tab.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/extractSamples.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/extractArtifacts.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/*LadderInfo.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/kitcolors.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/kitcolors2.0.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/ArtifactLabels.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/SiteSettings.xml ${dest}/Config");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/StandardPositiveControls.xml ${dest}/Config/LadderSpecifications");

  &SYSTEM("${CP} ${src}/docs/readme.rtf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OsirisHelp.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/V2.3-Change-Log.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.4.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.5.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.6.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.7.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.8_2.9.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.10.pdf ${dest}");
}

sub CopyWin
{
  my $testCP = `cp --version`;
  (length($testCP) < 10) &&
    die("Cannot find cp command, need to install " .
        "cygwin or include it in the path");
  my $PLAT = "x86";
  my $dir = `dirname $0`;
  $dir =~ s/\\/\//g;
  chomp $dir;
  length($dir) && (chdir($dir) || die("Cannot chdir ${dir}"));

  my $src = "..";
  my $sSuffix = "${VERSION}";
  $sSuffix =~ s/ /-/g;
  my $dest = './' . &GetVersion::GetDirectory();
  &COPYFILES($src,$dest);
  &MKDIR("${dest}/site");
  if ($COPYDLL)
  {
    my $DLLPATH= "${VCDIR}/redist/${PLAT}/Microsoft.VC100.CRT";  ## cygwin
    &SYSTEM("cp -uv --preserve=mode,timestamps \"${DLLPATH}/msvcp100.dll\" ${dest}");
    &SYSTEM("cp -uv --preserve=mode,timestamps \"${DLLPATH}/msvcr100.dll\" ${dest}");
  }
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/Release/TestAnalysisDirectoryLC.exe ${dest}");
  &SYSTEM("${CP} ${src}/fsa2xml/Release/fsa2xml.exe ${dest}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/Release/OsirisAnalysis.exe ${dest}");
  &SYSTEM("${CP} ${src}/MessageBook/cpmsg.bat ${dest}");
  &SYSTEM("${CP} ${src}/OsirisXML/names.bat ${dest}");
#  &SYSTEM("${CP} ${src}/Setup1/uninstall.bat ${dest}");

  my $zipFile = "${dest}-Windows.zip";

  if((!$NO_ZIP_FILE) && &TESTFILES($zipFile,"${dest}"))
  {
    (-r $zipFile) && (unlink($zipFile) || die("Cannot remove ${zipFile}"));
    &SYSTEM("${PATH7Z} a -r ${zipFile} ${dest}");
  }
}

sub CopyMac
{
  $CP = "cp -p ";
  my $dir = `dirname $0`;
  $dir =~ s/\\/\//g;
  chomp $dir;
  length($dir) && (chdir($dir) || die("Cannot chdir ${dir}"));

  my $TOP = $MAC_TOP_DIR;
  if (! -d $TOP)
  {
    &MKDIR($TOP);
  }
  my $sDir = &GetVersion::GetDirectory();
  my $APPDIR = "${TOP}/${sDir}.app";
  my $CONTENTS = "${APPDIR}/Contents";
  my $DEST = "${CONTENTS}/MacOS";
  my $CONFIG = "${DEST}/Config";
  my $LADDERDIR = "${CONFIG}/LadderSpecifications";
  my $VOLUMEDIR = "${CONFIG}/Volumes";
  my $RESOURCES = "${CONTENTS}/Resources";

  my $src = "..";
  my $x;
  for $x ($APPDIR,$CONTENTS,$DEST,$CONFIG,$LADDERDIR,$VOLUMEDIR,$RESOURCES)
  {
     &MKDIR($x);
  }
  &SYSTEM("${CP} ${src}/OsirisAnalysis/wxmac.icns ${RESOURCES}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/Info.plist ${CONTENTS}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/PkgInfo ${CONTENTS}");

  &SYSTEM("${CP} ${src}/OsirisAnalysis/cpmsgmac.sh ${DEST}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/bin/osiris ${DEST}");
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/bin/TestAnalysisDirectoryLC ${DEST}");
  &SYSTEM("${CP} ${src}/fsa2xml/bin/fsa2xml ${DEST}");
  &SYSTEM("strip ${DEST}/osiris");
  &SYSTEM("strip ${DEST}/TestAnalysisDirectoryLC");
  my $testDest = "${TOP}/TestAnalysis";
  &COPYFILES($src,$DEST,$TOP);

  ### BEGIN make tar.gz
  if(1)
  {
    my $zipDir = $ENV{OSIRISTGZ};
    if(!(-d $zipDir && -w $zipDir))
    {
      $zipDir = `dirname $0`;
      chomp $zipDir;
      chdir "${zipDir}/..";
      $zipDir = `pwd`;
      chomp $zipDir;
    }
    my $zipFile = "${zipDir}/Osiris-Mac-${VERSION}.tar.gz";
    if( ($#ARGV >= 0) || &TESTFILES($zipFile,$DEST) ||
        &TESTFILES($zipFile,$testDest) )
    {
      &SYSTEM("cd \"${TOP}\" ; /usr/bin/tar zcvf ${zipFile} TestAnalysis OsirisXSL ${sDir}.app");
    }
  }  ## end if(1)
  ### END make tar.gz
}

my $home = $ENV{"HOME"};
my $uname = `uname`;

if( $home =~ m|^/Users| )
{
  &CopyMac;
}
elsif( ($uname =~ m/cygwin/i) || ($uname =~ m/msys/i) || (!length($home)) || ($home =~ m|^.:|) || (substr($home,0,2) eq "\\\\") )
{
  &CopyWin;
}
else
{
  die("Cannot figure out which OS this is.");
}

0;
