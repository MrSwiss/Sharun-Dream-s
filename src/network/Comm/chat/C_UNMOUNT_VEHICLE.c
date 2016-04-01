#include "Player.h"

void C_UNMOUNT_VEHICLE_f(player_t *player)
{
	Player_NextHeaderR(player);
	World_PlayerMount(player, 0, 111111);
}
