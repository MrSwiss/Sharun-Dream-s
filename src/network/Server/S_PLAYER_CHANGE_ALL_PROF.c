#include "Player.h"

void S_PLAYER_CHANGE_ALL_PROF_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PLAYER_CHANGE_ALL_PROF);

	Player_Writer2(&buffer, 350);
	Player_Writer2(&buffer, 350);
	Player_Writer2(&buffer, 0);
	Player_Writer2(&buffer, 302);
	Player_Writer2(&buffer, 5);
	Player_Writer2(&buffer, 5);
	Player_Writer2(&buffer, 5);
	Player_Writer2(&buffer, 5);

	Player_Send(player, &buffer);
}
