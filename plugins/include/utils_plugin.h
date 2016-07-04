#ifndef _UTILS_PLUGIN_H_
#define _UTILS_PLUGIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
#endif

#define ulong	uint64_t
#define uint	uint32_t
#define ushort	uint16_t
#define uchar	uint8_t
#define byte	uint8_t
#define Wchar	uint16_t

#ifdef WIN32
	#define socklen_t int
#else
	#define SOCKET int
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
	#define SOCKADDR const struct sockaddr
#endif

extern uint (*l_sizeof)(char *src);
extern char* (*l_malloc)(uint size);
extern void (*l_free)(char *src);
extern char* (*l_realloc)(char *src, uint size);
extern char* (*l_mem2mem)(char *src);

extern char* (*bytedump)(const char *value);
extern char* (*strdump)(const char *value);
extern char* (*strndump)(const char *value, int len);
extern void (*strupdate)(char **src, const char *value);

extern char (*dir_exist)(const char *dirname);
extern char (*file_exist)(const char *filename);
extern size_t (*file_size)(const char *filename);
extern void (*l_mkdir)(const char* dirname);

extern float (*atof2)(char* val);
extern bool (*atob)(char* val);
extern short (*is_atoi)(char* val);

extern long (*microsec)();

extern char* (*l_realpath)(const char * src);

extern void (*hexdump)(const char* function, const char* title, const void *Vbuffer, unsigned long index);

extern void (*DEBUG)(const char*, ...);
extern void (*DEBUG2)(const char*, ...);

#endif // _UTILS_PLUGIN_H_
