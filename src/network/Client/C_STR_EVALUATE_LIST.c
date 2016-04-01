#include "Player.h"

void C_STR_EVALUATE_LIST_f(player_t *player)
{
	Player_Reader4(player);
	Player_Reader4(player);
	Player_Reader4(player);
	Player_Reader2(player);
	uint type = Player_Reader4(player);
	WCHAR *Wname = (WCHAR*)&player->Packet_R[player->Packet_R_Pos];
	S_STR_EVALUATE_LIST_f(player, type, Wname);
	Player_NextHeaderR(player);
}
