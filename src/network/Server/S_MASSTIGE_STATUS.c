#include "Player.h"

void S_MASSTIGE_STATUS_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_MASSTIGE_STATUS);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
