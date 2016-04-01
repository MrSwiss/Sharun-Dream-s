#include "Player.h"

void S_CONFIRM_INVITE_CODE_BUTTON_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_CONFIRM_INVITE_CODE_BUTTON);

	Player_Writer4(&buffer, 0x0F0001FA);
	Player_Writer4(&buffer, 0x91425600);
	Player_Writer4(&buffer, 0);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
