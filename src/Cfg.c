#include "globale.h"

short Cfg_Server_ID;
char *Cfg_Server_Name;
short Cfg_Server_Port;
short Cfg_Server_Usleep_Time;
char *Cfg_Server_DirSave;
char *Cfg_DataBase_IP;
short Cfg_DataBase_Port;
char *Cfg_DataBase_Login;
char *Cfg_DataBase_Pass;
char *Cfg_DataBase_Name;
short Cfg_DataBase_Fast_Thread;
short Cfg_DataBase_Slow_Thread;
short Cfg_Broadcast_Max_Thread;

int Cfg_Init()
{
	Cfg_Server_Name		= strdump(DEFAULT_SERVER_NAME);
	Cfg_Server_DirSave	= strdump("Sharun_Saves/");
	Cfg_Server_Port		= 14752;
	Cfg_DataBase_IP		= NULL;
	Cfg_DataBase_Port	= 3306;
	Cfg_Server_Usleep_Time  = 1;
	Cfg_DataBase_Login	= NULL;
	Cfg_DataBase_Pass	= NULL;
	Cfg_DataBase_Name	= NULL;
	Cfg_DataBase_Fast_Thread	= 1;
	Cfg_DataBase_Slow_Thread	= 1;
	Cfg_Broadcast_Max_Thread	= 1;

	FILE *fp = fopen("config.ini", "r");
	if (fp) {
		while (!feof(fp)) {
			char lib[20], val[1024];
			lib[0] = 0;
			val[0] = 0;
			fscanf(fp, "%s = %[^\n]s", lib, val);
			char *rnm = strstr(val, "\r");
			if (rnm) rnm[0] = 0;
			rnm = strstr(val, "\n");
			if (rnm) rnm[0] = 0;
			if (!strcmp(lib, "DirSave") && strlen(val)) {
				free(Cfg_Server_DirSave);
				Cfg_Server_DirSave = strdump(val);
			}
			if (!strcmp(lib, "DataBase_IP") && strlen(val)) Cfg_DataBase_IP = strdump(val);
			if (!strcmp(lib, "DataBase_Port")) Cfg_DataBase_Port = atoi(val);
			if (!strcmp(lib, "DataBase_Login") && strlen(val)) Cfg_DataBase_Login = strdump(val);
			if (!strcmp(lib, "DataBase_Pass") && strlen(val)) Cfg_DataBase_Pass = strdump(val);
			if (!strcmp(lib, "DataBase_Name") && strlen(val)) Cfg_DataBase_Name = strdump(val);
			if (!strcmp(lib, "DataBase_Fast_Thread")) Cfg_DataBase_Fast_Thread = atoi(val);
			if (!strcmp(lib, "DataBase_Slow_Thread")) Cfg_DataBase_Slow_Thread = atoi(val);
			if (!strcmp(lib, "Broadcast_Max_Thread")) Cfg_Broadcast_Max_Thread = atoi(val);
		}
		fclose(fp);
		DEBUG("%s (%i) :: Config Loaded.\n", __func__, __LINE__);
		return 0;
	}
	DEBUG("%s (%i) :: Config can't be Loaded !\n", __func__, __LINE__);
	return 1;
}

void Cfg_Clean()
{
	free(Cfg_Server_Name);
	Cfg_Server_Name = NULL;
	free(Cfg_Server_DirSave);
	Cfg_Server_DirSave = NULL;
	if (Cfg_DataBase_IP) free(Cfg_DataBase_IP);
	Cfg_DataBase_IP = NULL;
	if (Cfg_DataBase_Login) free(Cfg_DataBase_Login);
	Cfg_DataBase_Login = NULL;
	if (Cfg_DataBase_Pass) free(Cfg_DataBase_Pass);
	Cfg_DataBase_Pass = NULL;
	if (Cfg_DataBase_Name) free(Cfg_DataBase_Name);
	Cfg_DataBase_Name = NULL;
	DEBUG("%s (%i) :: Config Cleaned.\n", __func__, __LINE__);
}
