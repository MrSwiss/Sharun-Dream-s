#include "Player.h"

void S_DAILY_QUEST_COMPLETE_COUNT_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_BROCAST_GUILD_FLAG);

	Player_Writer4(&buffer, 0x000A0000);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
