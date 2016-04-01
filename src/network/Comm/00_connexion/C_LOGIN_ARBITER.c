#include "Player.h"

/***
 * 	TODO: IP BLOCK !
 **/
void C_LOGIN_ARBITER_f(player_t *player)
{
	/*ushort Pos_User = */Player_Reader2(player);
	/*ushort Pos_Pass = */Player_Reader2(player);
	/*ushort Pass_Len = */Player_Reader2(player);

	Player_Reader2(player);
	Player_Reader4(player);

	Player_Reader8(player);
	Player_Reader8(player);
	Player_Reader8(player);
	Player_Reader8(player);
	Player_Reader4(player);
	Player_Reader2(player);
	Player_Reader1(player);

	char   User[100];
	Player_ReaderSI(player, User);

	char Pass[Cfg_PLAYER.PassLength+1];
	strncpy(Pass, (char*)&player->Packet_R[player->Packet_R_Pos], Cfg_PLAYER.PassLength);
	Pass[Cfg_PLAYER.PassLength] = 0;

	SQL_Format_Char(User);

	void *rslt;
	char **row, *sql = "SELECT `AccoundId`, `password`, `RightLevel`, `end_pay` FROM `accounts` WHERE `Name`='%s' AND `PasswordRecovery` IS NULL AND `end_block` < '%ld'";
	char Query[strlen(sql)+strlen(User)+Cfg_PLAYER.PassLength];

	int run;
	do {
		run = 0;
		sprintf(Query, sql, User, time(NULL));
		rslt = SQL_Query_Fast(Query, 1);
		row = SQL_Next_Row(rslt);
		if ((!row || !row[0]) && Cfg_PLAYER.AutoAccount) {
			sprintf(Query, "INSERT INTO `accounts` (`Name`, `password`) VALUES ('%s', '%s');", User, Pass);
			SQL_Query_Fast(Query, 0);
			run = 1;
		}
	} while (run);
	
	if (row[0] && !strncmp(row[1], Pass, Cfg_PLAYER.PassLength)) {
		player->Account_Id = atol(row[0]);
		player->Right = atoi(row[2]);
		player->end_pay = atol(row[3]);
		player->Player_Id = 0;
		Mutex_Lock(Players_mtx);
		while (player->Player_Id < Player_Max && Players[player->Player_Id]) player->Player_Id++;
		if (player->Player_Id < Player_Max)
			Players[player->Player_Id] = player;
		Mutex_Unlock(Players_mtx);
		char *sql = "UPDATE `accounts` SET `last_ip` = '%s', `lastOnlineUTC`='%i' WHERE `AccoundId`='%ld'";
		sprintf(Query, sql, player->NetLink->IP, time(NULL), player->Account_Id);
		SQL_Query_Slow(Query);
		OpCodes_Write(player, S_LOADING_SCREEN_CONTROL_INFO);
		OpCodes_Write(player, S_REMAIN_PLAY_TIME);
		OpCodes_Write(player, S_LOGIN_ARBITER);
		OpCodes_Write(player, S_LOGIN_ACCOUNT_INFO);
		OpCodes_Write(player, S_LOAD_CLIENT_ACCOUNT_SETTING);
	}
	SQL_Result_Clear(rslt);

	Player_NextHeaderR(player);
}
