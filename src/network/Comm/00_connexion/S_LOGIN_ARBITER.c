#include "Player.h"

void S_LOGIN_ARBITER_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOGIN_ARBITER);

	Player_Writer2(&buffer, 1);
	Player_Writer2(&buffer, 2);
	Player_Writer4(&buffer, 0);
	Player_Writer2(&buffer, 0);
	Player_Writer2(&buffer, 6);
	Player_Writer4(&buffer, 0);
	Player_Writer2(&buffer, 0);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
