#include "boot.h"
#include "Player.h"

#include <locale.h>
#include <signal.h>
#include <stdarg.h>

Cfg_PLAYER_t Cfg_PLAYER;

void *Players_mtx;
player_t *Players[Player_Max];
bool main_stop;
char Start_Time[20];
ushort Start_Count;
char *SERVER_SIGN;
char *SERVER_COPYRIGHT;

//#define NO_ERROR
//#define NO_ERROR_FILE

#ifndef NO_ERROR_FILE
FILE * FError = NULL;
#else
#define FError stderr
#endif

char exiting(char ret)
{
	DEBUG(NULL);
	Network_Close();
	Add_Item_Close();
	SQL_Close();
	Cfg_Clean();
	OpCodes_Close();
	Mutex_Close(Players_mtx);
	close_NPC();
	close_Mounts();
	close_Broadcast();
	if (FError && FError != stderr) fclose(FError);
	return ret;
}

void intHandler()
{
	if (main_stop) return;
	main_stop = true;
	Network_Stop();
	exiting(0);
}

void DEBUG(const char *__msg, ...)
{
	if (!FError) return;
	if (!__msg) {
		fprintf(FError, "\n");
		return;
	}
	if (FError != stderr) {
		time_t rawtime;
		struct tm * t_info;
		time ( &rawtime );
		t_info = localtime ( &rawtime );
		fprintf(FError, "[%04i-%02i-%02i %02i:%02i:%02i] ", t_info->tm_year+1900, t_info->tm_mon+1, t_info->tm_mday, t_info->tm_hour, t_info->tm_min, t_info->tm_sec);
	}
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vfprintf(FError, __msg, __local_argv );
	va_end(__local_argv );
	fflush(FError);
}

void DEBUG2(const char *__msg, ...)
{
	if (!FError) return;
	va_list __local_argv;
	va_start(__local_argv, __msg );
	vfprintf(FError, __msg, __local_argv );
	va_end(__local_argv );
	fflush(FError);
}

int main(int argc, const char **argv)
{
	if (Cfg_Init())
		return exiting(1);

	char req[125];
	time_t rawtime;
	struct tm * t_info;
	time ( &rawtime );
	t_info = localtime ( &rawtime );
	char filename[strlen(Cfg_Server_DirSave)+30];
	sprintf(filename, "%sLogs/%04i-%02i-%02i_%02i:%02i:%02i.log", Cfg_Server_DirSave, t_info->tm_year+1900, t_info->tm_mon+1, t_info->tm_mday, t_info->tm_hour, t_info->tm_min, t_info->tm_sec);
	if (FError != stderr) {
#ifndef NO_ERROR
		FError = fopen(filename, "w");
		if (!FError) {
			fprintf(stderr, "%s (%i) :: Can't open \"%s\" in write mode !\n", __func__, __LINE__, filename);
			fflush(stderr);
//			return 1;
		}
#endif
	}	

	main_stop = false;
	if (argc < 2) {
		DEBUG("Give me my ID !\n\t%s 1\n", argv[0]);
		fprintf(stderr, "Give me my ID !\n\t%s 1\n", argv[0]);
		fflush(stderr);
		return 1;
	}
	Cfg_Server_ID = atoi(argv[1]);
#ifndef WIN32
	setlocale(LC_ALL, "");
#endif
	signal(SIGINT, intHandler);
	srand (time(NULL));

	init_HP_Mod();
	Players_mtx = Mutex_Init();

	DEBUG("%s (%i) :: Init Ok.\n", __func__, __LINE__);

	if (OpCodes_Init())
		return exiting(1);

	if (Cfg_DataBase_Fast_Thread < SQL_MIN_THREAD && Cfg_DataBase_Slow_Thread < 1) {
		DEBUG("%s (%i) :: No enough SQL Thread (min:%i, Cfg:%i) (slow: min:1, Cfg:%i) !\n", __func__, __LINE__,
			SQL_MIN_THREAD, Cfg_DataBase_Fast_Thread, Cfg_DataBase_Slow_Thread);
		return 1;
	}

	init_Broadcast();
	Network_Init();

	if (SQL_Init(Cfg_DataBase_IP, Cfg_DataBase_Port, Cfg_DataBase_Login, Cfg_DataBase_Pass,
	    Cfg_DataBase_Name, Cfg_DataBase_Fast_Thread, Cfg_DataBase_Slow_Thread) < SQL_MIN_THREAD)
		return exiting(1);

	Add_Item_Init();

	time ( &rawtime );
	t_info = localtime ( &rawtime );
	sprintf(Start_Time, "%04i-%02i-%02i %02i:%02i:%02i", t_info->tm_year+1900, t_info->tm_mon+1, t_info->tm_mday, t_info->tm_hour, t_info->tm_min, t_info->tm_sec);
	sprintf(req, "UPDATE `Server_Stats` SET `Start_Time`='%ld', `Start_Count`=`Start_Count`+1, `Player_Count`='-1' WHERE `Server_ID`='%i';",
		rawtime, Cfg_Server_ID);
	SQL_Query_Fast(req, 0);

	sprintf(req, "SELECT `Start_Count` FROM `Server_Stats` WHERE `Server_ID`='%i';", Cfg_Server_ID);
	Start_Count = SQL_int(req);

	SQL_Query_Slow2("DELETE FROM `Server_Used_Character` WHERE `Server_ID`='%i'", Cfg_Server_ID);

	const char *Cfg_main_SQL = "SELECT `Value` FROM `Server_Cfg` WHERE `Key`='%s' AND `Server_ID` IN (0, %i) ORDER BY `Server_ID` DESC;";

	sprintf(req, Cfg_main_SQL, "Name", Cfg_Server_ID);
	Cfg_Server_Name = SQL_rechar(req, Cfg_Server_Name);

	sprintf(req, Cfg_main_SQL, "Port", Cfg_Server_ID);
	Cfg_Server_Port = SQL_int(req);
	if (!Cfg_Server_Port) Cfg_Server_Port = 14752;

	sprintf(req, Cfg_main_SQL, "PassLength", Cfg_Server_ID);
	Cfg_PLAYER.PassLength = SQL_short(req);
	if (!Cfg_PLAYER.PassLength) Cfg_PLAYER.PassLength = 32;

	sprintf(req, Cfg_main_SQL, "PLayer_SUBID", Cfg_Server_ID);
	Cfg_PLAYER.PLayer_SUBID = SQL_short(req);
	if (!Cfg_PLAYER.PLayer_SUBID) Cfg_PLAYER.PLayer_SUBID = 1000;

	sprintf(req, Cfg_main_SQL, "NPC_FRENDLY_SUBID", Cfg_Server_ID);
	Cfg_PLAYER.NPC_FRENDLY_SUBID = SQL_short(req);
	if (!Cfg_PLAYER.NPC_FRENDLY_SUBID) Cfg_PLAYER.NPC_FRENDLY_SUBID = 2000;

	sprintf(req, Cfg_main_SQL, "NPC_MONSTERS_SUBID", Cfg_Server_ID);
	Cfg_PLAYER.NPC_MONSTERS_SUBID = SQL_short(req);
	if (!Cfg_PLAYER.NPC_MONSTERS_SUBID) Cfg_PLAYER.NPC_MONSTERS_SUBID = 3000;

	sprintf(req, Cfg_main_SQL, "Stamina4spd", Cfg_Server_ID);
	Cfg_PLAYER.Stamina4spd = SQL_short(req);

	sprintf(req, Cfg_main_SQL, "newHP", Cfg_Server_ID);
	Cfg_PLAYER.newHP = SQL_short(req);

	sprintf(req, Cfg_main_SQL, "AutoAccount", Cfg_Server_ID);
	Cfg_PLAYER.AutoAccount = SQL_short(req);

	sprintf(req, Cfg_main_SQL, "HardwareHistory", Cfg_Server_ID);
	Cfg_PLAYER.HardwareHistory = SQL_short(req);

	sprintf(req, Cfg_main_SQL, "SERVER_SIGN", Cfg_Server_ID);
	SERVER_SIGN = strdump("<!-- Signature from team of this server -->");
	SERVER_SIGN = SQL_rechar(req, SERVER_SIGN);

	sprintf(req, Cfg_main_SQL, "SERVER_COPYRIGHT", Cfg_Server_ID);
	SERVER_COPYRIGHT = strdump("<!-- Copyright from team of this server -->");
	SERVER_COPYRIGHT = SQL_rechar(req, SERVER_COPYRIGHT);

//	sprintf(req, Cfg_main_SQL, "UsleepTime", Cfg_Server_ID);
//	Cfg_Server_Usleep_Time = SQL_short(req);
	Cfg_Server_Usleep_Time = 100;

	DEBUG(NULL);
	init_Mounts();
	init_Items();
	init_NPC();
	DEBUG(NULL);

	SQL_Query_Slow2("UPDATE `Server_Stats` SET `Player_Count`='0' WHERE `Server_ID`='%i';", Cfg_Server_ID);

	if (Network_Start(Cfg_Server_Port))
		return exiting(1);

	for (int i=0; i<Player_Max; i++) Players[i] = NULL;
	DEBUG(NULL);
	DEBUG("Welcome on \"%s\" (ver. %s)\nServer Name: \"%s\"\nReady to work !\n\n", SYSTEM_NAME, SYSTEM_VERSION, Cfg_Server_Name);

	Network_Run();
	DEBUG(NULL);

	if (main_stop) usleep(3);
	else
		return exiting(0);
	return 0;
}
