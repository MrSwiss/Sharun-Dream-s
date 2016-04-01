#include "Player.h"

void S_CHECK_VERSION_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CHECK_VERSION);
	
	Player_Writer1(&buffer, 1);

	Player_Send(player, &buffer);
}
