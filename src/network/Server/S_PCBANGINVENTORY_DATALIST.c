#include "Player.h"

void S_PCBANGINVENTORY_DATALIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PCBANGINVENTORY_DATALIST);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
