#include "Player.h"

void S_MOVE_DISTANCE_DELTA_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_MOVE_DISTANCE_DELTA);

	Player_Writer4(&buffer, 0x43480000);

	Player_Send(player, &buffer);
}
