#include "Player.h"

void C_PING_f(player_t *player)
{
	OpCodes_Write(player, S_RESPONSE_PONG);
	Player_NextHeaderR(player);
}
