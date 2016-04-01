#include "Player.h"

void S_PREPARE_EXIT_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PREPARE_EXIT);

	Player_Writer4(&buffer, Confirm_Time);

	Player_Send(player, &buffer);

	player->exit_type = 1;
	player->exit_time = time(NULL) + Confirm_Time;
}
