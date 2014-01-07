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
#  FileName: mknotice.pl
#  Author:   Douglas Hoffman
#
#
use strict 'vars';
my $fn;

my $AUTHOR = "Douglas Hoffman";

for $fn (@ARGV)
{
  (-f $fn) || system("touch ${fn}");
}

open FIN,"grep -L \"PUBLIC DOMAIN NOTICE\" *.cpp *.h|";
my @fList = (<FIN>);
close FIN;
my $fnTmp;
my $notice;
my $line;

for $fn (@fList)
{
  chomp $fn;
  $notice = <<EOF;
/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: ${fn}
*  Author:   ${AUTHOR}
*
*/
EOF

   $notice =~ s/\r\n/\n/gs;
   print "${fn}\n";
   $fnTmp = "${fn}_";
   if( -f $fnTmp ) { unlink $fnTmp; }
   rename($fn,$fnTmp) || die ("Cannot rename ${fn}\n");
   open FOUT,">${fn}";
   open FIN,"<${fnTmp}";
   binmode FOUT;
   print FOUT $notice;
   while ($line = <FIN>)
   {
      $line =~ s/\r\n/\n/gs;
      print FOUT $line;
   }
   close FIN;
   close FOUT;
}
