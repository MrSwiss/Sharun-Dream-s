#include "Player.h"

void S_CANCEL_EXIT_f(player_t *player)
{
	player->exit_type = 0;
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CANCEL_EXIT);

	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
