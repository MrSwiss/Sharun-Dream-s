#include "Player.h"

void USER_SETTING_Saved(player_t *player, ushort OpCode)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, OpCode);

	Player_Writer4(&buffer, 8);

	Player_Send(player, &buffer);
}
