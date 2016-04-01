#include "Player.h"

void S_MY_DESCRIPTION_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_MY_DESCRIPTION);

	Player_Writer4(&buffer, 0x00000006);

	Player_Send(player, &buffer);
}
