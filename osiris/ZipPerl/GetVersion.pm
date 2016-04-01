#!/usr/local/bin/perl

use strict 'vars';
use IO::File;
package GetVersion;

our $VERSION = undef;
our $got = undef;

sub DIRNAME
{
  ## in unix `basename` is better, but this needs to work in windows
  my $s = shift;
  $s =~ s/[\\\/]?[^\\\/]+$//;
  length($s) || ($s = ".");
  $s;
}

sub Get
{
  if(!$got)
  {
    $got = 1;
    my $r = shift || $0;
    my $s = &DIRNAME($r);
    my $fname = "${s}/../Version/OsirisVersion.h";
    if(-r $fname)
    {
      my $fh = new IO::File("<$fname");
      my $line = 1;
      while( $line && ($line !~ m/ OSIRIS_VERS_BASE /))
      {
        $line = <$fh>;
      }
      if($line)
      {
        chomp $line;
        $line =~ s/^.*OSIRIS_VERS_BASE\s*"//;
        $line =~ s/\".*$//;
        $line =~ s/\s/-/g;
        $VERSION = $line;
      }
    }
  }
  $VERSION;
}
sub GetDirectory
{
  my $s = &Get();
  $s =~ s/ /-/g;
  "Osiris-${s}";
}

if ("/$0" =~ m/[\\\/]GetVersion.pm$/)
{
  my $s = &DIRNAME("${0}");
  print $s,"\n";
  print &Get(),"\n";
  print &GetDirectory(),"\n";
}
1;
