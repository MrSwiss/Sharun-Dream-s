#include "Player.h"

void C_SOCIAL_f(player_t *player)
{
	int id = Player_Reader4(player);

	Player_NextHeaderR(player);

	World_PlayerSocial(player, id);
}
