#include "Player.h"

void S_SEND_USER_PLAY_TIME_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SEND_USER_PLAY_TIME);

	char *sql = "SELECT `online_time` FROM `characters` WHERE `object_id`='%i';";
	char Query[strlen(sql)+MAX_UINT];
	sprintf(Query, sql, player->Charcter_Id);

	Player_Writer4(&buffer, SQL_int(Query));
	Player_Writer8(&buffer, time(NULL));

	Player_Send(player, &buffer);
}
