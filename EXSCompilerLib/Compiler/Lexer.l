%{
#include <stdio.h>
#include <stdlib.h>

#include "Parser.tab.h"

extern "C" int yywrap();

#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define SAVE_OPERATOR yylval.op = yytext[0]
%}

%%

"print"					return T_PRINT;
[ \t\n]                 ;
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TID;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
[0-9]+.[0-9]+           SAVE_TOKEN; return TDOUBLE;
"="						return ASSIGN;
"=="					return EQUAL;
"+"						SAVE_OPERATOR; return T_OP_ADD;
"-"						SAVE_OPERATOR; return T_OP_SUBSTRACT;
"*"						SAVE_OPERATOR; return T_OP_MULTIPLY;
"/"						SAVE_OPERATOR; return T_OP_DIVIDE;
";"						return END_STMT;
"("						return T_LPARENTHESIS;
")"						return T_RPARENTHESIS;
"{"						return T_BLOCK_START;
"}"						return T_BLOCK_END;
","						return T_COMMA;
.						{ printf("Unknown token: %s\n", yytext); yyterminate(); }

%%
