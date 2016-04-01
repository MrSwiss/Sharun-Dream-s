#include "Player.h"

void S_INVEN_f(player_t *player, short show)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_INVEN);
/*
00 object_id
01 owner_id
02 item_id
03 item_count
04 enchant_level
05 enchant_max
06 autor
07 location
08 index
09 crystal1
10 crystal2
11 crystal3
12 crystal4
13 binded
14 binder
*/
	int total = 0;
	ushort nextPos = 0, UnkPos, totalPos;

	// определяем кол-во всех ячеяк
	totalPos = buffer.Pos;
	buffer.Pos += 2;

	
	nextPos = buffer.Pos;
	Player_Writer2(&buffer, 0);
	Player_Writer4(&buffer, player->Charcter_Id);
	Player_Writer4(&buffer, player->SUBID);

	Player_Writer8(&buffer, player->money);

	Player_Writer1(&buffer, show);// 0101
	Player_Writer2(&buffer, 1);
	Player_Writer4(&buffer, player->Item_slot_count*8);

	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 1);
	Player_Writer4(&buffer, 0); // new
	Player_Writer4(&buffer, 0); // new
	Player_Writer4(&buffer, 0); // new

	for (short index=0; index<39+player->Item_slot_count*8; index++) {
		if (player->Item_slot[index].item) {
			total++;

			if (nextPos) Player_WriterSet2(&buffer, buffer.Pos, nextPos);

			Player_Writer2(&buffer, buffer.Pos);

			nextPos = buffer.Pos;
			Player_Writer2(&buffer, 0);

			Player_Writer4(&buffer, 0);
			// Count ? Item Stat
			// Pos of Item Stat

			UnkPos = buffer.Pos;
			Player_Writer2(&buffer, 0);

			Player_Writer4(&buffer, player->Item_slot[index].item->id);
			Player_Writer8(&buffer, player->Item_slot[index].id);
			Player_Writer4(&buffer, player->Charcter_Id);
			Player_Writer4(&buffer, player->SUBID);
			Player_Writer4(&buffer, index+1);//i + 1);
			Player_Writer4(&buffer, 0);
			Player_Writer8(&buffer, player->Item_slot[index].count);
			Player_Writer4(&buffer, 0);

			Player_Writer1(&buffer, player->Item_slot[index].item->bind ? 1 : 0);

			// если кристалов нет
			for (int j=0; j<4; j++)
				Player_Writer4(&buffer, player->Item_slot[index].crystal[j]);

			Player_Writer8(&buffer, 0);
			Player_Writer8(&buffer, 0);
			Player_Writer8(&buffer, 0);//-1);
			Player_Writer8(&buffer, 0);//-1);
			Player_Writer8(&buffer, 0);
			Player_Writer1(&buffer, 0);
			Player_Writer4(&buffer, 0); // need to have Combat item type added like 3 etc...(349, ...)
			Player_Writer8(&buffer, 0);
			Player_Writer8(&buffer, 0);
			Player_Writer8(&buffer, 0);
			Player_Writer4(&buffer, player->Item_slot[index].binded); // ??? = atoi(row[1]) linked owner ?
			Player_Writer4(&buffer, 0);
			Player_Writer4(&buffer, 0);
			Player_Writer1(&buffer, 0);
//			Player_Writer4(&buffer, 0xFFFFFFFF); 0xFFFFFFFF // rollable bonus
			Player_Writer1(&buffer, -1);//36);
			Player_Writer1(&buffer, -1);//0);
			Player_Writer1(&buffer, -1);//0);
			Player_Writer1(&buffer, -1);//0);
			Player_Writer4(&buffer, 0);

			Player_WriterSet2(&buffer, buffer.Pos, UnkPos);
			Player_Writer2(&buffer, 0);

			/** // Special Stat?
			  * ushort This Pos
			  * ushort Next Stat Pos ; if = 0 : no next
			  * uint Stat Code	320200 ; 325300 ; 325200
			  **/
			Player_Writer4(&buffer, 0);
		}
	}

	Player_WriterSet2(&buffer, total, totalPos);

	Player_Send(player, &buffer);
}
