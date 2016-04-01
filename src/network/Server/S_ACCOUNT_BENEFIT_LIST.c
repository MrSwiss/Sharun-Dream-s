#include "Player.h"

void S_ACCOUNT_BENEFIT_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_ACCOUNT_BENEFIT_LIST);

	Player_Writer4(&buffer, 0);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);
}
