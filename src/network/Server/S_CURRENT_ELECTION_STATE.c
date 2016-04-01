#include "Player.h"

void S_CURRENT_ELECTION_STATE_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CURRENT_ELECTION_STATE);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 11);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0x3CFF0000);
	Player_Writer4(&buffer, 0x000051E4);
	Player_Writer2(&buffer, 0);

	Player_Send(player, &buffer);
}
