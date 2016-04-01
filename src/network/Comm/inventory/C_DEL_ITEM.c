#include "Player.h"

void C_DEL_ITEM_f(player_t *player)
{
	Player_Reader4(player); // Player ID
	Player_Reader4(player); // Sub ID

	ushort slot = Player_Reader2(player); // Slot
	slot += 39;

	Player_Reader2(player);
	ushort item_count = Player_Reader2(player); // Item Count

	player->Item_slot[slot].count -= item_count;
	SQL_Query_Slow2("UPDATE `character_items` SET `item_count` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", player->Item_slot[slot].count, player->Charcter_Id, player->Item_slot[slot].id);
	if (player->Item_slot[slot].count <= 0) {
		player->Item_slot[slot].item = NULL;
		SQL_Query_Slow2("DELETE FROM `character_items` WHERE `owner_id` = '%i' AND `object_id` = '%i';", player->Charcter_Id, player->Item_slot[slot].id);
	}

	S_INVEN_f(player, 0);

	Player_NextHeaderR(player);
}
