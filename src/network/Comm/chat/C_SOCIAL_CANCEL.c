#include "Player.h"

void C_SOCIAL_CANCEL_f(player_t *player)
{
	Player_NextHeaderR(player);

	World_PlayerSocial(player, -1);
}
