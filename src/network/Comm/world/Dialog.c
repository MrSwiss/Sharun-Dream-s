#include "Player.h"
/*
void S_SPAWN_NPC_f(player_t *player, NPC_t *npc, bool isFriend)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SPAWN_NPC);

	Player_Writer4(&buffer, 0);//00 00 00 00 8

	int shiftreplace = buffer.Pos;

	Player_Writer2(&buffer, 109);// 6D 00 10
	Player_Writer4(&buffer, npc->Id);// A3 D4 0C 00 обжект ид 14
	Player_Writer4(&buffer, npc->SUBID);// 00 80 0B 00 саб ид 18
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00 26

	for (int i=0; i<3; i++)
		Player_WriterF(&buffer, npc->World.XYZ[i]);
	Player_Writer2(&buffer, npc->World.heading);// 10 17 heading 40

	Player_Writer4(&buffer, 12);// 0C 00 00 00 статик 44

	Player_Writer4(&buffer, npc->npc_id);// 66 00 00 00 нпц темплейт ид 48
	Player_Writer2(&buffer, npc->npc_type);// 0D 00 класс моба 50

	Player_Writer2(&buffer, 0);// 2D 00 мб расса моба 52
	Player_Writer4(&buffer, 110);// 4C 00 00 00 56
	Player_Writer2(&buffer, 0);// 00 00 58
	Player_Writer4(&buffer, 5);// 05 00 00 00 62
	Player_Writer1(&buffer, 1);// 01 00 видимость 63

	Player_Writer1(&buffer, isFriend);// 01 00 1 - нпс 0 - моб 64
	Player_Writer2(&buffer, npc->spawned); // нужна ли вспышка 0 -вспышка

	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer2(&buffer, 0);// 00 00
	Player_Writer1(&buffer, 0);// 00
	Player_Writer4(&buffer, 0);// new 00000000
	Player_Writer1(&buffer, 0);// new 00
	Player_Writer4(&buffer, 0xB9ACC5D8);// new D8C5ACB9
	Player_Writer4(&buffer, 0x0000CE74);// new 74CE0000

	Player_WriterSet2(&buffer, buffer.Pos - 8, shiftreplace);

	Player_Send(player, &buffer);
}*/

