#!perl
use GetVersion;
my $INSTALLDIR = 'C:\Apps\User_Installs\Osiris';
my $DIR_CUT = '.*User_Installs.Osiris.';
my $sDir = &GetVersion::GetDirectory();
my $DIFF = 'c:\Apps\User_Installs\Cygwin\bin\diff';
if(! -d $sDir) 
{
  die "Cannot find directory ${sDir}";
}
open FOUT,">mklists.bat";
print FOUT <<EOF;
\@echo off
dir /b/s C:\\Apps\\User_Installs\\Osiris | grep -v "site.." | sed "s/${DIR_CUT}//" > install.txt
dir /b/s ${sDir} | sed "s/.*ZipPerl.//" | sed 's/${sDir}.//' > zip.txt
{$DIFF} install.txt zip.txt
EOF
close FOUT;
0;
