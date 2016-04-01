#include "Player.h"

void S_RETURN_TO_LOBBY_f(player_t *player)
{
	World_Exit(player);

	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_RETURN_TO_LOBBY);

	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);

	player->exit_type = 0;
}
