#include "Player.h"

void C_SELECT_USER_f(player_t *player)
{
	player->Charcter_Id = Player_Reader4(player);
	Player_NextHeaderR(player);

	OpCodes_Write(player, S_SELECT_USER);
	OpCodes_Write(player, S_BROCAST_GUILD_FLAG);
	OpCodes_Write(player, S_CURRENT_ELECTION_STATE);
	OpCodes_Write(player, S_ONGOING_HUNTING_EVENT_LIST);
	OpCodes_Write(player, S_LOGIN);
//	S_INVEN_f(player, 0);
	OpCodes_Write(player, S_SKILL_LIST);
	OpCodes_Write(player, S_AVAILABLE_SOCIAL_LIST);
	OpCodes_Write(player, S_CLEAR_QUEST_INFO);

	OpCodes_Write(player, S_ARTISAN_SKILL_LIST);
	OpCodes_Write(player, S_ARTISAN_RECIPE_LIST);
	OpCodes_Write(player, S_NPCGUILD_LIST);
	OpCodes_Write(player, S_PET_INCUBATOR_INFO_CHANGE);
	OpCodes_Write(player, S_PET_INFO_CLEAR);
	OpCodes_Write(player, S_VIRTUAL_LATENCY);
	OpCodes_Write(player, S_MOVE_DISTANCE_DELTA);
	OpCodes_Write(player, S_MY_DESCRIPTION);
	OpCodes_Write(player, S_F2P_PremiumUser_Permission);
	OpCodes_Write(player, S_NOTIFY_CHANGE_CLASS_AND_ELITE);
	OpCodes_Write(player, S_MASSTIGE_STATUS);
	OpCodes_Write(player, S_AVAILABLE_EVENT_MATCHING_LIST);
	OpCodes_Write(player, S_FESTIVAL_LIST);

	OpCodes_Write(player, S_LOAD_TOPO);
	OpCodes_Write(player, S_LOAD_HINT);
	OpCodes_Write(player, S_ACCOUNT_BENEFIT_LIST);
	OpCodes_Write(player, S_SEND_USER_PLAY_TIME);
	OpCodes_Write(player, S_PCBANGINVENTORY_DATALIST);
	OpCodes_Write(player, S_COMPLETED_MISSION_INFO);
	OpCodes_Write(player, S_DAILY_QUEST_COMPLETE_COUNT); // = S_DAILY_QUEST_COMPLETE_COUNT
	OpCodes_Write(player, S_AVAILABLE_EVENT_MATCHING_LIST);
	OpCodes_Write(player, S_LOAD_CLIENT_USER_SETTING);
	OpCodes_Write(player, S_LOAD_CHAT_SETTING);

}
