#include "Player.h"

void C_SKILL_LEARN_LIST_f(player_t *player)
{
	Player_NextHeaderR(player);
	S_SKILL_LEARN_LIST_f(player);
}
