#include "globale.h"
#include "Player.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>

uint l_sizeof(char *src)
{
	if (!src) return 0;
	uint i;
	memcpy(&i, src - sizeof(uint), sizeof(uint)); 
	return i;
}

char *l_malloc(uint size)
{
	char *out = malloc(size + sizeof(uint));
	memcpy(out, &size, sizeof(uint)); 
	return out + sizeof(uint);
}

void l_free(char *src)
{
	free(src - sizeof(uint));
}

char *l_realloc(char *src, uint size)
{
	char *out = realloc(src ? src - sizeof(uint) : NULL, size + sizeof(uint));
	if (!out) {
		out = l_malloc(size);
		memcpy(out, src, l_sizeof(src));
		l_free(src);
	} else out += sizeof(uint);
	memcpy(out - sizeof(uint), &size, sizeof(uint));
	return out;
}

char *l_mem2mem(char *src)
{
	if (!src) return NULL;
	char *out = malloc(l_sizeof(src));
	if (out)
		memcpy(out, src, l_sizeof(src));
	return out;
}

char *bytedump(const char *value)
{
	size_t size = sizeof(value)/sizeof(*value);
	char *dst = malloc(size);
	memcpy(dst, value, size);
	return dst;
}

char *strdump(const char *value)
{
	char *dst = malloc(strlen(value)+1);
	strcpy(dst, value);
	return dst;
}

char *strndump(const char *value, int len)
{
	char *dst = malloc(len+1);
	memset(dst, 0, len+1);
	strncpy(dst, value, len);
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

WCHAR utf8_to_unicode(const unsigned char *c, int *pos)
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

int unicode_to_utf8_length(const WCHAR *c)
{
    int i=0;
    while (c[0]) {
       if (c[0]<(1<<7))// 7 bit Unicode encoded as plain ascii
       {
           i+=1;
           c += sizeof(WCHAR) * 1;
       }
       else if (c[0]<(1<<11))// 11 bit Unicode encoded in 2 UTF-8 bytes
       {
           i+=2;
           c += sizeof(WCHAR) * 2;
       }
       else if (c[0]<(1<<16))// 16 bit Unicode encoded in 3 UTF-8 bytes
       {
           i+=3;
           c += sizeof(WCHAR) * 3;
       }
       else if (c[0]<(1<<21))// 21 bit Unicode encoded in 4 UTF-8 bytes
       {
           i+=4;
           c += sizeof(WCHAR) * 4;
       }
    }
    return i;
}

int unicode_to_utf8(char* b, WCHAR c)
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

int WCHARlen(const WCHAR *wc)
{
  int i = 0;
  while (wc[i]) i++;
  return i;
}

int cpy_WCHAR(WCHAR *wc, const char *c, int len)
{
  WCHAR b;
  int size = 0;
  wc[size] = 0;
  for (int i=0; c[i] && size < len-1; i++) {
    b = utf8_to_unicode((unsigned char *)c + i, &i);
    wc[size] = b;
    size++;
    wc[size] = 0;
  }
  return size;
}

int cpy_WCHAR2(WCHAR *wc, const char *c)
{
  WCHAR b;
  int size = 0;
  wc[size] = 0;
  for (int i=0; c[i]; i++) {
    b = utf8_to_unicode((unsigned char *)c + i, &i);
    wc[size] = b;
    size++;
    wc[size] = 0;
  }
  return size;
}

WCHAR *to_WCHARi(const char *c, int *size)
{
  WCHAR b;
  WCHAR *wc = malloc(sizeof(WCHAR)*(utf8_to_unicode_length((const unsigned char *)c)+1));
  *size = 0;
  for (int i=0; c[i]; i++) {
    b = utf8_to_unicode((const unsigned char *)&c[i], &i);
    wc[*size] = b;
    (*size)++;
    wc[*size] = 0;
  }
  return wc;
}

WCHAR *to_WCHAR(const char *c)
{
  int size;
  return to_WCHARi(c, &size);
}

char *to_CHAR(const WCHAR *wc)
{
  char *c = malloc(unicode_to_utf8_length(wc)+1);
  int size = 0;
  c[size] = 0;
  for (int i=0; wc[i]; i++) {
    int isize = unicode_to_utf8(c + size, wc[i]);
    size += isize;
  }
  return c;
}

int to_CHARi(const WCHAR *wc, char *c)
{
  int size = 0;
  c[size] = 0;
  for (int i=0; wc[i]; i++) {
    int isize = unicode_to_utf8(c + size, wc[i]);
    size += isize;
  }
  return size;
}

char dir_exist(const char *dirname)
{
  struct stat   buffer;   
  if( stat( dirname, &buffer ) != 0 )
    return 0;
#ifdef WIN32
  else if( buffer.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
#else
  else if( S_ISDIR(buffer.st_mode) )
#endif
    return 1;
  return 0;
}

char file_exist(const char *filename)
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

size_t file_size(const char *filename)
{
  struct stat   buffer;   
  stat (filename, &buffer);
  return buffer.st_size;
}

void l_mkdir(const char* dirname)
{
#ifdef WIN32
  mkdir(dirname);
#else
  mkdir(dirname, 0777);
#endif

}

float atof2(char* val)
{
  char *tmp = strchr(val, '.');
  if (tmp) tmp[0] = ',';
  return atof(val);
}

short is_atoi(char* val)
{
  while (val[0]) {
    if ( val[0] != '-'
      && val[0] != '0'
      && val[0] != '1'
      && val[0] != '2'
      && val[0] != '3'
      && val[0] != '4'
      && val[0] != '5'
      && val[0] != '6'
      && val[0] != '7'
      && val[0] != '8'
      && val[0] != '9'
    ) return 0;
    val++;
  }
  return 1;
}

long microsec()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	long microsec = ((unsigned long long)time.tv_sec * 1000000);
	microsec += time.tv_usec;
	return microsec;
}

void hexdump(const char* function, const char* title, const void *Vbuffer, unsigned long index)
{
	const unsigned char *buffer = Vbuffer;
	DEBUG("%s (%i) :: %s :: %s (%ld)\n", __func__, __LINE__, function, title, index);
	for (unsigned long i=0; i<index; i+=16) {
		DEBUG2("[%10ld]\t", i);
		for (int spacer=0; spacer<16; spacer++) {
			if (i+spacer >= index)
				DEBUG2("   ");
			else
				DEBUG2("%02X ", buffer[i+spacer]);
			if ((spacer+1)/4 == (float)(spacer+1)/4)
				DEBUG2(" ");
		}
		DEBUG2("\t: ");
		for (int spacer=0; spacer<16; spacer++) {
			if (i+spacer >= index)
				DEBUG2("   ");
			else {
				if (buffer[i+spacer] < 32) DEBUG2(".");
				else DEBUG2("%c",buffer[i+spacer]);
			}
		}
		DEBUG2("\n");
	}
	DEBUG2("\n");
}
