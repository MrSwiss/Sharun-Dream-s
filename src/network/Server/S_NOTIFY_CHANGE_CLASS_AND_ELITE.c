#include "Player.h"

void S_NOTIFY_CHANGE_CLASS_AND_ELITE_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_NOTIFY_CHANGE_CLASS_AND_ELITE);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
