#include "Player.h"

void C_SIMPLE_TIP_REPEAT_CHECK_f(player_t *player)
{
	uint va = Player_Reader4(player);
	Player_NextHeaderR(player);
	S_SIMPLE_TIP_REPEAT_CHECK_f(player, va);
}
