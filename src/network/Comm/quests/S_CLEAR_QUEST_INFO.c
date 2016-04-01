#include "Player.h"

void S_CLEAR_QUEST_INFO_f(player_t *player)
{
	Player_Send_Fast(player, S_CLEAR_QUEST_INFO);
}
