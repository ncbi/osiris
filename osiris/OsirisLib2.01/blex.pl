
sub fix
{
  my ($infile,$outfile) = @_;
  open(FIN,"<${infile}") || die("Cannot open input file ${infile}");
  open(FOUT,">${outfile}") || die("Cannot open output file ${outfile}");
  my $line;
  $\ = "\n";
  while($line = <FIN>)
  {
    $line =~ s/[\r\n]+$//;
    $line =~ s/\r//g;
    print FOUT $line;
  }
  close FIN;
  close FOUT;
}

open L, "SmartExpressionExpr.l";
open CPP, "ExpressionLex.cpp";
open YACC, "ExpressionYacc.cpp";
open Y, "SmartExpressionExpr.y";
open PARSER, "ExpressionParser.cpp";
my $path = "C:/Program Files (x86)/cygwin/bin/";
my $flex = "\"${path}flex.exe\"";
my $bison = "\"${path}bison.exe\"";

if ((!-e PARSER) || (-M Y < -M PARSER) || (-M L < -M PARSER) || (!-e CPP) || (-M L < -M CPP)) {

	close CPP;
	close YACC;
	system "${flex} -oExpressionLex.cpp -PExpr SmartExpressionExpr.l";
	system "${bison} -d -pExpr -oExpressionYacc.cpp SmartExpressionExpr.y";
	close PARSER;
	system "copy /y ExpressionYacc.cpp + ExpressionLex.cpp  _ExpressionParser.cpp";
        &fix("_ExpressionParser.cpp","ExpressionParser.cpp");

} elsif ((!-e YACC) || (-M Y < -M YACC)) {

	close CPP;
	close YACC;
	system "${bison} -d -pExpr -oExpressionYacc.cpp SmartExpressionExpr.y";
	close PARSER;
	system "copy /y ExpressionYacc.cpp + ExpressionLex.cpp  _ExpressionParser.cpp";
        &fix("_ExpressionParser.cpp","ExpressionParser.cpp");

} else {

	close CPP;
	close YACC;
	close PARSER;
}

close L;
