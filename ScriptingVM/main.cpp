#include "VM/VM.h"
#include "Compiler/Compiler.h"

#include <iostream>
#include <stdarg.h>
#include "ILTextWriter.h"

extern "C" int yywrap(void) {
	return 1;
}
extern FILE* yyin;

//YYSTYPE yylval; // union in which strings are passed

int lineno = 1;
int errors = 0;

// Function used to report errors
void Error(const char *format, ...) 
{
	va_list args;

	errors++;
	printf("Line %d: ", lineno);
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	printf("\n");
}

// Show an error count
void ErrorSummary() {
	printf("%d error(s) were found.\n", errors);
}

// Function called by the parser when an error occurs while parsing
// (parse error or stack overflow)
void yyerror(const char *msg) {
	Error(msg);
}

int main(int argc, char** argv)
{
	Compiler compiler;
	ILByteCode* code = compiler.CompileScript("test.exo");
	if(code == nullptr)
	{
		std::cin.get();
		return 0;
	}

	ILTextWriter ilWriter;
	ilWriter.Open("test.exo.il");
	ilWriter.WriteIL(code);
	ilWriter.Close();

	VM* vm = new VM();
	vm->ExecuteByteCode(code);
	
	std::cin.get();
	
    return 0;
}

