#include "Player.h"

void S_LOAD_HINT_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOAD_HINT);

	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
