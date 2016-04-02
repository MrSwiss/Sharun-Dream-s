#include "Player.h"

void Character_Save(player_t *player)
{
	char *sql = "UPDATE `characters` SET `x`='%s', `y`='%s', `z`='%s', `heading`='%i', `zone_id`='%i', `hp`='%i', `mp`='%i', `heart`='%i', `last_online`='%ld', `online_time`=`online_time`+'%ld' WHERE `object_id`='%i';";
	char XYZ[3][30];
	for (int i=0; i<3; i++) {
		sprintf(XYZ[i], "%f", player->World.XYZ[i]);
		SQL_toFloat(XYZ[i]);
	}
	char Query[strlen(sql)+strlen(XYZ[0])+strlen(XYZ[1])+strlen(XYZ[2])+MAX_UINT*6+MAX_ULONG*2];
	long l_time = time(NULL) - player->loged;
	player->loged += l_time;
	sprintf(Query, sql, XYZ[0], XYZ[1], XYZ[2], player->World.heading, player->World.Zone->Id, player->Stats.HP, player->Stats.MP, player->Stats.Stamina[1], time(NULL), l_time, player->Character_Id);
	SQL_Query_Slow(Query);
}
