#include "Player.h"

void C_SHOW_ITEM_TOOLTIP_EX_f(player_t *player)
{
	ushort namePos = Player_Reader2(player);

	Player_Reader2(player);
	Player_Reader2(player);
	
	ulong itemID = Player_Reader8(player);

	char name[150];
	player->Packet_R_Pos = player->Packet_R_HeadPos + namePos;
	Player_ReaderSI(player, name);

	S_SHOW_ITEM_TOOLTIP_f(player, name, itemID);

	Player_NextHeaderR(player);
}
