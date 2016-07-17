#include "Sharun.hpp"

#include <ctime>

/***
 * 	TODO: IP BLOCK !
 **/
void* C_LOGIN_ARBITER_f(player* player_l, packet* packet_l)
{
	ushort Pos_User = packet_l->read_2();
	ushort Pos_Pass = packet_l->read_2();
	ushort Pass_Len = packet_l->read_2();

	packet_l->pos += 2;
	packet_l->pos += 4;

	packet_l->pos += 8;
	packet_l->pos += 8;
	packet_l->pos += 8;
	packet_l->pos += 8;
	packet_l->pos += 4;
	packet_l->pos += 2;
	packet_l->pos += 1;

DEBUG("->\t %ld %ld %ld\n", Pos_User, Pos_Pass, Pass_Len);
	char   User[100];
	packet_l->pos = Pos_User;
	packet_l->read_C(User);

	char Pass[Pass_Len+1];
	packet_l->pos = Pos_Pass ;
	packet_l->read(Pass, Pass_Len);
	Pass[Pass_Len] = 0;

//	SQL_Format_Char(User);

	void *rslt = NULL;
	const char *sql = "SELECT `AccoundId`, `password`, `RightLevel`, `end_pay` FROM `accounts` WHERE `Name`='%s' AND `PasswordRecovery` IS NULL AND `end_block` < '%ld'";
	char **row, Query[strlen(sql)+strlen(User)+Pass_Len];

DEBUG("->\t \"%s\"\n\t-> \"%s\"\n", User, Pass);
	int run;
	do {
		run = 0;
		sprintf(Query, sql, User, time(NULL));
		rslt = Sharun->DB.Query_Fast(Query, 1);
		row = Sharun->DB.Next_Row(rslt);
		if ((!row || !row[0]) && Sharun->Settings.AutoAccount) {
			Sharun->DB.Result_Clear(rslt);
			rslt = NULL;
			sprintf(Query, "INSERT INTO `accounts` (`Name`, `password`) VALUES ('%s', '%s');", User, Pass);
			Sharun->DB.Query_Fast(Query, 0);
			run = 1;
		}
	} while (run);
	
	if (row[0] && !strncmp(row[1], Pass, strlen(row[1]))) {
		DEBUG("Hello \"%s\"\n", User);
DEBUG("->\t \"%s\"\n\t-> \"%s\"\n", row[0], row[2]);
		player_l = new_player(atol(row[0]));
DEBUG("->\t player_l->OpCode_GID=%i, player_l->Level=%i\n", player_l->OpCode_GID, player_l->Level);
/*		player_l->Right = atoi(row[2]);
		player_l->end_pay = atol(row[3]);
		Mutex_Lock(Players_mtx);
		while (player_l->Player_Id < Player_Max && Players[player_l->Player_Id]) player_l->Player_Id++;
		if (player_l->Player_Id < Player_Max)
			Players[player_l->Player_Id] = player_l;
		Mutex_Unlock(Players_mtx);
		char *sql = "UPDATE `accounts` SET `last_ip` = '%s', `lastOnlineUTC`='%i' WHERE `AccoundId`='%ld'";
		sprintf(Query, sql, player_l->NetLink->IP, time(NULL), player_l->Account_Id);
		SQL_Query_Slow(Query);
		OpCodes_Write(player_l, S_LOADING_SCREEN_CONTROL_INFO);
		OpCodes_Write(player_l, S_REMAIN_PLAY_TIME);
		OpCodes_Write(player_l, S_LOGIN_ARBITER);
		OpCodes_Write(player_l, S_LOGIN_ACCOUNT_INFO);
		OpCodes_Write(player_l, S_LOAD_CLIENT_ACCOUNT_SETTING);*/
	}
	Sharun->DB.Result_Clear(rslt);
	return NULL;
}
