#!/usr/bin/perl
#
#  convert osiris.png to a c header file, osiris_png.h, with binary image data 
#  used for the splash and 'about' windows
#
#
use strict 'vars';
use IO::File;


my $EOL = "\r\n";

sub OpenInFile
{
  my $fnIn = shift;
  my $fn = new IO::File("<${fnIn}");
  $fn || die("Cannot open file ${fnIn}");
  binmode $fn;
  $fn;
}
sub OpenOutFile
{
  my $fnOut = shift;
  my $fn = new IO::File(">${fnOut}");
  $fn || die("Cannot open file ${fnOut}");
  binmode $fn; 
  $fn;
}

sub run
{
  my ($fpIn,$fpOut, $imageName) = @_;
  my $n;
  my $i;
  my $s;
  my $nOrd;
  my $nWritten = 0;
  my $nTotal = 0;
  my $comma = "";
  my $str =
    "${EOL}static const unsigned char ${imageName}[] = {${EOL}";
  while ($n = $fpIn->read($s,4096))
  {
    for($i = 0; $i < $n; $i++)
    {
      $str .= $comma;
      $comma = ", ";
      if($nWritten >= 16)
      {
        $str .= "${EOL}";
        $fpOut->print($str);
        $str = "";
        $nWritten = 0;
      }
      $nOrd = ord(substr($s,$i,1));
      $str .= $nOrd;
      $nWritten++;
      $nTotal++;
    }
  }
  $str .= "${EOL}};${EOL}${EOL}";
  $fpOut->print($str);
  0;
}

my $top = "#ifndef __C_MRU_LIST_ICONS_H__${EOL}#define __C_MRU_LIST_ICONS_H__${EOL}${EOL}";
my $bottom = "${EOL}${EOL}#endif${EOL}";

my $fOut = OpenOutFile("CMruListIcons.h");
$fOut->print($top);

my $fIn = &OpenInFile("iconBatch.png");
&run($fIn,$fOut,"iconBatch");

$fIn = &OpenInFile("iconTable.png");
&run($fIn,$fOut,"iconTable");

$fIn = &OpenInFile("iconPlot.png");
&run($fIn,$fOut,"iconPlot");
$fOut->print($bottom);

