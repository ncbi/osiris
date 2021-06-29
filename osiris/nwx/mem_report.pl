#!/usr/bin/perl
use strict 'vars';


sub SortCmp
{
  my ($a,$b) = @_;
  my $rtn = ($a->[0] <=> $b->[0]) || ($a->[1] <=> $b->[1]);
  $rtn;
}

sub BuildList
{
  my $line;
  my $number;
  my @list = ();
  my $linenr = 0;
  my $bytes = 0;
  while($line = <FIN>)
  {
    ++$linenr;
    $line =~ s/\s+$//;
    if($line =~ m/^[*]+ MEMORY/)
    {
      $number = $line;
      $number =~ s/^([^{]*\{)(\d+)(\}.*)/\2/;
      $line =~ s/.*MEMORY *//;
      push(@list,[int($number),$linenr, $line]);
    }
    elsif($line =~ m/^\{\d+\}.*normal block/)
    {
      # get alloc number
      $number = substr($line,1);
      $number =~ s/[^0-9].*//;
      push(@list,[int($number), $linenr, $line]);
      # get length
      $number = $line;
      $number =~ s/(.* )(\d+)( bytes.*)/\2/;
      $bytes += int($number)
    }
  }
  my $printList = [];  
  my @rlist = sort { &SortCmp($a,$b); } @list;
  my $prev = shift @rlist;
  for $line (@rlist)
  {
    if(($line->[0] == $prev->[0]) && ($line->[0] =~ m/normal block/))
    {
      $prev->[1] .= "--FOUND";
    }
    push @$printList, $prev->[-1];
    $prev = $line;
  }
  $prev && (push @$printList, $prev->[-1]);
  push @$printList, "${bytes} bytes leaked.";
  $printList;
}

sub DumpList
{
  my $list = shift;
  local $\ = "\n";
  local $, = "\n";
  print FOUT @$list;
}

my ($infile, $outfile) = @ARGV;
((!$infile) || ($infile eq '-')) && ($infile = "&STDIN");
((!$outfile) || ($outfile eq '-')) && ($outfile = "&STDOUT");
open(FIN, "<$infile") || die("Cannot open input file ${infile}");
open(FOUT, ">$outfile")|| die("Cannot open output file ${outfile}");
&DumpList(&BuildList);
0;

