#include "Player.h"

void S_CREATE_USER_f_Fail(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CREATE_USER);
	Player_Writer1(&buffer, 0);
	Player_Send(player, &buffer);
}

void S_CREATE_USER_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CREATE_USER);
	Player_Writer1(&buffer, 1);
	Player_Send(player, &buffer);
}
