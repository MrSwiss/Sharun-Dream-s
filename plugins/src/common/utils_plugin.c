#include "utils_plugin.h"

#ifdef WIN32

void sharun_set_init(void*);

// UTILS WIN32 Adaptator ******************************************************************
uint (*l_sizeof)(char *src);
char* (*l_malloc)(uint size);
void (*l_free)(char *src);
char* (*l_realloc)(char *src, uint size);
char* (*l_mem2mem)(char *src);

char* (*bytedump)(const char *value);
char* (*strdump)(const char *value);
char* (*strndump)(const char *value, int len);
void (*strupdate)(char **src, const char *value);

char (*dir_exist)(const char *dirname);
char (*file_exist)(const char *filename);
size_t (*file_size)(const char *filename);
void (*l_mkdir)(const char* dirname);

float (*atof2)(char* val);
bool (*atob)(char* val);
short (*is_atoi)(char* val);

long (*microsec)();

char* (*l_realpath)(const char * src);

void (*hexdump)(const char* function, const char* title, const void *Vbuffer, unsigned long index);

void (*DEBUG)(const char*, ...);
void (*DEBUG2)(const char*, ...);

// Adaptator Start ************************************************************************
void utils_init(void *Link[])
{
	if (!Link)
		return;
	uint i = 0;

	sharun_set_init(Link[i++]);

	l_sizeof = Link[i++];
	l_malloc = Link[i++];
	l_free = Link[i++];
	l_realloc = Link[i++];
	l_mem2mem = Link[i++];

	bytedump = Link[i++];
	strdump = Link[i++];
	strndump = Link[i++];
	strupdate = Link[i++];

	dir_exist = Link[i++];
	file_exist = Link[i++];
	file_size = Link[i++];
	l_mkdir = Link[i++];

	atof2 = Link[i++];
	atob = Link[i++];
	is_atoi = Link[i++];

	microsec = Link[i++];

	l_realpath = Link[i++];

	hexdump = Link[i++];

	DEBUG = Link[i++];
	DEBUG2 = Link[i++];
}

#endif
