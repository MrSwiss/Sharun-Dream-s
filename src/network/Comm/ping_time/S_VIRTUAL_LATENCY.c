#include "Player.h"

void S_VIRTUAL_LATENCY_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_VIRTUAL_LATENCY);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
