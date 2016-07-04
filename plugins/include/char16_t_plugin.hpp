#ifndef _CHAR16_T_PLUGIN_H_
#define _CHAR16_T_PLUGIN_H_

extern size_t	(*strlen16)(const char16_t *src);

extern short	(*strcnmp16)(const char16_t *src1, const char16_t *src2, size_t len);
extern short	(*strcmp16)(const char16_t *src1, const char16_t *src2);

extern char16_t*	(*strstr16)(const char16_t *src1, const char16_t *src2);
extern char16_t*	(*strrstr16)(const char16_t *src1, const char16_t *src2);

extern size_t		(*strncpy16)(char16_t *dst, const char16_t *src, size_t len);
extern size_t		(*strcpy16)(char16_t *dst, const char16_t *src);
extern size_t		(*strncpy8_16)(char16_t *wc, const char *c, size_t len);
extern size_t		(*strcpy8_16)(char16_t *wc, const char *c);
extern size_t		(*strncpy16_8)(char *c, const char16_t *wc, size_t len);
extern size_t		(*strcpy16_8)(char *c, const char16_t *wc);

extern size_t		(*str_strlen)(const char16_t *wc);

extern char*		(*str_str16_8)(const char16_t *wc);
extern char16_t*	(*str_n_str8_16)(const char *c, size_t *size);
extern char16_t*	(*str_str8_16)(const char *c);

#endif // _CHAR16_T_PLUGIN_H_
