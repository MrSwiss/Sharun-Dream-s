#include "Player.h"

void S_AVAILABLE_EVENT_MATCHING_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_AVAILABLE_EVENT_MATCHING_LIST);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0x00010000);
	Player_Writer2(&buffer, 0);

	Player_Send(player, &buffer);
}
