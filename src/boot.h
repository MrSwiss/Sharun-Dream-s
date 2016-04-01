#ifndef __BOOT_H__
#define __BOOT_H__

#include "globale.h"

extern char *Cfg_Server_Name;
extern short Cfg_Server_Port;
extern char *Cfg_DataBase_IP;
extern short Cfg_DataBase_Port;
extern char *Cfg_DataBase_Login;
extern char *Cfg_DataBase_Pass;
extern char *Cfg_DataBase_Name;
extern short Cfg_DataBase_Fast_Thread;
extern short Cfg_DataBase_Slow_Thread;
int Cfg_Init();
void Cfg_Clean();


extern bool		localhost;
void	Network_Init();
int		Network_Start(unsigned short port);
void	Network_Run();
void	Network_Stop();
void	Network_Close();
void	Network_Test(void*);


void	Add_Item_Init();
void	Add_Item_Close();

int		OpCodes_Init();
void	OpCodes_Close();

void	init_HP_Mod();

void	init_Mounts();
void	close_Mounts();

void	init_NPC();
void	close_NPC();

void	init_Items();
void	close_Items();

void	init_Broadcast();
void	close_Broadcast();

int		SQL_Init(const char *host, short port, const char *name, const char *pass, const char *db, short thr, short sthr);
void	SQL_Close();

#endif // __BOOT_H__
