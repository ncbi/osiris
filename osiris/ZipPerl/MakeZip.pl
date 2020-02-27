#!/usr/bin/perl
#
# under windows, this should run from activeperl
#
use strict 'vars';
use GetVersion;
use File::Path qw(remove_tree);
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
my $MAC_TOP_DIR = $ENV{HOME} . "/Applications/Osiris-Installer";
my $VERSION = &GetVersion::Get();
my $CP = "cp -vup ";

my $COPYDLL = 1; ## set to 0 if using MS Visual C++ Express
my $PATH7Z = 'c:\\Progra~1\\7-Zip\\7z.exe';  ## if 7z.exe is not in the path, set the full DOS path here


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
  if($destTest)
  {
    &MKDIR($destTest);
  }
  else
  {
    $destTest = $dest;
  }
  my $destXSL = "${dest}/ExportFiles";
  &MKDIR("${dest}");
  &MKDIR($destXSL)
  &MKDIR("${dest}/Config");
  &MKDIR("${dest}/Config/Volumes");
  &MKDIR("${dest}/Config/xsd");
  &MKDIR("${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} -R ${src}/docs/TestAnalysis ${destTest}");
  for my $sdir (qw/
    CANNABIS
    Cofiler
    CORDIS_PLUS
    CORDIS_PLUS_HID
    GF
    GFHID
    GlobalFilerMixNoStutter
    GlobalFilerMixStutter
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
    LANESTANDARDONLY2
    LANESTANDARDONLY3
    LANESTANDARDONLY4
    LANESTANDARDONLY5
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
    PPFusion_HID_Sole_Source
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

  &SYSTEM("${CP} ${src}/OsirisXML/SampleLadderData.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisXML/SpreadSheetTemplate.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisXML/SpreadSheetUtils.xsl ${destXSL}");
  &SYSTEM("${CP} ${src}/OsirisXML/util.xsl ${destXSL}");

  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/*LadderInfo.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/kitcolors.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/LadderSpecifications/kitcolors2.0.xml ${dest}/Config/LadderSpecifications");
  &SYSTEM("${CP} ${src}/OsirisXML/CmfList.xml ${dest}/Config");
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
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.10.2.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.10.3.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.11.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.11.1.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.11.2.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.12.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.12.1.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.12.2.pdf ${dest}");
  &SYSTEM("${CP} ${src}/docs/OSIRIS_Release_Notes_v2.13.pdf ${dest}");
}
sub GetVCDir
{
  my $DRIVE = undef;
  my $PLAT = "x86";
  for my $sDrive ("/cygdrive/c","/c")
  {
    my $ok = system("test -d ${sDrive}");
    ($ok == 0) && ($DRIVE = $sDrive) && last;
  }
  $DRIVE || die("Cannot find mingw or cygwin c drive");
  #my $p = "${DRIVE}/Program Files (x86)/Microsoft Visual Studio 10.0/VC"; 
  my $p = "${DRIVE}/Program Files (x86)/Microsoft Visual Studio/2017/Professional/VC/Redist/MSVC/14.16.27012/x86/Microsoft.VC141.CRT";
  system("test -d \"$p\"") && die("Cannot find MS Visual Studio (${p})");
  $p;
#  my $DLLPATH= "${p}/redist/${PLAT}/Microsoft.VC100.CRT";  ## cygwin
#  system("test -d \"$DLLPATH\"") && die("Cannot find MS runtime");
#  $DLLPATH;
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
  my $sSuffix = "${VERSION}";
  $sSuffix =~ s/ /-/g;
  my $dest = './' . &GetVersion::GetDirectory();
  &COPYFILES($src,$dest);
  if ($COPYDLL)
  {
    my $DLLPATH = &GetVCDir();
    &SYSTEM("cp -uv --preserve=mode,timestamps \"${DLLPATH}/\"*.dll ${dest}") && die();
  }
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/Release/TestAnalysisDirectoryLC.exe ${dest}");
  &SYSTEM("${CP} ${src}/fsa2xml/Release/fsa2xml.exe ${dest}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/Release/OsirisAnalysis.exe ${dest}");
  &SYSTEM("${CP} ${src}/MessageBook/cpmsg.bat ${dest}");
  &SYSTEM("${CP} ${src}/OsirisXML/names.bat ${dest}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/CSitePath.vbs ${dest}");
  &SYSTEM("${CP} ${src}/nwx/pinger.vbs ${dest}");
#  &SYSTEM("${CP} ${src}/Setup1/uninstall.bat ${dest}");

  my $zipFile = "${dest}-Windows.exe";

  if((!$NO_ZIP_FILE) && &TESTFILES($zipFile,"${dest}"))
  {
    (-r $zipFile) && (unlink($zipFile) || die("Cannot remove ${zipFile}"));
    &SYSTEM("${PATH7Z} a -r -sfx7z.sfx  ${zipFile} ${dest}");
  }
}

sub CheckSignMac()
{
  my $PATH=shift;
  my $SIG = shift;
  my $NOSIGN = "\nApplication will not be signed.";
  if(length($ENV{NOSIGN}))
  {
    return undef;
  }
  if(! (-d $PATH || -f $PATH))
  {
    print STDERR "Application path, ${PATH}, is not found",$NOSIGN;
    return undef;
  }
  if(!length($SIG))
  {
    print STDERR "Cannot find environment variable, SIGNATURE",$NOSIGN;
    return undef;
  }
  my $XDISP=$ENV{DISPLAY};
  if(!length($XDISP))
  {
    print STDERR "Cannot find environment variable, DISPLAY,\n",
                 "and cannot determine if this is the mac console.",$NOSIGN;
    return undef;
  }
  if($XDISP !~ m/(^.*os.macosforce.xquartz)?:0(\.0+)?$/)
  {
    print STDERR "Environment variable, DISPLAY=${XDISP}\n",
                 "and cannot determine if this is the mac console.",$NOSIGN;
    return undef;
  }
  return 1;
}


sub CopyMac
{
  $CP = "cp -p ";
  my $dir = `dirname $0`;
  $dir =~ s/\\/\//g;
  chomp $dir;
  length($dir) && (chdir($dir) || die("Cannot chdir ${dir}"));

  my $TOP = $MAC_TOP_DIR;
  ((-d $TOP) ? remove_tree($TOP) : 1) || die("Cannot rmdir ${TOP}");
  &MKDIR($TOP);
  my $SYSTEM_FOLDER_NAME = "System Applications Folder";
  my $USER_FOLDER_NAME = "User Applications Folder";


  my $sDir = &GetVersion::GetDirectory();
  my $APPDIR = "${TOP}/${sDir}.app";
  my $CONTENTS = "${APPDIR}/Contents";
  my $DEST = "${CONTENTS}/MacOS";
  my $CONFIG = "${DEST}/Config";
  my $LADDERDIR = "${CONFIG}/LadderSpecifications";
  my $VOLUMEDIR = "${CONFIG}/Volumes";
  my $RESOURCES = "${CONTENTS}/Resources";
  my $APP_SYS_FOLDER = "${TOP}/${SYSTEM_FOLDER_NAME}.app";
  my $APP_USER_FOLDER = "${TOP}/${USER_FOLDER_NAME}.app";

  my $SYSTEM_FOLDER_DEST =
    "${APP_SYS_FOLDER}/Contents/MacOS";
  my $USER_FOLDER_DEST =
    "${APP_USER_FOLDER}/Contents/MacOS";


  my @APP_FOLDERS =
    (
     "${APP_SYS_FOLDER}",
     "${APP_SYS_FOLDER}/Contents",
     "${SYSTEM_FOLDER_DEST}",
     "${APP_USER_FOLDER}",
     "${APP_USER_FOLDER}/Contents",
     "${USER_FOLDER_DEST}"
    );
  my $src = "..";
  my $x;
  for $x ($APPDIR,$CONTENTS,$DEST,$CONFIG,$LADDERDIR,$VOLUMEDIR,$RESOURCES,@APP_FOLDERS)
  {
     &MKDIR($x);
  }



  &SYSTEM("${CP} ${src}/dmg/00_Read_This_First.pdf ${TOP}");

  &SYSTEM("${CP} ${src}/OsirisAnalysis/wxmac.icns ${RESOURCES}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/Info.plist ${CONTENTS}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/PkgInfo ${CONTENTS}");

  &SYSTEM("${CP} ${src}/nwx/pinger.sh ${DEST}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/CSitePath.sh ${DEST}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/cpmsgmac.sh ${DEST}");
  &SYSTEM("${CP} ${src}/OsirisAnalysis/bin/osiris ${DEST}");
  &SYSTEM("${CP} ${src}/TestAnalysisDirectoryLCv2.11/bin/TestAnalysisDirectoryLC ${DEST}");
  &SYSTEM("${CP} ${src}/fsa2xml/bin/fsa2xml ${DEST}");

  &SYSTEM("${CP} -R \"${src}/dmg/System Applications Folder.app\" \"${TOP}\"");
  &SYSTEM("${CP} -R \"${src}/dmg/User Applications Folder.app\" \"${TOP}\"");
  #
  #  create the folder apps for dmg
  #

  &SYSTEM("${CP} ${src}/dmg/user_folder.sh \"${USER_FOLDER_DEST}/folder.sh\"");
  &SYSTEM("${CP} ${src}/dmg/sys_folder.sh \"${SYSTEM_FOLDER_DEST}/folder.sh\"");

  &SYSTEM("${CP} ${src}/OpenFolder/bin/OpenFolder \"${SYSTEM_FOLDER_DEST}/${SYSTEM_FOLDER_NAME}\"");
  &SYSTEM("${CP} ${src}/OpenFolder/bin/OpenFolder \"${USER_FOLDER_DEST}/${USER_FOLDER_NAME}\"");

  # done with folder apps
  #
  &SYSTEM("strip ${DEST}/osiris");
  &SYSTEM("strip ${DEST}/TestAnalysisDirectoryLC");
  &COPYFILES($src,$DEST,$TOP);

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
