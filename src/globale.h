#ifndef __GLOBALE_H__
#define __GLOBALE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <wchar.h>
#include <time.h>

#include "c_Thread.h"

#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
	#ifndef sleep
		#define sleep(a) Sleep(a * 1000)
	#endif
#else
	#include <pthread.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#define WINAPI
	#define SOCKET int
	#define closesocket close
	#define SOCKADDR const struct sockaddr
#endif

#ifndef BOOL
	#define BOOL bool
#endif
#ifndef TRUE
	#define TRUE true
#endif
#ifndef FALSE
	#define FALSE false
#endif
//#ifndef ushort
	#define ushort uint16_t
//#endif
//#ifndef uint
	#define uint uint32_t
//#endif
//#ifndef ulong
	#define ulong uint64_t
//#endif
//#ifndef uchar
	#define uchar uint8_t
//#endif
//#ifndef byte
	#define byte uint8_t
//#endif
//#ifndef wchar
	#define WCHAR uint16_t
//#endif

#define DEFAULT_SERVER_NAME "Sharun Dream's"
#define SYSTEM_NAME "Sharun Dream's"
#define SYSTEM_VERSION L_VERSION

#define MAX_BYTE	3
#define MAX_USHORT	5
#define MAX_UINT	10
#define MAX_ULONG	20
#define MAX_FLOAT	20

#define LV_Max 120
#define walk_speed 40
#define DB_detail_1 37
#define Confirm_Time 1
#define SQL_MIN_THREAD 1
#define Charcters_Max_Per_Account 15
#define Player_Max 100//99999999 //4294967295
#define Packet_Max_Size 0xffff


#include "NetLink.h"
#include "Stat.h"
#include "ZONES.h"
#include "CHAR_LOC.h"
#include "Class.h"
#include "Name_Color.h"
#include "Other.h"
#include "Race.h"
#include "Slot_Type.h"
#include "Stuff.h"
#include "Move_Type.h"
#include "Broadcast_Type.h"
#include "Chat_Type.h"
#include "Mount.h"
#include "NPC.h"
#include "Items.h"

#include "p_buffer.h"

typedef struct Cfg_PLAYER_t {
	short	PassLength;
	int	PLayer_SUBID;
	int	NPC_FRENDLY_SUBID;
	int	NPC_MONSTERS_SUBID;
	bool	Stamina4spd;
	bool	newHP;
	bool	AutoAccount;
	bool	HardwareHistory;
} Cfg_PLAYER_t;

extern ulong PLAYER_COUNT;
extern char  Start_Time[20];
extern ushort Start_Count;
extern char *SERVER_SIGN;
extern char *SERVER_COPYRIGHT;
extern short Cfg_Server_ID;
extern char *Cfg_Server_Name;
extern short Cfg_Server_Port;
extern short Cfg_Server_Usleep_Time;
extern char *Cfg_Server_DirSave;
extern short Cfg_Broadcast_Max_Thread;

extern Cfg_PLAYER_t Cfg_PLAYER;

typedef struct PACKET_h {
	ushort Size;
	ushort Code;
} PACKET_h;

void	DEBUG(const char *__msg, ...);
void	DEBUG2(const char *__msg, ...);

void	NetLink_close(netlink_t *);

uint	l_sizeof(char *);
char*	l_malloc(uint );
void	l_free(char *);
char*	l_realloc(char *, uint);
char*	l_mem2mem(char *);
char*	bytedump(const char *);
char*	strdump(const char *);
char*	strndump(const char *, int);
int	WCHARlen(const WCHAR *);
int	cpy_WCHAR(WCHAR *, const char *, int);
int	cpy_WCHAR2(WCHAR *, const char *);
WCHAR*	to_WCHARi(const char *, int *);
WCHAR*	to_WCHAR(const char *);
char*	to_CHAR(const WCHAR*);
int	to_CHARi(const WCHAR *wc, char *c);
char	dir_exist(const char *);
char	file_exist(const char *);
size_t	file_size(const char *);
float	atof2(char*);
short	is_atoi(char*);
long	microsec();
char*	l_realpath(char*);
void	hexdump(const char* function, const char* title, const void *buffer, unsigned long index);

uint	SQL_Max_Row(void *);
void	SQL_Seek(void *, ulong);
void*	SQL_Next_Row(void *);
void	SQL_Result_Clear(void *);
void	SQL_toFloat(char *);
void*	SQL_Query_Fast(char *, char);
void*	SQL_Query_Fast2(char *, char, ...);
void	SQL_Query_Slow(char *);
void	SQL_Query_Slow2(char *, ...);

short	SQL_short(char *);
void	SQL_short2(char *, short *);
int	SQL_int(char *);
void	SQL_int2(char *, int *);
long	SQL_long(char *);
void	SQL_long2(char *, long *);
char*	SQL_char(char *);
void	SQL_charcpy(char *, char *, int);
char*	SQL_freechar(char *, char *);
char*	SQL_rechar(char *, char *);

int	SQL_Check_Char(char *);
void	SQL_Format_Char(char *);

void*	Mutex_Init();
void	Mutex_Close(void *);
void	Mutex_Lock(void *);
void	Mutex_Unlock(void *);
void	Mutex_Pause(void *);
void	Mutex_SPause(void *);
void	Mutex_Resume(void *);

#endif // __GLOBALE_H__
