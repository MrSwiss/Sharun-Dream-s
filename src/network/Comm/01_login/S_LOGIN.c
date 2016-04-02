#include "Player.h"

void S_LOGIN_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOGIN);

	Player_Load(player);

		// remeber Name_Pos pos
		ushort Name_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later

		// remeber Detail1_Pos pos
		ushort Detail1_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later
		Player_Writer2(&buffer, 32);	// Detail1 Len

		// remeber Detail2_Pos pos
		ushort Detail2_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later
		Player_Writer2(&buffer, 64);	// Detail2 Len

		ushort Class_Race = 10000;
		Class_Race += (player->Race_Id * 2 + 1 + player->Sex) * 100;
		Class_Race += player->Class_Id + 1;

		Player_Writer4(&buffer, Class_Race); // Sex + Race + Class
		Player_Writer4(&buffer, player->Account_Id);//player->Character_Id); // Account_id
		Player_Writer4(&buffer, player->SUBID); // sub_object_id

		Player_Writer4(&buffer, 20 + Cfg_Server_ID); // SERVER ID
		Player_Writer4(&buffer, player->Character_Id); // Character_ID


		Player_Writer4(&buffer, 0);
		Player_Writer1(&buffer, player->Stats.HP ? 1 : 0);// player.isDead
		Player_Writer4(&buffer, 0);
		Player_Writer4(&buffer, 50);
		Player_Writer4(&buffer, 110);

		Player_Writer1(&buffer, 101); // HEAD APPEARANCE
		for (int j = 0; j < 6; j++)
			Player_Writer1(&buffer, player->appearance[j]);
		Player_Writer1(&buffer, 0); // temp[14]

		Player_Writer2(&buffer, 1);
		Player_Writer2(&buffer, player->Level);// уровень игрока

		Player_Writer2(&buffer, player->collect[0]);// Collect Mining
		Player_Writer2(&buffer, 0);// Collect 0
		Player_Writer2(&buffer, player->collect[1]);// Collect Plant
		Player_Writer2(&buffer, player->collect[2]);// Collect Energy

		Player_Writer4(&buffer, 1);
		Player_Writer2(&buffer, 0);

		Player_Writer4(&buffer, 0);

		Player_Writer8(&buffer, player->exp); // not really exp !
		Player_Writer8(&buffer, player->exp);
		Player_Writer8(&buffer, Experience_LEVEL[player->Level]);

		Player_Writer4(&buffer, 0);// ?
		Player_Writer4(&buffer, 0);// ?
		Player_Writer4(&buffer, 0x000001A3);// ?
		Player_Writer8(&buffer, 0);// ?

		Player_Writer4(&buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].item->id : 0);// DB_getEquipment(rslt2, SLOT_WEAPON, player->Character_Id)); // SLOT_WEAPON
		Player_Writer4(&buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].item->id : 0);// DB_getEquipment(rslt2, SLOT_ARMOR, player->Character_Id)); // SLOT_ARMOR;
		Player_Writer4(&buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].item->id : 0);// DB_getEquipment(rslt2, SLOT_GLOVES, player->Character_Id)); // SLOT_GLOVES;
		Player_Writer4(&buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].item->id : 0);// DB_getEquipment(rslt2, SLOT_BOOTS, player->Character_Id)); // SLOT_BOOTS;
		Player_Writer4(&buffer, player->Item_slot[SLOT_SHIRT].item ? player->Item_slot[SLOT_SHIRT].item->id : 0);// DB_getEquipment(rslt2, SLOT_SHIRT, player->Character_Id)); // SLOT_SHIRT;
		Player_Writer4(&buffer, player->Item_slot[SLOT_HAT].item ? player->Item_slot[SLOT_HAT].item->id : 0);// DB_getEquipment(rslt2, SLOT_HAT, player->Character_Id)); // SLOT_HAT;
		Player_Writer4(&buffer, player->Item_slot[SLOT_MASK].item ? player->Item_slot[SLOT_MASK].item->id : 0);// DB_getEquipment(rslt2, SLOT_MASK, player->Character_Id)); // SLOT_MASK;

		Player_Writer4(&buffer, 0x1291E64A); // new
		Player_Writer4(&buffer, 0);// new
			
		Player_Writer8(&buffer, 1);
		Player_Writer1(&buffer, 0);
		Player_Writer4(&buffer, 0);// 03 00 00 00 если 3, то надпись "Ангел смерти"
		Player_Writer4(&buffer, 0);// 00 00 00 00

		Player_Writer4(&buffer, 0);// 00 00 00 00 // new
		Player_Writer4(&buffer, 0);// 00 00 00 00 // new
		Player_Writer4(&buffer, 0);// 00 00 00 00 // new
		Player_Writer4(&buffer, 0);// 00 00 00 00 // new
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
/*48*/		Player_Writer4(&buffer, 0);// 00 00 00 00

		Player_Writer1(&buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].enchant[0] : 0);// weapon.getEnchantLevel
		Player_Writer1(&buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].enchant[0] : 0);// armor.getEnchantLevel
		Player_Writer1(&buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].enchant[0] : 0);// glove.getEnchantLevel
		Player_Writer1(&buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].enchant[0] : 0);// boot.getEnchantLevel

		Player_Writer1(&buffer, 0);// 00

		Player_Writer4(&buffer, 0);// 78 00 00 00 .//карма player.getKarma()
		Player_Writer1(&buffer, 1);// 01 00 00 00

		Player_Writer4(&buffer, player->Item_slot[SLOT_APP_HAT].item ? player->Item_slot[SLOT_APP_HAT].item->id : 0);// new SLOT_APP_HAT
		Player_Writer4(&buffer, player->Item_slot[SLOT_APP_MASK].item ? player->Item_slot[SLOT_APP_MASK].item->id : 0);// new SLOT_APP_MASK
		Player_Writer4(&buffer, player->Item_slot[SLOT_APP_BACK].item ? player->Item_slot[SLOT_APP_BACK].item->id : 0);// new SLOT_APP_BACK
		Player_Writer4(&buffer, player->Item_slot[SLOT_APP_WEAPON].item ? player->Item_slot[SLOT_APP_WEAPON].item->id : 0);// new SLOT_APP_WEAPON
		Player_Writer4(&buffer, player->Item_slot[SLOT_APP_ARMOR].item ? player->Item_slot[SLOT_APP_ARMOR].item->id : 0);// new SLOT_APP_ARMOR
		Player_Writer4(&buffer, 0); // new

		Player_Writer4(&buffer, 0);// 00 00 00 00 // new 02 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00 // new 64 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00 // new

		Player_Writer4(&buffer, 1);// 01 00 00 00
		Player_Writer4(&buffer, 0);// 00 00 00 00
		Player_Writer1(&buffer, 0);// 00
		Player_Writer4(&buffer, 100);// 64 00 00 00
		Player_Writer4(&buffer, 0x3F800000);
		Player_Writer1(&buffer, 1);
		Player_Writer4(&buffer, 1);
		Player_Writer4(&buffer, 0);

		Player_WriterSet2(&buffer, buffer.Pos, Name_Pos);
		Player_WriterS(&buffer, player->name, 36);

		Player_WriterSet2(&buffer, buffer.Pos, Detail1_Pos);
		for (int j = 0; j<32; j++)
			Player_Writer1(&buffer, player->appearance[6+j]);

		Player_WriterSet2(&buffer, buffer.Pos, Detail2_Pos);
		for (int j = 0; j < 64; j++)
			Player_Writer1(&buffer, player->appearance[6+32+j]);

	Player_Send(player, &buffer);
}
