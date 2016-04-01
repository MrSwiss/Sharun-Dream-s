#include "Player.h"

void S_ARTISAN_SKILL_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_ARTISAN_SKILL_LIST);
	Player_Writer2(&buffer, 5);

	Player_Writer2(&buffer, 25);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0x48000000);
	Player_Writer4(&buffer, 0x00000E44);
	Player_Writer1(&buffer, 0);

	Player_Writer2(&buffer, 25);
	Player_Writer2(&buffer, 57);
	Player_Writer4(&buffer, 6);
	Player_Writer4(&buffer, 6);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 57);
	Player_Writer2(&buffer, 89);
	Player_Writer4(&buffer, 7);
	Player_Writer4(&buffer, 7);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 89);
	Player_Writer2(&buffer, 121);
	Player_Writer4(&buffer, 21);
	Player_Writer4(&buffer, 21);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 121);
	Player_Writer2(&buffer, 153);
	Player_Writer4(&buffer, 22);
	Player_Writer4(&buffer, 22);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 153);
	Player_Writer2(&buffer, 0);
	Player_Writer4(&buffer, 23);
	Player_Writer4(&buffer, 23);
	Player_Writer4(&buffer, 0x3F800000);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0xFFFFFFFF);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
