#pragma once

#include <cstdlib>
#include <cstdio>

class Assert
{
public:
	static void Fatal(const char* message, const char* file, long line);
};

#ifdef NDEBUG
#define ASSERT_FATAL(EXPRESSION, MESSAGE) ((void)0)
#else
#define ASSERT_FATAL(EXPRESSION, MESSAGE) ((EXPRESSION) ? (void)0 : Assert::Fatal(#MESSAGE, __FILE__, __LINE__))
#endif
