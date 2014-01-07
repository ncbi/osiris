#!/usr/bin/perl
#
# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#               National Center for Biotechnology Information
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government have not placed any restriction on its use or reproduction.
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
#  Please cite the author in any work or product based on this material.
#
# ===========================================================================
#
#  FileName: f2d.pl
#  Author: Douglas Hoffman
#
#  convert osiris.png to a c header file, osiris_png.h, with binary image data
#  used for the splash and 'about' windows
#
#
use strict 'vars';
use IO::File;
use POSIX;
my $INPUT_FILE =
 (($#ARGV >= 0) && (-r $ARGV[0])) ? $ARGV[0] :  "osiris13b4.png";
my $OUTPUT_FILE =
 (($#ARGV >= 1) && length($ARGV[1])) ? $ARGV[1] :  "osiris_png.h";
my $IMAGE_NAME = ${OUTPUT_FILE};
$IMAGE_NAME =~ s/.h$//;

my $f1 = &OpenInFile($INPUT_FILE);
my $f2 = &OpenOutFile($OUTPUT_FILE);
my $SCRIPT = $0;
my $DATE = POSIX::strftime("%d-%b-%Y",localtime(time));

$SCRIPT =~ s|.*/||g;
$SCRIPT =~ s|.*\\||g;

my $EOL = "\n";

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
  my ($fpIn,$fpOut) = @_;
  my $s;
  my $n;
  my $i;
  my $nOrd;
  my $nWritten = 0;
  my $nTotal = 0;
  my $comma = "";
  my $str =
"/*${EOL}" .
"* ===========================================================================${EOL}" .
"*${EOL}" .
"*                            PUBLIC DOMAIN NOTICE${EOL}" .
"*               National Center for Biotechnology Information${EOL}" .
"*${EOL}" .
"*  This software/database is a \"United States Government Work\" under the${EOL}" .
"*  terms of the United States Copyright Act.  It was written as part of${EOL}" .
"*  the author's official duties as a United States Government employee and${EOL}" .
"*  thus cannot be copyrighted.  This software/database is freely available${EOL}" .
"*  to the public for use. The National Library of Medicine and the U.S.${EOL}" .
"*  Government have not placed any restriction on its use or reproduction.${EOL}" .
"*${EOL}" .
"*  Although all reasonable efforts have been taken to ensure the accuracy${EOL}" .
"*  and reliability of the software and data, the NLM and the U.S.${EOL}" .
"*  Government do not and cannot warrant the performance or results that${EOL}" .
"*  may be obtained by using this software or data. The NLM and the U.S.${EOL}" .
"*  Government disclaim all warranties, express or implied, including${EOL}" .
"*  warranties of performance, merchantability or fitness for any particular${EOL}" .
"*  purpose.${EOL}" .
"*${EOL}" .
"*  Please cite the author in any work or product based on this material.${EOL}" .
"*${EOL}" .
"* ===========================================================================${EOL}" .
"*${EOL}" .
"*  FileName: ${OUTPUT_FILE}${EOL}" .
"*  Created:  ${DATE}${EOL}" .
"*  This file was created with the perl script, ${SCRIPT},${EOL}" .
"*  using the file, ${INPUT_FILE}, for input${EOL}" .
"*  The script was written by Douglas Hoffman${EOL}" .
"*${EOL}" .
"*/${EOL}" .
   "#ifndef __${IMAGE_NAME}_H__${EOL}" .
   "#define __${IMAGE_NAME}_H__${EOL}" .
   "${EOL}static const unsigned char ${IMAGE_NAME}[] = {${EOL}";
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
  $str .= "${EOL}};${EOL}${EOL}#endif${EOL}";
  $fpOut->print($str);
  print STDERR $nTotal,"${EOL}";
  0;
}


&run($f1,$f2);

