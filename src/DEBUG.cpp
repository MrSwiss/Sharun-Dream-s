#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <ctime>

#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "data_type.hpp"
#include "utils.hpp"

#ifndef NO_ERROR_FILE
FILE * FError = NULL;
#else
#define FError stderr
#endif

void DEBUG(const char *__msg, ...)
{
	if (!FError) return;
	if (!__msg) {
		fprintf(FError, "\n");
		return;
	}
	if (FError != stderr) {
		time_t rawtime;
		struct tm * t_info;
		time ( &rawtime );
		t_info = localtime ( &rawtime );
		fprintf(FError, "[%04i-%02i-%02i %02i:%02i:%02i] ", t_info->tm_year+1900, t_info->tm_mon+1, t_info->tm_mday, t_info->tm_hour, t_info->tm_min, t_info->tm_sec);
	}
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vfprintf(FError, __msg, __local_argv );
	va_end(__local_argv );
	fflush(FError);
}

void DEBUG2(const char *__msg, ...)
{
	if (!FError) return;
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vfprintf(FError, __msg, __local_argv );
	va_end(__local_argv );
	fflush(FError);
}
