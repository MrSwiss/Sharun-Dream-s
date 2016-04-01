#include "Player.h"

void S_LOGIN_ACCOUNT_INFO_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOGIN_ACCOUNT_INFO);

	Player_Writer4(&buffer, 0x0E004B3C);
	Player_Writer4(&buffer, 0x5E000000);
	Player_Writer4(&buffer, 0x05500000);
	Player_Writer4(&buffer, 0x6C006100);
	Player_Writer4(&buffer, 0x6E006500);
	Player_Writer4(&buffer, 0x74004C00);
	Player_Writer4(&buffer, 0x00005400);

	Player_Send(player, &buffer);
}
