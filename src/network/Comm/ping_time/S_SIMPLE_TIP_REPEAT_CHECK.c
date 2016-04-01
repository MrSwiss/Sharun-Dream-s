#include "Player.h"

void S_SIMPLE_TIP_REPEAT_CHECK_f(player_t *player, uint va)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SIMPLE_TIP_REPEAT_CHECK);

//	Player_Writer4(&buffer, va);
	Player_Writer4(&buffer, 0);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
