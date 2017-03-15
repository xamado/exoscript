#include "Assert.h"

void Assert::Fatal(const char* message, const char* file, long line)
{
	fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", message, file, line);
	abort();
}