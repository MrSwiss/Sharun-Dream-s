#include "Player.h"

void S_NPCGUILD_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_NPCGUILD_LIST);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0x0FAC2722);
	Player_Writer4(&buffer, 0x00008000);

	Player_Send(player, &buffer);
}
