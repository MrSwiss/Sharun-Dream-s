#include "Player.h"

void S_PREPARE_RETURN_TO_LOBBY_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PREPARE_RETURN_TO_LOBBY);

	Player_Writer4(&buffer, Confirm_Time);

	Player_Send(player, &buffer);

	player->exit_type = 2;
	player->exit_time = time(NULL) + Confirm_Time;
}
