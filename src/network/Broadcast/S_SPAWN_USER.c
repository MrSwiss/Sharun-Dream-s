#include "Player.h"

P_Buffer_t *S_SPAWN_USER_f(player_t *player)
{
	P_Buffer_t * buffer = New_WBuffer(S_SPAWN_USER);

		Player_Writer8(buffer, 0);

		ushort Name_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later
		ushort GName_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later
		ushort Title_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later

		// remeber Detail1_Pos pos
		ushort Detail1_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later
		Player_Writer2(buffer, 32);// кол-во байт описания внешности

		ushort GTitle_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later
		ushort GIcon_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later

		// remeber Detail2_Pos pos
		ushort Detail2_Pos = buffer->Pos;
		buffer->Pos += 2;	// Write Later
		Player_Writer2(buffer, 64);	// Detail2 Len

		ushort Class_Race = 10000;
		Class_Race += (player->Race_Id * 2 + 1 + player->Sex) * 100;
		Class_Race += player->Class_Id + 1;

		Player_Writer4(buffer, 0);// айди сервера
		Player_Writer4(buffer, player->Account_Id);
		Player_Writer4(buffer, player->Charcter_Id);// обджект ид объекта
		Player_Writer4(buffer, player->SUBID);
		for (int i=0; i<3; i++)
			Player_WriterF(buffer, player->DjW->XYZ[i]);
		Player_Writer2(buffer, player->DjW->heading);

		Player_Writer4(buffer, 0);//player.getColor(newPlayer)); // цвет ника
		Player_Writer4(buffer, Class_Race);// индетификатор класса и расы объекта
		Player_Writer2(buffer, 0);
		Player_Writer2(buffer, 0x46);
		Player_Writer2(buffer, 0xAA);
		Player_Writer2(buffer, 0); // поза перса
		Player_Writer2(buffer, 0);
		Player_Writer1(buffer, 1);
		Player_Writer1(buffer, player->Stats.HP ? 1 : 0); // смерть

		// внешность
		Player_Writer1(buffer, 101); // temp[9]
		for (int j = 0; j < 6; j++)
			Player_Writer1(buffer, player->appearance[j]);
		Player_Writer1(buffer, 0); // temp[14]

		Player_Writer4(buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].item->id : 0);// DB_getEquipment(rslt2, SLOT_WEAPON, player->Charcter_Id)); // SLOT_WEAPON
		Player_Writer4(buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].item->id : 0);// DB_getEquipment(rslt2, SLOT_ARMOR, player->Charcter_Id)); // SLOT_ARMOR;
		Player_Writer4(buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].item->id : 0);// DB_getEquipment(rslt2, SLOT_GLOVES, player->Charcter_Id)); // SLOT_GLOVES;
		Player_Writer4(buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].item->id : 0);// DB_getEquipment(rslt2, SLOT_BOOTS, player->Charcter_Id)); // SLOT_BOOTS;
		Player_Writer4(buffer, player->Item_slot[SLOT_SHIRT].item ? player->Item_slot[SLOT_SHIRT].item->id : 0);// DB_getEquipment(rslt2, SLOT_SHIRT, player->Charcter_Id)); // SLOT_SHIRT;
		Player_Writer4(buffer, player->Item_slot[SLOT_HAT].item ? player->Item_slot[SLOT_HAT].item->id : 0);// DB_getEquipment(rslt2, SLOT_HAT, player->Charcter_Id)); // SLOT_HAT;
		Player_Writer4(buffer, player->Item_slot[SLOT_MASK].item ? player->Item_slot[SLOT_MASK].item->id : 0);// DB_getEquipment(rslt2, SLOT_MASK, player->Charcter_Id)); // SLOT_MASK;

		Player_Writer4(buffer, player->spawned); // вспышка
		Player_Writer4(buffer, player->mount ? player->mount->Mount_Id : 0);//newPlayer.getMountId()); // животное

		Player_Writer4(buffer, 7); // 7 ???
		Player_Writer4(buffer, 0); // 326 ??? // Title_id

		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);

		Player_Writer4(buffer, 0);
		Player_Writer2(buffer, 0);
		Player_Writer1(buffer, 0);

		Player_Writer1(buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].enchant[0] : 0);// weapon.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].enchant[0] : 0);// armor.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].enchant[0] : 0);// glove.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].enchant[0] : 0);// boot.getEnchantLevel

		Player_Writer1(buffer, 0);
		Player_Writer1(buffer, 0);//newPlayer.isPvPMode() ? 1 : 0); // включен ли пвп режим

		Player_Writer2(buffer, player->Level);
		Player_Writer2(buffer, 0);
		Player_Writer4(buffer, 0);

		Player_Writer4(buffer, 0);// что-то тоже нательное

		Player_Writer1(buffer, 1);

		Player_Writer4(buffer, player->Item_slot[SLOT_APP_HAT].item ? player->Item_slot[SLOT_APP_HAT].item->id : 0);// new SLOT_APP_HAT
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_MASK].item ? player->Item_slot[SLOT_APP_MASK].item->id : 0);// new SLOT_APP_MASK
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_BACK].item ? player->Item_slot[SLOT_APP_BACK].item->id : 0);// new SLOT_APP_BACK
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_WEAPON].item ? player->Item_slot[SLOT_APP_WEAPON].item->id : 0);// new SLOT_APP_WEAPON
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_ARMOR].item ? player->Item_slot[SLOT_APP_ARMOR].item->id : 0);// new SLOT_APP_ARMOR
		Player_Writer4(buffer, 0);

		Player_Writer4(buffer, 0);
		Player_Writer4(buffer, 0);
		Player_Writer1(buffer, 0);

		Player_Writer4(buffer, 0);// Allience
		// 1 = Velika ; 2 = Allemantheia ; 3 = Kaiator
		Player_Writer4(buffer, 100);// 64 00 00 00 = 100
		Player_Writer4(buffer, 0);// 0
		Player_Writer4(buffer, 256001);// 01 E8 03 00 = 256001
		Player_Writer4(buffer, 0);// 0
		Player_Writer1(buffer, 0);// 0
		Player_Writer4(buffer, 100);// 64 00 00 00 = 100
		Player_Writer2(buffer, 0);// 0
		Player_Writer2(buffer, 16256);// 80 3F = 16256

		Player_WriterSet2(buffer, buffer->Pos, Name_Pos);
		Player_WriterS(buffer, player->name, 36); //name);// имя

		Player_WriterSet2(buffer, buffer->Pos, GName_Pos);
		Player_WriterS(buffer, "", 36);//player->Guild->name);// название клана

		Player_WriterSet2(buffer, buffer->Pos, Title_Pos);
		Player_WriterS(buffer, player->title, 36);// title); // титул

		Player_WriterSet2(buffer, buffer->Pos, Detail1_Pos);
		for (int j = 0; j<32; j++)
			Player_Writer1(buffer, player->appearance[6+j]);

		Player_WriterSet2(buffer, buffer->Pos, GTitle_Pos);
		Player_WriterS(buffer, "", 36);//player->Guild->title);

		Player_WriterSet2(buffer, buffer->Pos, GIcon_Pos);
		Player_WriterS(buffer, "", 36);//player->Guild->icon);

		Player_WriterSet2(buffer, buffer->Pos, Detail2_Pos);
		for (int j = 0; j < 64; j++)
			Player_Writer1(buffer, player->appearance[6+32+j]);

	return buffer;
}
