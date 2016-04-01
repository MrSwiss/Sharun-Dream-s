#include "Player.h"

void C_RETURN_TO_LOBBY_f(player_t *player)
{
	OpCodes_Write(player, S_PREPARE_RETURN_TO_LOBBY);
	Player_NextHeaderR(player);
}
