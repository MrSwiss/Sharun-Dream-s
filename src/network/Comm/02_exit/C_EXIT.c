#include "Player.h"

void C_EXIT_f(player_t *player)
{
	OpCodes_Write(player, S_PREPARE_EXIT);
	Player_NextHeaderR(player);
}
