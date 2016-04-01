#include "Player.h"

void S_F2P_PremiumUser_Permission_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_F2P_PremiumUser_Permission);

	Player_Writer4(&buffer, 0x00140001);
	Player_Writer4(&buffer, 0x00000005);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0x00000014);
	Player_Writer4(&buffer, 0x00000001);

	Player_Send(player, &buffer);
}
