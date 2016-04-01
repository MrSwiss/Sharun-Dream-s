#include "Player.h"

void C_CANCEL_EXIT_f(player_t *player)
{
	player->exit_type = 0;
	OpCodes_Write(player, S_CANCEL_EXIT);
	Player_NextHeaderR(player);
}
