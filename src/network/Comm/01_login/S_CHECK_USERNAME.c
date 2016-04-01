#include "Player.h"

void S_CHECK_USERNAME_f(player_t *player, uchar result)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CHECK_USERNAME);

	Player_Writer1(&buffer, result);

	Player_Send(player, &buffer);
}
