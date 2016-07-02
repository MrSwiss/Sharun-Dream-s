#include <char16_t.hpp>
#include <cstring>

size_t strlen16(const char16_t *src)
{
	if (!src)
		return -1;
	char16_t *tmp = (char16_t*)src;
	for (; *tmp; tmp++) ;
	return tmp - src;
}

short strcnmp16(const char16_t *src1, const char16_t *src2, size_t len)
{
	if (!src1)
		return 1;
	if (!src2)
		return 2;
	if (!len)
		return 3;
	size_t len1 = strlen16(src1);
	size_t len2 = strlen16(src2);
	if (len > len1)
		return 1;
	if (len > len2)
		return 2;
	return memcmp(src1, src2, len);
}

short strcmp16(const char16_t *src1, const char16_t *src2)
{
	if (!src1)
		return 1;
	if (!src2)
		return 2;
	size_t len1 = strlen16(src1);
	size_t len2 = strlen16(src2);
	if (len1 > len2)
		return 1;
	if (len1 < len2)
		return 2;
	return memcmp(src1, src2, len1+1);
}

char16_t * strstr16(const char16_t *src1, const char16_t *src2)
{
	if (!src1)
		return NULL;
	if (!src2)
		return NULL;
	size_t len1 = strlen16(src1);
	size_t len2 = strlen16(src2);
	if (len1 < len2)
		return NULL;
	char16_t * dst = (char16_t*)src1;
	while (memcmp(dst, src2, len2)) {
		if (dst > src1 + len1)
		    return NULL;
		dst++;
	}
	return dst;
}

char16_t * strrstr16(const char16_t *src1, const char16_t *src2)
{
	if (!src1)
		return NULL;
	if (!src2)
		return NULL;
	size_t len1 = strlen16(src1);
	size_t len2 = strlen16(src2);
	if (len1 < len2)
		return NULL;
	char16_t * dst = (char16_t*)&src1[len1 - len2];
	while (memcmp(dst, src2, len2)) {
		if (dst < src1)
		    return NULL;
		dst--;
	}
	return dst;
}


int utf8_to_unicode_length(const unsigned char *c)
{
  int i=0;
  while (c[0]) {
      /*if ((*c & 0x80) == 0)
          NULL;
      else */if ((*c & 0xe0) == 0xc0)
          c += 1;
      else if ((*c & 0xf0) == 0xe0)
          c += 2;
      else if ((*c & 0xf8) == 0xf0)
          c += 3;

      c++;
      i++;
  }
  return i;
}

char16_t utf8_to_unicode(const unsigned char *c, int *pos)
{
  if ((*c & 0x80) == 0)
      return c[0];
  else if ((*c & 0xe0) == 0xc0) {
    *pos += 1;
    return (((c[0] & 0x1f) << 6) | (c[1] & 0x3f));
  } else if ((*c & 0xf0) == 0xe0) {
    *pos += 2;
    return (((c[0] & 0x0f) << 12) | ((c[1] & 0x3f) << 6) | (c[2] & 0x3f));
  } else if ((*c & 0xf8) == 0xf0) {
    *pos += 3;
    return (((c[0] & 0x07) << 18) | ((c[1] & 0x3f) << 12) | ((c[2] & 0x3f) << 6) | (c[3] & 0x3f));
  } else
    return 0xffff;
}

int unicode_to_utf8_length(const char16_t *c)
{
    int i=0;
    while (c[0]) {
       if (c[0]<(1<<7))// 7 bit Unicode encoded as plain ascii
       {
           i+=1;
           c += sizeof(char16_t) * 1;
       }
       else if (c[0]<(1<<11))// 11 bit Unicode encoded in 2 UTF-8 bytes
       {
           i+=2;
           c += sizeof(char16_t) * 2;
       }
       else if (c[0]<(1<<16))// 16 bit Unicode encoded in 3 UTF-8 bytes
       {
           i+=3;
           c += sizeof(char16_t) * 3;
       }
       else if (c[0]<(1<<21))// 21 bit Unicode encoded in 4 UTF-8 bytes
       {
           i+=4;
           c += sizeof(char16_t) * 4;
       }
    }
    return i;
}

int unicode_to_utf8(char* b, char16_t c)
{
    int i=0;
    if (c<(1<<7))// 7 bit Unicode encoded as plain ascii
    {
        *b++ = c;
        i++;
    }
    else if (c<(1<<11))// 11 bit Unicode encoded in 2 UTF-8 bytes
    {
        *b++ = (c>>6)|0xC0;
        i++;
        *b++ = (c&0x3F)|0x80;
        i++;
    }
    else if (c<(1<<16))// 16 bit Unicode encoded in 3 UTF-8 bytes
        {
        *b++ = (c>>12)|0xE0;
        i++;
        *b++ =  ((c>>6)&0x3F)|0x80;
        i++;
        *b++ =  (c&0x3F)|0x80;
        i++;
    }

    else if (c<(1<<21))// 21 bit Unicode encoded in 4 UTF-8 bytes
    {
        *b++ = (c>>18)|0xF0;
        i++;
        *b++ = ((c>>12)&0x3F)|0x80;
        i++;
        *b++ = ((c>>6)&0x3F)|0x80;
        i++;
        *b++ = (c&0x3F)|0x80;
        i++;
    }
    *b++ = 0;
    return i;
}

size_t strncpy16(char16_t *wc, const char *c, size_t len)
{
  char16_t b;
  size_t size = 0;
  wc[size] = 0;
  for (int i=0; c[i] && size < len-1; i++) {
    b = utf8_to_unicode((unsigned char *)c + i, &i);
    wc[size] = b;
    size++;
    wc[size] = 0;
  }
  return size;
}

size_t strcpy16(char16_t *wc, const char *c)
{
  return strncpy16(wc, c, strlen(c)+1);
}

size_t strncpy16(char *c, const char16_t *wc, size_t len)
{
  size_t size = 0;
  c[size] = 0;
  for (int i=0; wc[i] && size < len-1; i++) {
    size_t isize = unicode_to_utf8(c + size, wc[i]);
    size += isize;
  }
  return size;
}

size_t strcpy16(char *c, const char16_t *wc)
{
  return strncpy16(c, wc, strlen16(wc)+1);
}

size_t strncpy16(char16_t *dst, const char16_t *src, size_t len)
{
  size_t size = -1;
  do {
    size++;
    dst[size] = src[size];
  }while (src[size] && size < len);
  return size;
}

size_t strcpy16(char16_t *dst, const char16_t *src)
{
  return strncpy16(dst, src, strlen16(src)+1);
}

size_t str_strlen(const char16_t *wc)
{
  return unicode_to_utf8_length(wc);
}

char *str_str(const char16_t *wc)
{
  char *c = (char*)malloc(unicode_to_utf8_length(wc)+1);
  int size = 0;
  c[size] = 0;
  for (int i=0; wc[i]; i++) {
    int isize = unicode_to_utf8(c + size, wc[i]);
    size += isize;
  }
  return c;
}

char16_t *str_str(const char *c, size_t *size)
{
  char16_t b;
  char16_t *wc = (char16_t*)malloc(sizeof(char16_t)*(utf8_to_unicode_length((const unsigned char *)c)+1));
  *size = 0;
  for (int i=0; c[i]; i++) {
    b = utf8_to_unicode((const unsigned char *)&c[i], &i);
    wc[*size] = b;
    (*size)++;
    wc[*size] = 0;
  }
  return wc;
}

char16_t *str_str(const char *c)
{
  size_t size_l = 0;
  return str_str(c, &size_l);
}
