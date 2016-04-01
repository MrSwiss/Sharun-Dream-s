#include "Player.h"

void S_SERVER_TIME_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SERVER_TIME);

	Player_Writer8(&buffer, time(NULL));

	Player_Send(player, &buffer);
}
