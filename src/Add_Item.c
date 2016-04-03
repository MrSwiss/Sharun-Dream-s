#include "Player.h"

extern Item_t* Items;

ulong Item_ID_Act = 1;
void * Item_ID_Act_mtx = NULL;

void Add_Item_Init()
{
	Item_ID_Act_mtx = Mutex_Init();
	char req[1024];
	sprintf(req, "SELECT MAX(`character_items`.`object_id`) FROM `character_items`, `characters` WHERE `character_items`.`owner_id`=`characters`.`object_id` AND `Server_ID`='%i';", Cfg_Server_ID);
	Item_ID_Act = SQL_int(req);
	Item_ID_Act++;
}

void Add_Item_Close()
{
	if (Item_ID_Act_mtx) Mutex_Close(Item_ID_Act_mtx);
	Item_ID_Act_mtx = NULL;
	Item_ID_Act = 1;
}

void Add_Item(player_t *player, uint ID, short count)
{
	short inv_pos = 39;
	while (inv_pos < 39+player->Item_slot_count*8) {
		if (player->Item_slot[inv_pos].item && player->Item_slot[inv_pos].item->id == ID &&
		    player->Item_slot[inv_pos].count < player->Item_slot[inv_pos].item->maxStack) {
			short left = player->Item_slot[inv_pos].item->maxStack - player->Item_slot[inv_pos].count;
			if (count <= left) {
				player->Item_slot[inv_pos].count += count;
				count = 0;
			} else {
				player->Item_slot[inv_pos].count += left;
				count -= left;
			}
		}
		inv_pos++;
	}
	if (count <= 0)
		return;
	Item_t * t_Item = get_Item(ID);
	if (!t_Item)
		return;
	while (count > 0) {
		inv_pos = 39;
		while (player->Item_slot[inv_pos].item && inv_pos < 39+player->Item_slot_count*8) inv_pos++;
		if (inv_pos < 39+player->Item_slot_count*8) {
			player->Item_slot[inv_pos].item = t_Item;
			short left = player->Item_slot[inv_pos].item->maxStack - player->Item_slot[inv_pos].count;
			Mutex_Lock(Item_ID_Act_mtx);
			player->Item_slot[inv_pos].id = Item_ID_Act++;
			Mutex_Unlock(Item_ID_Act_mtx);
			player->Item_slot[inv_pos].enchant[0] = 0;
			player->Item_slot[inv_pos].enchant[1] = 0;
			player->Item_slot[inv_pos].crystal[0] = 0;
			player->Item_slot[inv_pos].crystal[1] = 0;
			player->Item_slot[inv_pos].crystal[2] = 0;
			player->Item_slot[inv_pos].crystal[3] = 0;
			player->Item_slot[inv_pos].binded = 0;
			player->Item_slot[inv_pos].binder[0] = 0;
			if (count < left) {
				player->Item_slot[inv_pos].count = count;
				count = 0;
			} else {
				player->Item_slot[inv_pos].count = left;
				count -= left;
			}
			SQL_Query_Slow2("INSERT INTO `character_items` VALUES ('%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i')",
				player->Item_slot[inv_pos].id,
				player->Character_Id,
				player->Item_slot[inv_pos].item->id,
				player->Item_slot[inv_pos].count,
				player->Item_slot[inv_pos].enchant[0],
				player->Item_slot[inv_pos].enchant[1],
				inv_pos,
				player->Item_slot[inv_pos].crystal[0],
				player->Item_slot[inv_pos].crystal[1],
				player->Item_slot[inv_pos].crystal[2],
				player->Item_slot[inv_pos].crystal[3],
				player->Item_slot[inv_pos].binded
			);
		}
	}
}
