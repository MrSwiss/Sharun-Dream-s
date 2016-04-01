#include "Player.h"

void C_SHOW_INVEN_f(player_t *player)
{
	S_INVEN_f(player, 1);
	Player_NextHeaderR(player);
}
