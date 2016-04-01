#include "Player.h"

void S_PET_INCUBATOR_INFO_CHANGE_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PET_INCUBATOR_INFO_CHANGE);

	Player_Writer2(&buffer, 5);
	Player_Writer2(&buffer, 22);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0);

	Player_Writer4(&buffer, 60);
	Player_Writer2(&buffer, 0);

	Player_Writer2(&buffer, 22);
	Player_Writer2(&buffer, 34);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 34);
	Player_Writer2(&buffer, 46);//2E00
	Player_Writer4(&buffer, -1);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 46);
	Player_Writer2(&buffer, 58);
	Player_Writer4(&buffer, -1);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 58); //3A00
	Player_Writer2(&buffer, 70);
	Player_Writer4(&buffer, -1);
	Player_Writer4(&buffer, 0);

	Player_Writer2(&buffer, 70); //46000000
	Player_Writer2(&buffer, 0); //46000000
	Player_Writer4(&buffer, -1);
	Player_Writer4(&buffer, 0);

	Player_Send(player, &buffer);
}
