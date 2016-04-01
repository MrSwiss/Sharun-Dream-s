#include "Player.h"

void S_BROCAST_GUILD_FLAG_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_BROCAST_GUILD_FLAG);

	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
