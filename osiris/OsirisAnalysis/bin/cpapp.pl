#!/usr/bin/perl
use strict 'vars';
my $filesCopied = 0;
sub System
{
  my $cmd = shift;
  local $\ = "\n";
  print "${cmd}";
  system($cmd);
}
sub CopyFile
{
  my ($src,$dst,$strip) = @_;
  my $MTIME = 9;
  if(-d $dst)
  {
    my $fn = $src;
    $fn =~ s|.*/||g;
    $dst = "${dst}/$fn";
  }
  my @s1 = stat($src);
  my @s2 = stat($dst);
  my $rtn = 0;
  if($#s1 < $MTIME)
  {
    $rtn = 1;
    print "Cannot find ${src}\n";
  }
  elsif(($#s2 < $MTIME) || ($s1[$MTIME] > $s2[$MTIME]))
  {
    my $cmd = "cp -p \"${src}\" \"${dst}\"";
    $rtn = &System($cmd);
    if(!$rtn)
    {
      $filesCopied++;
      if($strip)
      {
        $cmd = "strip \"${dst}\"";
        $rtn = &System($cmd);
      }
    }
  }
  $rtn;
}
sub DIE
{
  die("Failed\n");
}
sub CheckTestAnalysis
{
  my $dest = shift;
  my $gzip = "../../docs/TestAnalysis.tar.gz";
  if( (-f $gzip) && !(-d "${dest}/TestAnalysis") )
  {
    my $cmd = "gzip -cd ${gzip} | (cd \"${dest}\" ; tar xf - )";
    print $cmd ."\n";
    system $cmd;
    $filesCopied++;
  }
}
sub MKDIRS
{
  my @dirs = @_;
  my $d;
  for $d (@dirs)
  {
    if(! -d $d)
    {
      print "mkdir ${d}\n";
      mkdir($d,0755) || &DIE;

    }
  }
}

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
my $XML = "../../OsirisXML";
my $MSG = "../../MessageBook";
my $RESOURCES = "${CONTENTS}/Resources";
my @dirs = ($APPDIR,$CONTENTS, $DEST, $CONFIG, $RESOURCES);
&MKDIRS(@dirs);
&CopyFile("../wxmac.icns",$RESOURCES);
&CopyFile("../Info.plist",$CONTENTS);
&CopyFile("../PkgInfo",$CONTENTS);
&CopyFile("osirisanalysis","${DEST}",1) && &DIE;
&CopyFile(
  "../../TestAnalysisDirectoryLC/bin/TestAnalysisDirectoryLC",
  ${DEST},1) && &DIE;
&CopyFile("${XML}/GenericLabSettings.xml","${CONFIG}/defaultlab.xml") && &DIE;
my @clist = qw/kitcolors.xml Common.xsd LabSettingsV2type.xsd LadderData.xml MarkerSets.xsd OsirisAnalysisReport-1.2.xsd OsirisSTRStandardSettings.xml StandardSettingsType.xsd/;
for my $f (@clist)
{
  &CopyFile("${XML}/${f}","$CONFIG") && &DIE;
}

#for my $f ("OsirisMessageBook3.xsd","OsirisMessageBookV3.1.xml","OsirisMessageBook3.1.xsd","OsirisMessageBook2.xml","OsirisMessageBook2.xsd")
for my $f (glob("${MSG}/*.x*"))
{
  &CopyFile("$f",$CONFIG) && &DIE;
}

&CopyFile("../../docs/OsirisHelp.pdf",$DEST);

&CheckTestAnalysis($DEST);

if($filesCopied && (-r "./mktar"))
{
  system("sh ./mktar");
}


0;
