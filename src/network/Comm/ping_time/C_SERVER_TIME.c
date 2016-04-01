#include "Player.h"

void C_SERVER_TIME_f(player_t *player)
{
	OpCodes_Write(player, S_SERVER_TIME);
	Player_NextHeaderR(player);
}
