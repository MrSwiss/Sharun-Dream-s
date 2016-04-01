#include "Player.h"

void S_DELETE_USER_f(player_t *player, uchar result)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_DELETE_USER);

	Player_Writer1(&buffer, result);

	Player_Send(player, &buffer);
}
