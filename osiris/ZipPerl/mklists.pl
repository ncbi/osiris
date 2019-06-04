#!perl
use lib '.';
use strict 'vars';
use GetVersion;
my $sDir = &GetVersion::GetDirectory();
my $INSTALLDIR = "C:\\Apps\\User_Installs\\{$sDir}";
my $UNIXINSTALL = "/Apps/User_Installs/${sDir}";
my $DIR_CUT = '.*User_Installs.Osiris.';
if(! -d $sDir) 
{
  die "Cannot find directory ${sDir}";
}

sub dump
{
  my ($s,$filename,$eol) = @_;
  my @aa = split /\n/,$s;
  for my $i (0 .. $#aa)
  {
    $aa[$i] =~ s/\s+$//;
    $aa[$i] .= $eol;
  }
  open FOUT,">${filename}";
  binmode FOUT;
  print FOUT @aa;
  close FOUT;
}


my $sOUTBAT = <<EOF;
\@echo off
bash mklists.sh
EOF


my $sOUTSH = <<EOF;
#!/bin/bash
if test -d "/c"; then
  DRIVE="/c"
elif test -d "/cygdrive/c"; then
  DRIVE="/cygdrive/c"
else
  echo "Cannot find c drive"
  exit 1
fi
export PATH="/bin:/usr/bin:\${PATH}"
find "\${DRIVE}${UNIXINSTALL}" | grep -v "/site.." | grep -v "^\${DRIVE}${UNIXINSTALL}"'\$' | sed "s|.*\${DRIVE}${UNIXINSTALL}.||" > install.txt
find "${sDir}" | grep -v "^${sDir}"'\$' | sed 's/.*${sDir}.//' > zip.txt
diff install.txt zip.txt
EOF

&dump($sOUTBAT,"mklists.bat","\r\n");
&dump($sOUTSH,"mklists.sh","\n");
0;