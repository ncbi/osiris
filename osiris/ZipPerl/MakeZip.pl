#!/usr/bin/perl
use strict 'vars';
use GetVersion;
my $VERSION = &GetVersion::Get();
my $CP = "cp -vup ";

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
  Cofiler
  GF
  GFHID
  ID
  IDNO250
  IDNO250_HID
  ID_HID
  IDplus
  IDplusNO250
  NGMSElect
  NGM
  PP12
  PP16
  PP16_HID
  PP18D
  PP21
  PPESI16_FSA
  PPESI16_HID
  PPESI17_FSA
  PPESI17_HID
  PPESX16_FSA
  PPESX16_HID
  PPESX17_FSA
  PPESX17_HID
  PPFusion
  PPFusion_HID
  PPY
  PPY23
  PPY23_HID
  Profiler
  SEfilerPlus
  SGMPlus
  Yfiler
  /)
  {
    my $dest1 = "${dest}/Config/Volumes/${sdir}";
    &MKDIR($dest1);
    &SYSTEM("${CP} ${src}/OsirisXML/NamedxmlConfigurations/${sdir}/*.??? ${dest1}");
  }

  my $config;

  for $config (qw|Common.xsd LabSettingsV4.xsd LadderSpecifications/ILSAndLadderInfo.xsd LadderSpecifications/MarkerSetV4.xsd StandardSettingsV4.xsd OsirisAnalysisReport-2.0.xsd StandardSettingsType.xsd|)
  {
    &SYSTEM("${CP} ${src}/OsirisXML/${config} ${dest}/Config/xsd");
  }
  &SYSTEM("${CP} ${src}/MessageBook/OsirisMessageBook4.0.xsd ${dest}/Config/xsd");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/tab.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/extractSamples.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/extractArtifacts.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/*LadderInfo.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/kitcolors.xml ${dest}/Config/LadderSpecifications");

  &SYSTEM("${CP} ${src}/docs/readme.rtf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OsirisHelp.pdf ${dest}");
}

sub CopyWin
{
  my $testCP = `cp --version`;
  (length($testCP) < 10) &&
    die("Cannot find cp command, need to install " .
        "cygwin or include it in the path");
  my $dir = `dirname $0`;
  $dir =~ s/\\/\//g;
  chomp $dir;
  length($dir) && (chdir($dir) || die("Cannot chdir ${dir}"));

  my $src = "..";
  my $dest = "./Osiris";
  &COPYFILES($src,$dest);
  &MKDIR("${dest}/site");
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/Release/TestAnalysisDirectoryLC.exe ${dest}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/Release/OsirisAnalysis.exe ${dest}");
  &SYSTEM("${CP} ${src}/MessageBook/cpmsg.bat ${dest}");
  &SYSTEM("${CP} ${src}/OsirisXML/names.bat ${dest}");
#  &SYSTEM("${CP} ${src}/Setup1/uninstall.bat ${dest}");

  my $zipFile = "Osiris-${VERSION}-Windows.zip";

  if(&TESTFILES($zipFile,"${dest}"))
  {
    &SYSTEM("7z a -r ${zipFile} ${dest}");
  }
}

sub CopyMac
{
  $CP = "cp -p ";
  my $dir = `dirname $0`;
  $dir =~ s/\\/\//g;
  chomp $dir;
  length($dir) && (chdir($dir) || die("Cannot chdir ${dir}"));

  my $TOP = $ENV{HOME} . "/Apps";
  if (!(-d $TOP))
  {
    $TOP = "/Applications";
    (-d $TOP) || die("Cannot find destination folder ${TOP}");
  }
  my $APPDIR = "${TOP}/Osiris.app";
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

  &SYSTEM("${CP} ${src}/OsirisAnalysis/bin/osirisanalysis ${DEST}");
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/bin/TestAnalysisDirectoryLC ${DEST}");
  &SYSTEM("strip ${DEST}/osirisanalysis");
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
      &SYSTEM("cd \"${TOP}\" ; tar zcvf ${zipFile} TestAnalysis OsirisXSL Osiris.app");
    }
  }  ## end if(1)
  ### END make tar.gz
}

my $home = $ENV{"HOME"};

if( $home =~ m|^/Users| )
{
  &CopyMac;
}
elsif( (!length($home)) || ($home =~ m|^.:|) || (substr($home,0,2) eq "\\\\") )
{
  &CopyWin;
}
else
{
  die("Cannot figure out which OS this is.");
}

0;
