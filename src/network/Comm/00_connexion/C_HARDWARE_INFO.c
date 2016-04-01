#include "Player.h"

void C_HARDWARE_INFO_f(player_t *player)
{
	if (Cfg_PLAYER.HardwareHistory) {
		ushort OS_Pos  = Player_Reader2(player);
		ushort CPU_Pos = Player_Reader2(player);
		ushort GPU_Pos = Player_Reader2(player);


		uint CRAM = Player_Reader4(player);
		uint GRAM = Player_Reader4(player);
		uint G_W  = Player_Reader4(player);
		uint G_H  = Player_Reader4(player);

		Player_Reader1(player);

		uint *ScreenID  = (uint*)&player->Packet_R[player->Packet_R_HeadPos + OS_Pos - 4];

		uint S_ID, S_W, S_H;
		do {
			S_W  = Player_Reader4(player);
			S_H  = Player_Reader4(player);
			S_ID = Player_Reader4(player);
		} while (S_ID != *ScreenID);

		char OS[101], CPU[101], GPU[101];

		player->Packet_R_Pos = player->Packet_R_HeadPos + OS_Pos;
		Player_ReaderSI(player, OS);

		player->Packet_R_Pos = player->Packet_R_HeadPos + CPU_Pos;
		Player_ReaderSI(player, CPU);

		player->Packet_R_Pos = player->Packet_R_HeadPos + GPU_Pos;
		Player_ReaderSI(player, GPU);

		SQL_Format_Char(OS);
		SQL_Format_Char(CPU);
		SQL_Format_Char(GPU);

		char *SQL = "INSERT INTO `hardware` VALUES ('%i', '%ld', '%i', '%s', '%s', '%s (%i MB)', '%s (%i MB)', '%ix%i', '[%i] %ix%i');";
		char Query[115+MAX_ULONG+strlen(player->NetLink->IP)+strlen(OS)+strlen(CPU)+strlen(GPU)+MAX_UINT*9];
		sprintf(Query, SQL, player->Account_Id, time(NULL), Cfg_Server_ID, player->NetLink->IP, OS, CPU, CRAM, GPU, GRAM, G_W, G_H, S_ID, S_W, S_H);
		SQL_Query_Slow(Query);
	}

	Player_NextHeaderR(player);
}
