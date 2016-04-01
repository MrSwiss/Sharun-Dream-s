#include "Player.h"

void S_STR_EVALUATE_LIST_f(player_t *player, uint type, WCHAR *name)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_STR_EVALUATE_LIST);

	Player_Writer2(&buffer, 1);
	Player_Writer2(&buffer, 8);
	Player_Writer4(&buffer, 8);
	Player_Writer2(&buffer, 22);
	Player_Writer4(&buffer, type);//индекс
	Player_Writer4(&buffer, 0); //18
	Player_Writer(&buffer, (byte*)name, (WCHARlen(name)+1)*sizeof(WCHAR));

	Player_Send(player, &buffer);
}
