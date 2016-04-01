#include "Player.h"

void S_ARTISAN_RECIPE_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_ARTISAN_RECIPE_LIST);

	Player_Writer4(&buffer, 0);
	Player_Writer2(&buffer, 0x0100);

	Player_Send(player, &buffer);
}
