#include "Player.h"

void S_USER_STATUS_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_USER_STATUS);

	Player_Writer4(&buffer, player->Charcter_Id);
	Player_Writer4(&buffer, player->SUBID);
	Player_Writer4(&buffer, 0);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
