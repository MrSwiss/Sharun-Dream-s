#include "Player.h"

void S_PLAYER_CHANGE_MP_f(player_t *player, player_t *attacker, int countChange, int type)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PLAYER_CHANGE_MP);

	Player_Writer4(&buffer, player->Stats.MP); // сколько сейчас
	Player_Writer4(&buffer, player->Stats.maxMP[1]); // сколько размер полоски
	Player_Writer4(&buffer, countChange); // на сколько увеличилось хп
	Player_Writer4(&buffer, type);// если 01 будет зелёный плюсик
	Player_Writer4(&buffer, player->Character_Id); // наш ид
	Player_Writer4(&buffer, player->SUBID); // наш сабид
	if(attacker) {
		Player_Writer4(&buffer, attacker->Character_Id); // ИД того кто вас хильнул
		Player_Writer4(&buffer, attacker->SUBID); // САБ ИД того кто вас хильнул
	} else {
		Player_Writer4(&buffer, 0); // ИД того кто вас хильнул
		Player_Writer4(&buffer, 0); // САБ ИД того кто вас хильнул
	}

	Player_Send(player, &buffer);
}
