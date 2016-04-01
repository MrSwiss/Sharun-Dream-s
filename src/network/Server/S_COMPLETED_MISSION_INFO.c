#include "Player.h"

void S_COMPLETED_MISSION_INFO_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_COMPLETED_MISSION_INFO);

	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
