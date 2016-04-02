#include "Player.h"

void C_EQUIP_ITEM_f(player_t *player)
{
	Player_Reader4(player); // Player ID
	Player_Reader4(player); // Sub ID

	ushort slot = Player_Reader2(player); // Slot
	slot--;

//	Player_Reader2(player);
//	Player_Reader4(player);

//	uint item_id = Player_Reader4(player); // Item ID

	Player_NextHeaderR(player);

	short dest_pos = -1;
	bool changed = false;
	if (!player->Item_slot[slot].item) return;
	if (player->Item_slot[slot].binded && player->Item_slot[slot].binded != player->Character_Id) {
		S_CHAT_to_Me_f(player, SYSTEM_CHAT, "You can't equip this item (not linked to your) !");
		return;
	}

	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_WEAPON")) {
		dest_pos = SLOT_WEAPON;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ARMOR_BODY")) {
		dest_pos = SLOT_ARMOR;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ARMOR_ARM")) {
		dest_pos = SLOT_GLOVES;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ARMOR_LEG")) {
		dest_pos = SLOT_BOOTS;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "necklace")) {
		dest_pos = SLOT_NECKLACE;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_UNDERWEAR")) {
		dest_pos = SLOT_SHIRT;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "accessoryHair")) {
		memcpy(&player->Item_slot[SLOT_HAT], &player->Item_slot[slot], sizeof(Item_slot_t));
		dest_pos = SLOT_HAT;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "accessoryFace")) {
		dest_pos = SLOT_MASK;
		changed = true;
	} else 
	if ((!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") || !strcmp(player->Item_slot[slot].item->type, "EQUIP_STYLE_ACCESSORY")) && !strcmp(player->Item_slot[slot].item->category, "style_hair")) {
		dest_pos = SLOT_APP_HAT;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_STYLE_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "style_face")) {
		dest_pos = SLOT_APP_MASK;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_STYLE_WEAPON")) {
		dest_pos = SLOT_APP_WEAPON;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_STYLE_BODY")) {
		dest_pos = SLOT_APP_ARMOR;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_STYLE_BACK")) {
		dest_pos = SLOT_APP_BACK;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "belt")) {
		dest_pos = SLOT_BELT;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "brooch")) {
		dest_pos = SLOT_BROOCH;
		changed = true;
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "earring")) {
		if (!player->Item_slot[SLOT_EARRING_L].item) {
			dest_pos = SLOT_EARRING_L;
			changed = true;
		} else
		if (!player->Item_slot[SLOT_EARRING_R].item) {
			dest_pos = SLOT_EARRING_R;
			changed = true;
		}
	} else 
	if (!strcmp(player->Item_slot[slot].item->type, "EQUIP_ACCESSORY") && !strcmp(player->Item_slot[slot].item->category, "ring")) {
		if (!player->Item_slot[SLOT_RING_L].item) {
			dest_pos = SLOT_RING_L;
			changed = true;
		} else
		if (!player->Item_slot[SLOT_RING_R].item) {
			dest_pos = SLOT_RING_R;
			changed = true;
		}
	}


	if (changed) {
		ulong obj_id = 0;
		if (dest_pos > -1) {
			if (player->Item_slot[dest_pos].item) SQL_Query_Slow2("UPDATE `character_items` SET `index` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", slot, player->Character_Id, player->Item_slot[dest_pos].id);
			if (player->Item_slot[slot].item) SQL_Query_Slow2("UPDATE `character_items` SET `index` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", dest_pos, player->Character_Id, player->Item_slot[slot].id);
			obj_id = player->Item_slot[slot].id;
			Item_slot_t Item_slot_tmp;
			memcpy(&Item_slot_tmp, &player->Item_slot[dest_pos], sizeof(Item_slot_t));
			memcpy(&player->Item_slot[dest_pos], &player->Item_slot[slot], sizeof(Item_slot_t));
			memcpy(&player->Item_slot[slot], &Item_slot_tmp, sizeof(Item_slot_t));
		}

		P_Buffer_t * buffer = OpCodes_Write(player, S_USER_EXTERNAL_CHANGE);
		buffer->BC_Type = BC_TYPE_ARROUND;
		buffer->Sender = player;
//		S_USER_STATUS_f(player);
		if ( dest_pos < SLOT_HAT
		  || dest_pos == SLOT_BELT
		  || dest_pos == SLOT_BROOCH )
			S_PLAYER_STAT_UPDATE_f(player);
		S_INVEN_f(player, 0);
		if (dest_pos > -1)
			S_EQUIP_ITEM_f(player, obj_id);
//		S_INVEN_CHANGEDSLOT_f(player, dest_pos-39+1);
		Broadcast(buffer);
	}
}
