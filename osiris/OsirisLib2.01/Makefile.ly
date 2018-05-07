BISON=/usr/bin/bison
FLEX=/usr/bin/flex
CAT=/bin/cat
RM=/bin/rm

all: ExpressionParserUX.cpp
	

ExpressionParserUX.cpp: ExpressionYacc.cpp ExpressionLex.cpp
	$(CAT) ExpressionYacc.cpp ExpressionLex.cpp > ExpressionParserUX.cpp

ExpressionLex.cpp: SmartExpressionExpr.l Expression.h
	$(FLEX) -oExpressionLex.cpp -PExpr SmartExpressionExpr.l

ExpressionYacc.cpp:  SmartExpressionExpr.y Expression.h
	$(BISON) -d -pExpr -oExpressionYacc.cpp SmartExpressionExpr.y

clean:
	$(RM) -rf ExpressionParserUX.cpp ExpressionYacc.cpp ExpressionLex.cpp
