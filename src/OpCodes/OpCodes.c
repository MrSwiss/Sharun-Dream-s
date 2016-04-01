#include "Player.h"

#include <dirent.h>

int OpCodes_Group_Count;

OpCode_recv_t ***OpCodes_StR = NULL;
ushort **OpCodes_StS = NULL;
short OpCode_CountR, OpCode_CountS;

int OpCodes_Init()
{
	int i;
	ushort filename_s = strlen(Cfg_Server_DirSave)+7+1;
	char filename[filename_s];
	sprintf(filename, "%sOpCodes", Cfg_Server_DirSave);
	OpCodes_StR = NULL;
	OpCodes_StS = NULL;
	for (i=0; !OpCode_LstR[i].name || strcmp(OpCode_LstR[i].name, "done"); i++) {
		OpCode_LstR[i].name = OpCodes_Name[OpCode_LstR[i].code];
	}
	OpCode_CountR = i;
	for (i=0; !OpCode_LstS[i].name || strcmp(OpCode_LstS[i].name, "done"); i++) {
		OpCode_LstS[i].name = OpCodes_Name[OpCode_LstS[i].code];
	}
	OpCode_CountS = i;
	if (!OpCode_CountR || !OpCode_CountS) {
		DEBUG("%s (%i) :: No OpCodes activated in program !\n", __func__, __LINE__);
		return 1;
	}
	OpCodes_Group_Count = 0;
	DIR *pdir;
	struct dirent *dirp;
	if ((pdir = opendir(filename)) != NULL) {
		while ((dirp = readdir(pdir)) != NULL) {
			if ((strcmp(dirp->d_name, ".")) && (strcmp(dirp->d_name, "..")) && (strcmp(dirp->d_name, ".")) && (!dir_exist(dirp->d_name)) && ((!strcmp(strrchr(dirp->d_name, '.'), ".txt")) || (!strcmp(strrchr(dirp->d_name, '.'), ".TXT")))) {
				char filename2[filename_s+1+strlen(dirp->d_name)];
				sprintf(filename2, "%s/%s", filename, dirp->d_name);
				FILE *fp = fopen(filename2, "r");
				if (fp) {
					OpCodes_StR = realloc(OpCodes_StR, sizeof(OpCode_recv_t**)*(OpCodes_Group_Count+1));
					OpCodes_StR[OpCodes_Group_Count] = malloc(sizeof(OpCode_recv_t*)*OpCodes_Max);
					i = 0;
					for (i=0; i<OpCodes_Max; i++)
						OpCodes_StR[OpCodes_Group_Count][i] = NULL;
					OpCodes_StS = realloc(OpCodes_StS, sizeof(ushort*)*(OpCodes_Group_Count+1));
					OpCodes_StS[OpCodes_Group_Count] = malloc(sizeof(ushort*)*OpCode_CountS);
					for (int i=0; i<OpCode_CountS; i++)
						OpCodes_StS[OpCodes_Group_Count][i] = 0x1111;
					char tmpS[100], tmp[10];
					while (!feof(fp)) {
						fscanf(fp, "%s = %s", tmpS, tmp);
						ushort tmpD = atoi(tmp);
						if (strlen(tmpS)) {
							i = 0;
							while (i<OpCode_CountR && strcmp(tmpS, OpCode_LstR[i].name)) i++;
							if (OpCode_LstR[i].name && strcmp(OpCode_LstR[i].name, "done"))
								OpCodes_StR[OpCodes_Group_Count][tmpD] = &OpCode_LstR[i];
							i = 0;
							while (i<OpCode_CountS && strcmp(tmpS, OpCode_LstS[i].name)) i++;
							if (OpCode_LstS[i].name && strcmp(OpCode_LstS[i].name, "done")) OpCodes_StS[OpCodes_Group_Count][i]/*->code*/ = tmpD;
						}
					}
					OpCodes_Group_Count++;
					fclose(fp);
				}
			}
		}
	}
	if (!OpCodes_Group_Count)
		DEBUG("%s (%i) :: No OpCodes to Load !\n", __func__, __LINE__);
	else	DEBUG("%s (%i) :: %i Groups of OpCodes Loaded.\n", __func__, __LINE__, OpCodes_Group_Count);
	return !OpCodes_Group_Count;
}

short OpCodes_DetectOpCode(ushort H_Code)
{
	for (short i=0; i<OpCodes_Group_Count; i++) 
		if (OpCodes_StR[i][H_Code] && !strcmp("C_LOGIN_ARBITER", OpCodes_StR[i][H_Code]->name))
			return i;
	return -1;
}

void OpCodes_GetOpCode(player_t *player, P_Buffer_t *buffer)
{
	if (buffer->OpCode == 0x1111)
		Player_WriterSet2(buffer, buffer->OpCode, 2);
	if (!player || player->OpCode_GID < 0 || !OpCodes_StS[player->OpCode_GID]/*[buffer->OpCode-1]*/ || buffer->OpCode-1 > OpCode_CountS) return;
	Player_WriterSet2(buffer, OpCodes_StS[player->OpCode_GID][buffer->OpCode-1]/*->code*/, 2);
}

const char *OpCodes_GetOpCode_Name(player_t *player, ushort H_Code)
{
	if (H_Code-1 > OpCode_CountS) return NULL;
	return /*OpCodes_StS[player->OpCode_GID]*/OpCode_LstS[H_Code-1].name;
}

const char *OpCodes_FindName(player_t *player)
{
	if (!player || player->OpCode_GID < 0) return NULL;
	PACKET_h *PACKET_H = (PACKET_h*)&player->Packet_R[player->Packet_R_HeadPos];
	if (OpCodes_StR[player->OpCode_GID][PACKET_H->Code])
		return OpCodes_StR[player->OpCode_GID][PACKET_H->Code]->name;
	return NULL;
}

void OpCodes_Read(player_t *player)
{
	if (!player || player->OpCode_GID < 0 || player->Packet_R_HeadPos >= player->Packet_R_Size) return;
	PACKET_h *PACKET_H = (PACKET_h*)&player->Packet_R[player->Packet_R_HeadPos];
	if (!OpCodes_StR[player->OpCode_GID][PACKET_H->Code]) {
//		Player_Send_Fast(player, 0x1111);
		Player_NextHeaderR(player);
	} else
		OpCodes_StR[player->OpCode_GID][PACKET_H->Code]->callback(player);
}

void *OpCodes_Write(player_t *player, ushort H_Code)
{
	if (!player || player->OpCode_GID < 0 || !OpCodes_StS[player->OpCode_GID][H_Code-1]) return NULL;
	return /*OpCodes_StS[player->OpCode_GID]*/OpCode_LstS[H_Code-1].callback(player);
}

void OpCodes_Close()
{
	for (int i=0; i<OpCodes_Group_Count; i++) {
		free(OpCodes_StR[i]);
		free(OpCodes_StS[i]);
	}
	free(OpCodes_StR);
	OpCodes_StR = NULL;
	free(OpCodes_StS);
	OpCodes_StS = NULL;
}
