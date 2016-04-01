#include "Player.h"

void S_EXIT_f(player_t *player)
{
	World_Exit(player);

	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_EXIT);

	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
