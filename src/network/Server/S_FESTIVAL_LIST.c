#include "Player.h"

void S_FESTIVAL_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_FESTIVAL_LIST);

	Player_Writer4(&buffer, 0x00080002);
	Player_Writer4(&buffer, 0x00100008);
	Player_Writer4(&buffer, 0x00000004);
	Player_Writer4(&buffer, 0x00000010);
	Player_Writer4(&buffer, 0x00000050);

	Player_Send(player, &buffer);
}
