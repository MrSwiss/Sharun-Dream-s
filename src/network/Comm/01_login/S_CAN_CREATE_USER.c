#include "Player.h"

void S_CAN_CREATE_USER_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CAN_CREATE_USER);

	if (player->Characters_Count < Charcters_Max_Per_Account)
		Player_Writer4(&buffer, 1);
	else
		Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
