#pragma once

extern int lineno;
extern FILE *yyin;  // the input stream

// Function prototype
extern int yylex(void);
