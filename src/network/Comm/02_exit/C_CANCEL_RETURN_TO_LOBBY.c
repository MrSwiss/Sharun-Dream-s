#include "Player.h"

void C_CANCEL_RETURN_TO_LOBBY_f(player_t *player)
{
	player->exit_type = 0;
	OpCodes_Write(player, S_CANCEL_RETURN_TO_LOBBY);
	Player_NextHeaderR(player);
}
