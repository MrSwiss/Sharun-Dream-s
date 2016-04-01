#include "Player.h"

void C_MOVE_INVEN_POS_f(player_t *player)
{
	Player_Reader4(player); // Player ID
	Player_Reader4(player); // Sub ID

	ushort slot1 = Player_Reader4(player); // Slot1
	slot1--;
	ushort slot2 = Player_Reader4(player); // Slot2
	slot2--;

			if (player->Item_slot[slot2].item) SQL_Query_Slow2("UPDATE `character_items` SET `index` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", slot1, player->Charcter_Id, player->Item_slot[slot2].id);
			if (player->Item_slot[slot1].item) SQL_Query_Slow2("UPDATE `character_items` SET `index` = '%i' WHERE `owner_id` = '%i' AND `object_id` = '%i';", slot2, player->Charcter_Id, player->Item_slot[slot1].id);

	Item_slot_t Item_slot_tmp;
	memcpy(&Item_slot_tmp, &player->Item_slot[slot1], sizeof(Item_slot_t));
	memcpy(&player->Item_slot[slot1], &player->Item_slot[slot2], sizeof(Item_slot_t));
	memcpy(&player->Item_slot[slot2], &Item_slot_tmp, sizeof(Item_slot_t));

	S_INVEN_f(player, 0);

	Player_NextHeaderR(player);
}
