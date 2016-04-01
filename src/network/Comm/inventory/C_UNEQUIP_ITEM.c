#include "Player.h"

void C_UNEQUIP_ITEM_f(player_t *player)
{
	Player_Reader4(player); // Player ID
	Player_Reader4(player); // Sub ID

	ushort slot = Player_Reader2(player); // Slot
	slot--;

	Player_Reader2(player);
	Player_Reader4(player);

	uint item_id = Player_Reader4(player); // Item ID

	short inv_pos = 39;
	ulong obj_id = 0;
	bool changed = false;
	if (!player->Item_slot[slot].item) return;
	if (player->Item_slot[slot].item->id == item_id) {
		while (player->Item_slot[inv_pos].item && inv_pos < 39+player->Item_slot_count*8) inv_pos++;
		if (inv_pos >= 39+player->Item_slot_count*8)
			S_CHAT_to_Me_f(player, SYSTEM_CHAT, "Inventory is full !");
		else {
			memcpy(&player->Item_slot[inv_pos], &player->Item_slot[slot], sizeof(Item_slot_t));
			player->Item_slot[slot].item = NULL;
			SQL_Query_Slow2("UPDATE `character_items` SET `index` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", inv_pos, player->Charcter_Id, player->Item_slot[slot].id);
			obj_id = player->Item_slot[inv_pos].id;
			changed = true;
		}
	}

	if (changed) {
		P_Buffer_t * buffer = OpCodes_Write(player, S_USER_EXTERNAL_CHANGE);
		buffer->BC_Type = BC_TYPE_ARROUND;
		buffer->Sender = player;
//		S_USER_STATUS_f(player);
		if ( inv_pos < SLOT_HAT
		  || inv_pos == SLOT_BELT
		  || inv_pos == SLOT_BROOCH )
			S_PLAYER_STAT_UPDATE_f(player);
		S_INVEN_f(player, 0);
		S_UNEQUIP_ITEM_f(player, obj_id);
//		S_INVEN_CHANGEDSLOT_f(player, inv_pos-39+1);
		Broadcast(buffer);
	}

	Player_NextHeaderR(player);
}
