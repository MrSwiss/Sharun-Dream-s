#include "Player.h"

void S_SELECT_USER_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SELECT_USER);

	Player_Writer1(&buffer, 1);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
