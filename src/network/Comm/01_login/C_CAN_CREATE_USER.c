#include "Player.h"

void C_CAN_CREATE_USER_f(player_t *player)
{
	OpCodes_Write(player, S_CAN_CREATE_USER);
	Player_NextHeaderR(player);
}
