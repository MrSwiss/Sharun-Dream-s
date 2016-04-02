#include "Player.h"

void S_SHOW_ITEM_TOOLTIP_f(player_t *player, char *name, uint itemID)
{
	int i=0;
	while (i<Player_Max && (!Players[i] || (Players[i] && strcmp(name, Players[i]->name)))) i++;
	if (i>=Player_Max) return;

	int index=0;
	while (index<39+player->Item_slot_count*8 && (!Players[i]->Item_slot[index].item || (Players[i]->Item_slot[index].item
	    && Players[i]->Item_slot[index].id != itemID))) index++;
	if (index>=39+player->Item_slot_count*8) return;

	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SHOW_ITEM_TOOLTIP);
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
14 binder name
*/
	ushort cristal_Pos = buffer.Pos;
	Player_Writer4(&buffer, 0); //Cristal Count + Pos

	ushort UNK_Pos = buffer.Pos;
	Player_Writer4(&buffer, 0); //UNK Count (15) + Pos

	ushort binded_Pos = buffer.Pos;
	Player_Writer4(&buffer, 0); //Binded Pos + Binder Name Pos

	Player_Writer4(&buffer, 20); // 14 00 00 00
	Player_Writer8(&buffer, Players[i]->Item_slot[index].id); // BE B4 4F 01
	Player_Writer4(&buffer, Players[i]->Item_slot[index].item->id); // D8 33 00 00
	Player_Writer8(&buffer, Players[i]->Item_slot[index].id); // BE B4 4F 01
	Player_Writer4(&buffer, player->Character_Id); // 48 38 00 00
	Player_Writer4(&buffer, 0); // 00 00 00 00	SUB ID ?
	Player_Writer4(&buffer, index); // 44 00 00 00
	Player_Writer4(&buffer, 0); // 00 00 00 00
	Player_Writer4(&buffer, 1); // 01 00 00 00
	Player_Writer4(&buffer, 1); // 01 00 00 00
	Player_Writer4(&buffer, Players[i]->Item_slot[index].enchant[0]); // 00 00 00 00 заточка

	Player_Writer4(&buffer, 0);//1);

	Player_Writer4(&buffer, Players[i]->Item_slot[index].item->bind ? 1 : 0); // 00 00 00 00 00 00 00 00
	/* бонусы от заточки включаеться если заточить на +1 а так нули */
	Player_Writer4(&buffer, 0); // 00 00 00 00 флота атаки бонус от заточки
	Player_Writer4(&buffer, 0); // 00 00 00 00 флот овертёрна бонус от заточки
	Player_Writer4(&buffer, 0); // 00 00 00 00 флот защиты бонус от заточки
	Player_Writer4(&buffer, 0); // 00 00 00 00 флот защиты от овертёрна бонус от заточки

	/*
	  * writeIntS(0xACBD0400); //00 00 00 00 writeIntS(0x3CBF0400); //00 00 00 00 writeIntS(0x80BC0400); //00 00 00 00 writeIntS(0x30C10400); //00 00 00 00
	  */

	Player_Writer1(&buffer, 0); // 00

	/* бонусы */
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	Player_Writer8(&buffer, 0);// 00 00 00 00 00 00 00 00
	// нужно распиливать на флоты на каждый параметр бонус отдельно

	Player_Writer8(&buffer, 0);// бонус к следующим заточкам
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);// бонус к следующим заточкам

	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);// видимо какието бафы
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);// какойто байтовый беспредел
	Player_Writer8(&buffer, 0xFEFEFEFEFEFEFEFE);// ?? иногда 3

	Player_Writer8(&buffer, 0);

	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer2(&buffer, 0);

	Player_Writer8(&buffer, 0);//item.getItemLevel());// Итем лвл
	Player_Writer8(&buffer, 0);

	Player_Writer8(&buffer, 0xFFFFFFFFFFFFFFFF);// не понятно иногда биндиться суда
	Player_Writer8(&buffer, 0xFFFFFFFFFFFFFFFE);
	Player_Writer8(&buffer, 0xFFFFFFFFFFFFFFFF);

	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);
	Player_Writer4(&buffer, 0);

	Player_Writer1(&buffer, 3);
	Player_Writer1(&buffer, 0);
	Player_Writer1(&buffer, 0);
	Player_Writer1(&buffer, 0);
	Player_Writer1(&buffer, 172);
	Player_Writer8(&buffer, 0);
	Player_Writer8(&buffer, 0);

	if(Players[i]->Item_slot[index].item->bind && Players[i]->Item_slot[index].binded)
	{
		Player_WriterSet2(&buffer, buffer.Pos, binded_Pos);
		Player_Writer2(&buffer, 0);
		Player_WriterSet2(&buffer, buffer.Pos, binded_Pos+2);
		Player_WriterS(&buffer, Players[i]->Item_slot[index].binder, 36);
	}

	// если в итем вставлены кристаллы
	short cristal_count = 0;
	ushort Cristal_Next = 0;
	for (int j=0; j<4; j++) {
		if (Players[i]->Item_slot[index].crystal[j]) {
			if (!cristal_count)
				Player_WriterSet2(&buffer, buffer.Pos, cristal_Pos+2);
			if (Cristal_Next)
				Player_WriterSet2(&buffer, buffer.Pos, Cristal_Next);
			Player_Writer2(&buffer, buffer.Pos);
			Cristal_Next = buffer.Pos;
			Player_Writer2(&buffer, 0);
			Player_Writer4(&buffer, Players[i]->Item_slot[index].crystal[j]);
			cristal_count++;
		}
	}
	if (cristal_count)
		Player_WriterSet2(&buffer, cristal_count, cristal_Pos);

	/**
	 * UNKNOW
	 **/
	cristal_count = 0;
	for (int j=0; j<15; j++) {
		cristal_count++;
		if (!cristal_count)
			Player_WriterSet2(&buffer, buffer.Pos, UNK_Pos+2);
		if (j)
			Player_WriterSet2(&buffer, buffer.Pos, Cristal_Next);
		Player_Writer2(&buffer, buffer.Pos);
		Cristal_Next = buffer.Pos;
		Player_Writer2(&buffer, 0);
		Player_Writer4(&buffer, 0);
	}
	if (cristal_count)
		Player_WriterSet2(&buffer, cristal_count, UNK_Pos);

	Player_Send(player, &buffer);
}
