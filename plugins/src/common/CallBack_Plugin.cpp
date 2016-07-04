#include "Sharun.hpp"

#ifdef WIN32

sharun_t *Sharun;


size_t (*strlen16)(const char16_t*);
short (*strcnmp16)(const char16_t*, const char16_t*, size_t);
short (*strcmp16)(const char16_t*, const char16_t*);
char16_t*(*strstr16)(const char16_t*, const char16_t*);
char16_t*(*strrstr16)(const char16_t*, const char16_t*);
size_t (*strncpy16)(char16_t*, const char16_t*, size_t);
size_t (*strcpy16)(char16_t*, const char16_t*);
size_t (*strncpy8_16)(char16_t*, const char*, size_t);
size_t (*strcpy8_16)(char16_t*, const char*);
size_t (*strncpy16_8)(char*, const char16_t*, size_t);
size_t (*strcpy16_8)(char*, const char16_t*);
size_t (*str_strlen)(const char16_t*);
char*(*str_str16_8)(const char16_t*);
char16_t*(*str_n_str8_16)(const char*, size_t*);
char16_t*(*str_str8_16)(const char*);
uint (*l_sizeof)(char*);
char*(*l_malloc)(uint);
void (*l_free)(char*);
char*(*l_realloc)(char*, uint);
char*(*l_mem2mem)(char*);
char*(*bytedump)(const char*);
char*(*strdump)(const char*);
char*(*strndump)(const char*, int);
void (*strupdate)(char**, const char*);
char (*dir_exist)(const char*);
char (*file_exist)(const char*);
size_t (*file_size)(const char*);
void (*l_mkdir)(const char*);
float (*atof2)(char*);
bool (*atob)(char*);
short (*is_atoi)(char*);
long (*microsec)();
char*(*l_realpath)(const char*);
void (*hexdump)(const char*, const char*, const void*, unsigned long);
void (*DEBUG)(const char*, ...);
void (*DEBUG2)(const char*, ...);


extern "C" {

void CallBack_Plugin(void** Link)
{
	uint i = 0;
	Sharun = (sharun_t*) Link[i++];
	strlen16 = (size_t (*)(const char16_t*)) Link[i++];
	strcnmp16 = (short (*)(const char16_t*, const char16_t*, size_t)) Link[i++];
	strcmp16 = (short (*)(const char16_t*, const char16_t*)) Link[i++];
	strstr16 = (char16_t*(*)(const char16_t*, const char16_t*)) Link[i++];
	strrstr16 = (char16_t*(*)(const char16_t*, const char16_t*)) Link[i++];
	strncpy16 = (size_t (*)(char16_t*, const char16_t*, size_t)) Link[i++];
	strcpy16 = (size_t (*)(char16_t*, const char16_t*)) Link[i++];
	strncpy8_16 = (size_t (*)(char16_t*, const char*, size_t)) Link[i++];
	strcpy8_16 = (size_t (*)(char16_t*, const char*)) Link[i++];
	strncpy16_8 = (size_t (*)(char*, const char16_t*, size_t)) Link[i++];
	strcpy16_8 = (size_t (*)(char*, const char16_t*)) Link[i++];
	str_strlen = (size_t (*)(const char16_t*)) Link[i++];
	str_str16_8 = (char*(*)(const char16_t*)) Link[i++];
	str_n_str8_16 = (char16_t*(*)(const char*, size_t*)) Link[i++];
	str_str8_16 = (char16_t*(*)(const char*)) Link[i++];
	l_sizeof = (uint (*)(char*)) Link[i++];
	l_malloc = (char*(*)(uint)) Link[i++];
	l_free = (void (*)(char*)) Link[i++];
	l_realloc = (char*(*)(char*, uint)) Link[i++];
	l_mem2mem = (char*(*)(char*)) Link[i++];
	bytedump = (char*(*)(const char*)) Link[i++];
	strdump = (char*(*)(const char*)) Link[i++];
	strndump = (char*(*)(const char*, int)) Link[i++];
	strupdate = (void (*)(char**, const char*)) Link[i++];
	dir_exist = (char (*)(const char*)) Link[i++];
	file_exist = (char (*)(const char*)) Link[i++];
	file_size = (size_t (*)(const char*)) Link[i++];
	l_mkdir = (void (*)(const char*)) Link[i++];
	atof2 = (float (*)(char*)) Link[i++];
	atob = (bool (*)(char*)) Link[i++];
	is_atoi = (short (*)(char*)) Link[i++];
	microsec = (long (*)()) Link[i++];
	l_realpath = (char*(*)(const char*)) Link[i++];
	hexdump = (void (*)(const char*, const char*, const void*, unsigned long)) Link[i++];
	DEBUG = (void (*)(const char*, ...)) Link[i++];
	DEBUG2 = (void (*)(const char*, ...)) Link[i++];
}

}

#endif
