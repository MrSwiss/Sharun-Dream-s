#include "Player.h"

void S_LOADING_SCREEN_CONTROL_INFO_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOADING_SCREEN_CONTROL_INFO);

	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
