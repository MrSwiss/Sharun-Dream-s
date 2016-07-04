#ifndef _CHAR16_T_HPP_
#define _CHAR16_T_HPP_

#include <cstdio>
#include <cstdlib>

#ifdef DLL_VERSION
#include "char16_t_plugin.hpp"
#else
extern "C" {

size_t	strlen16(const char16_t *src);

short	strcnmp16(const char16_t *src1, const char16_t *src2, size_t len);
short	strcmp16(const char16_t *src1, const char16_t *src2);

char16_t*	strstr16(const char16_t *src1, const char16_t *src2);
char16_t*	strrstr16(const char16_t *src1, const char16_t *src2);

size_t		strncpy16(char16_t *dst, const char16_t *src, size_t len);
size_t		strcpy16(char16_t *dst, const char16_t *src);
size_t		strncpy8_16(char16_t *wc, const char *c, size_t len);
size_t		strcpy8_16(char16_t *wc, const char *c);
size_t		strncpy16_8(char *c, const char16_t *wc, size_t len);
size_t		strcpy16_8(char *c, const char16_t *wc);

size_t		str_strlen(const char16_t *wc);

char*		str_str16_8(const char16_t *wc);
char16_t*	str_n_str8_16(const char *c, size_t *size);
char16_t*	str_str8_16(const char *c);

}
#endif // DLL_VERSION

#endif // _CHAR16_T_HPP_
