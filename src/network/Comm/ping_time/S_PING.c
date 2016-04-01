#include "Player.h"

void S_PING_f(player_t *player)
{
	Player_Send_Fast(player, S_PING);
}
