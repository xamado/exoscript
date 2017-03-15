%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "ASTNode.h"
	
	#include "Compiler/Lexer.h"

	#define YYDEBUG 1				// Generate debug code; needed for YYERROR_VERBOSE
	#define YYERROR_VERBOSE			// Give a more specific parse error message 

	// Error-reporting function must be defined by the caller
	void Error (char *format, ...);

	// Forward references
	void yyerror (const char *msg);

    ASTBlockNode* gProgramBlock;
%}

%code requires {
	#include "ASTNode.h"
}

%union {
	ASTBlockNode* block;
	ASTStatementNode* statement;
	ASTExpressionNode* expression;
	ASTIdentifierNode* identifier;
	ASTFunctionParametersNode* function_params;
	ASTFunctionArgumentsNode* function_args;
	ASTVariableDeclarationNode* variable;
	std::string* string;
	int token;
	char op;
}

%token <string> TID TINTEGER TDOUBLE
%token <token> T_PRINT
%token <token> ERROR_TOKEN ASSIGN EQUAL
%token <token> T_BLOCK_START T_BLOCK_END T_LPARENTHESIS T_RPARENTHESIS
%token <token> T_COMMA 
%token <op> T_OP_ADD T_OP_SUBSTRACT T_OP_MULTIPLY T_OP_DIVIDE
%token <token> CONCAT END_STMT 

%type <identifier> identifier;
%type <block> program;
%type <block> statements;
%type <block> block;
%type <statement> statement
%type <expression> number;
%type <expression> expression;
%type <op> operators;
%type <function_args> function_args;
%type <function_params> function_params;
%type <variable> variable;
%%

/* ------------------------------------------------------------------
   Yacc grammar rules
   ------------------------------------------------------------------ */

program 
	: statements { gProgramBlock = $1; }
	;

statements
	: statement { $$ = new ASTBlockNode(); $$->GetStatements().push_back($1); }
	| statements statement { $1->GetStatements().push_back($2); }
	| /* empty */
	;

variable
	: identifier identifier									{ $$ = new ASTVariableDeclarationNode(*$1, *$2); }
	;

block
	: T_BLOCK_START statements T_BLOCK_END					{ $$ = $2; }
	| T_BLOCK_START T_BLOCK_END								{ $$ = new ASTBlockNode(); }
	;

function_params
	: /* blank */							{ $$ = new ASTFunctionParametersNode(); }
	| variable								{ $$ = new ASTFunctionParametersNode(); $$->Add(*$1); }
	| function_params T_COMMA variable		{ $1->Add(*$3); }
	;
	
function_args
	: /* blank */							{ $$ = new ASTFunctionArgumentsNode(); }
	| expression							{ $$ = new ASTFunctionArgumentsNode(); $$->Add(*$1); }
	| function_args T_COMMA expression		{ $1->Add(*$3); }
	;

statement
	: END_STMT																	{ puts("Empty statement"); }
	| variable END_STMT
	| identifier identifier T_LPARENTHESIS function_params T_RPARENTHESIS block	{ $$ = new ASTFunctionDeclarationNode(*$1, *$2, *$4, *$6); }
	| identifier identifier ASSIGN expression END_STMT							{ $$ = new ASTVariableDeclarationNode(*$1, *$2, *$4); }
	| identifier ASSIGN expression END_STMT										{ $$ = new ASTAssignmentNode(*$1, *$3); }
	| expression END_STMT														{ $$ = new ASTExpressionStatementNode(*$1); }
	| T_PRINT expression END_STMT												{ $$ = new ASTPrintNode(*$2); }
	;

expression
	: number
	| identifier
	| expression operators expression								{ $$ = new ASTBinaryOperatorNode(*$1, $2, *$3); }
	| identifier T_LPARENTHESIS function_args T_RPARENTHESIS		{ $$ = new ASTFunctionCallNode(*$1, *$3); }
	;
	  	  
identifier
	: TID       { $$ = new ASTIdentifierNode(*$1); }
	;

number 
	: TINTEGER	{ $$ = new ASTIntegerNode(atoi($1->c_str())); delete $1; }
	| TDOUBLE	{ $$ = new ASTDoubleNode(atof($1->c_str())); delete $1; }
	;
	
operators
	: T_OP_ADD
	| T_OP_SUBSTRACT
	| T_OP_DIVIDE
	| T_OP_MULTIPLY
	;
%%

/* ------------------------------------------------------------------
   Additional code (again copied verbatim to the output file)
   ------------------------------------------------------------------ */

