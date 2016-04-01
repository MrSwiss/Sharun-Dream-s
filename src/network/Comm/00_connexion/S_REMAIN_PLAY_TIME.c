#include "Player.h"

void S_REMAIN_PLAY_TIME_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_REMAIN_PLAY_TIME);

	Player_Writer8(&buffer, 6);

	Player_Send(player, &buffer);
}

