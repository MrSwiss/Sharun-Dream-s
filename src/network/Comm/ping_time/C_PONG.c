#include "Player.h"

void C_PONG_f(player_t *player)
{
	OpCodes_Write(player, S_PING);
	Player_NextHeaderR(player);
}
