#include "Player.h"

void C_REQUEST_GAMESTAT_PING_f(player_t *player)
{
	OpCodes_Write(player, S_RESPONSE_GAMESTAT_PONG);
	Player_NextHeaderR(player);
}
