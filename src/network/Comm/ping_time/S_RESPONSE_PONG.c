#include "Player.h"

void S_RESPONSE_PONG_f(player_t *player)
{
	Player_Send_Fast(player, S_RESPONSE_PONG);
}
