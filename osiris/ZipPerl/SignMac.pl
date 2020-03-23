#!/usr/bin/perl
#
# under windows, this should run from activeperl
#
use strict 'vars';
use GetVersion;
#
#  this script signs OSIRIS for Macintosh and creates
#  a tar.gz and .dmg file.
#
#

my $TOP = $ENV{"HOME"} . "/Applications/Osiris-Installer";
my $VERSION = &GetVersion::Get();
my $DISPLAY = $ENV{DISPLAY};
my $SIGNATURE = $ENV{SIGNATURE};
my $NOSIGN = "\napplication will not be signed.\n";

(-d $TOP) || die("Cannot find directory ${TOP}");
{
  my $uname = `uname`;
  chomp $uname;
  ($uname eq "Darwin") ||
    die("This doesn't appear to be a Macintosh");
}

sub SkipSign()
{
  my $sNOSIGN = $ENV{NOSIGN};
  my $rtn = undef;
  if(length(${sNOSIGN}))
  {
    print STDERR "NOSIGN=${sNOSIGN} found in the environment,", $NOSIGN;
    $rtn = 1;
  }
  $rtn;
}

sub CheckEnv()
{
  my $rtn = 1;
  my $XDISP=$ENV{DISPLAY};
  if(!length($SIGNATURE))
  {
    print STDERR "Cannot find environment variable, SIGNATURE,",$NOSIGN;
    $rtn = undef;
  }
  elsif(!length($XDISP))
  {
    print STDERR "Cannot find environment variable, DISPLAY,\n",
      "and cannot determine if this is the mac console,",$NOSIGN;
    $rtn = undef;
  }
  elsif($XDISP !~ m/(^.*os.macosforce.xquartz)?:0(\.0+)?$/)
  {
    print STDERR "Environment variable, DISPLAY=${XDISP}\n",
      "and cannot determine if this is the mac console,",$NOSIGN;
    $rtn = undef;
  }
  $rtn;
}

sub CheckSignMac()
{
  my $PATH=shift;
  if(! (-d $PATH || -f $PATH))
  {
    print STDERR "Application path, ${PATH}, is not found\n";
    return undef;
  }
  return 1;
}

sub SignAppMac()
{
  my $PATH=shift;
  my $rtn = undef;
  if(&CheckSignMac($PATH))
  {
    my $cmd = "codesign -f --options runtime --deep -s \"${SIGNATURE}\" \"${PATH}\"";
    print "${cmd}\n";
    my $nRtn = system($cmd);
    my $s = $nRtn ? " not" : "";
    print "\nApplication was${s} signed\n\n\n";
    $rtn = !$nRtn
  }
  return $rtn;
}
sub SignAppMacFiles()
{
  sub _signOneFile
  {
    my $line = shift;
    my $cmd = "codesign -f --options runtime -s \"${SIGNATURE}\" \"${line}\"";
    print "${cmd}\n";
    my $n = system($cmd);
    $n && print("FAILED to sign ${line}");
    $n;
  }
  my $PATH=shift;
  my $rtn = undef;
  if(!&CheckSignMac($PATH))
  {}
  elsif(-f $PATH && ! -d $PATH)
  {
    $rtn = !&_signOneFile($PATH);
  }
  elsif(!open(FIN,"find \"${PATH}\" -type f -print|"))
  {
    print "Cannot run find command on ${PATH}";
  }
  else
  {
    my $line;
    my $nRtn = 1;
    while($line = <FIN>)
    {
      chomp $line;
      (-f $line) &&
        ($nRtn = &_signOneFile($line));
      last if $nRtn;
    }
    close FIN;
    $rtn = !$nRtn;
  }
  return $rtn;
}

sub SignAll
{
  my $r = 1;
  for my $x (glob("{$TOP}/*"))
  {
    if($x =~ m/\.app$/i)
    {
      &SignAppMac($x) || ($r = 0);
    }
    else
    {
      &SignAppMacFiles($x) || ($r = 0);
    }
  }
  $r;
}

my $ok = &SkipSign() ? 1 : &CheckEnv() && &SignAll() && 1;
exit($ok ? 0 : 1);
