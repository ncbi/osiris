
all: ExpressionParserUX.cpp
	

ExpressionParserUX.cpp: ExpressionYacc.cpp ExpressionLex.cpp
	cat ExpressionYacc.cpp ExpressionLex.cpp > ExpressionParserUX.cpp

ExpressionLex.cpp: SmartExpressionExpr.l Expression.h
	flex -oExpressionLex.cpp -PExpr SmartExpressionExpr.l

ExpressionYacc.cpp:  SmartExpressionExpr.y Expression.h
	bison -d -pExpr -oExpressionYacc.cpp SmartExpressionExpr.y

clean:
	rm -rf ExpressionParserUX.cpp ExpressionYacc.cpp ExpressionLex.cpp
