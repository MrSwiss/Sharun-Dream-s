#include "Player.h"

void S_PET_INFO_CLEAR_f(player_t *player)
{
	Player_Send_Fast(player, S_PET_INFO_CLEAR);
}
