#!/usr/bin/perl

use strict 'vars';

my $cmd = 'grep RelativePath *.vcproj | sed "s/.*\\.\\\\//" | sed "s/..$//" |';
open FLIST,$cmd;
my @fList = (<FLIST>);
close FLIST;
my %fh;
my $fn;
for $fn (@fList)
{
  chomp $fn;
  $fh{$fn} = 1;
}

my @files = (glob("*.cpp"),glob("*.h"));
$\ = "\n";
for $fn (@files)
{
  if(!$fh{$fn})
  {
     print $fn;
  }
}
