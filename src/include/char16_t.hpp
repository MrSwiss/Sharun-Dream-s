#ifndef _CHAR16_T_HPP_
#define _CHAR16_T_HPP_

#include <cstdio>
#include <cstdlib>

size_t	strlen16(const char16_t *src);

short	strcnmp16(const char16_t *src1, const char16_t *src2, size_t len);
short	strcmp16(const char16_t *src1, const char16_t *src2);

char16_t*	strstr16(const char16_t *src1, const char16_t *src2);
char16_t*	strrstr16(const char16_t *src1, const char16_t *src2);

size_t		strncpy16(char16_t *wc, const char *c, size_t len);
size_t		strcpy16(char16_t *wc, const char *c);
size_t		strncpy16(char *c, const char16_t *wc, size_t len);
size_t		strcpy16(char *c, const char16_t *wc);
size_t		strncpy16(char16_t *dst, const char16_t *src, size_t len);
size_t		strcpy16(char16_t *dst, const char16_t *src);

size_t		str_strlen(const char16_t *wc);

char*		str_str(const char16_t *wc);
char16_t*	str_str(const char *c, size_t *size);
char16_t*	str_str(const char *c);

#endif // _CHAR16_T_HPP_
