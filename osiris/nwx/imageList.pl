#!/usr/bin/perl
#
#  convert png images to a c header file with binary image data 
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

my $top = "#ifndef __C_ARROW_IMAGES_H__${EOL}#define __C_ARROW_IMAGES_H__${EOL}${EOL}";
my $bottom = "${EOL}${EOL}#endif${EOL}";

my $fOut = OpenOutFile("CArrowImages.h");
$fOut->print($top);
my $fIn = &OpenInFile("uarr.png");
&run($fIn,$fOut,"sUarr");
$fIn = &OpenInFile("darr.png");
&run($fIn,$fOut,"sDarr");
$fIn = &OpenInFile("empty.png");
&run($fIn,$fOut,"sEmpty");
$fOut->print($bottom);

