cd ..
@echo on
c:\cygwin\bin\flex rgschemascan.l
c:\cygwin\bin\bison -d --skeleton=yaccmod.c rgschemascan.y
copy rgschemascan.tab.c+lex.yy.c rgschema.cpp /b
