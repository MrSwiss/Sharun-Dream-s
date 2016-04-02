#include "Player.h"

P_Buffer_t *S_USER_EXTERNAL_CHANGE_f(player_t *player)
{
	P_Buffer_t * buffer = New_WBuffer(S_USER_EXTERNAL_CHANGE);

		Player_Writer4(buffer, player->Character_Id);// обджект ид объекта
		Player_Writer4(buffer, player->SUBID);
		
		Player_Writer4(buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].item->id : 0);// DB_getEquipment(rslt2, SLOT_WEAPON, player->Character_Id)); // SLOT_WEAPON
		Player_Writer4(buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].item->id : 0);// DB_getEquipment(rslt2, SLOT_ARMOR, player->Character_Id)); // SLOT_ARMOR;
		Player_Writer4(buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].item->id : 0);// DB_getEquipment(rslt2, SLOT_GLOVES, player->Character_Id)); // SLOT_GLOVES;
		Player_Writer4(buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].item->id : 0);// DB_getEquipment(rslt2, SLOT_BOOTS, player->Character_Id)); // SLOT_BOOTS;
		Player_Writer4(buffer, player->Item_slot[SLOT_SHIRT].item ? player->Item_slot[SLOT_SHIRT].item->id : 0);// DB_getEquipment(rslt2, SLOT_SHIRT, player->Character_Id)); // SLOT_SHIRT;
		Player_Writer4(buffer, player->Item_slot[SLOT_HAT].item ? player->Item_slot[SLOT_HAT].item->id : 0);// DB_getEquipment(rslt2, SLOT_HAT, player->Character_Id)); // SLOT_HAT;
		Player_Writer4(buffer, player->Item_slot[SLOT_MASK].item ? player->Item_slot[SLOT_MASK].item->id : 0);// DB_getEquipment(rslt2, SLOT_MASK, player->Character_Id)); // SLOT_MASK;

		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);
		Player_Writer8(buffer, 0);

		Player_Writer1(buffer, player->Item_slot[SLOT_WEAPON].item ? player->Item_slot[SLOT_WEAPON].enchant[0] : 0);// weapon.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_ARMOR].item ? player->Item_slot[SLOT_ARMOR].enchant[0] : 0);// armor.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_GLOVES].item ? player->Item_slot[SLOT_GLOVES].enchant[0] : 0);// glove.getEnchantLevel
		Player_Writer1(buffer, player->Item_slot[SLOT_BOOTS].item ? player->Item_slot[SLOT_BOOTS].enchant[0] : 0);// boot.getEnchantLevel

		Player_Writer4(buffer, player->Item_slot[SLOT_APP_HAT].item ? player->Item_slot[SLOT_APP_HAT].item->id : 0);// new SLOT_APP_HAT
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_MASK].item ? player->Item_slot[SLOT_APP_MASK].item->id : 0);// new SLOT_APP_MASK
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_BACK].item ? player->Item_slot[SLOT_APP_BACK].item->id : 0);// new SLOT_APP_BACK
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_WEAPON].item ? player->Item_slot[SLOT_APP_WEAPON].item->id : 0);// new SLOT_APP_WEAPON
		Player_Writer4(buffer, player->Item_slot[SLOT_APP_ARMOR].item ? player->Item_slot[SLOT_APP_ARMOR].item->id : 0);// new SLOT_APP_ARMOR
		Player_Writer4(buffer, 0);

		Player_Writer1(buffer, 1);

	return buffer;
}
