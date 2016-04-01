#include "Player.h"

void C_SAVE_CHAT_SETTING_f(player_t *player)
{
	ushort size = 0, actual = 0;
	memcpy(&size, player->Packet_R + player->Packet_R_HeadPos, 2);
	size -= 4;
	char data[size];
	actual = player->Packet_R_Size - player->Packet_R_HeadPos - 4;
	if (actual > size) actual = size;
	memcpy(data, player->Packet_R + player->Packet_R_HeadPos + 4, actual);
	player->Packet_R_Pos = player->Packet_R_HeadPos + 4 + actual;
	while (actual < size) {
		Player_Recv(player);
		player->Packet_R_Pos = size - actual;
		if (player->Packet_R_Pos > player->Packet_R_Size) player->Packet_R_Pos = player->Packet_R_Size;
		memcpy(data + actual, player->Packet_R, player->Packet_R_Pos);
		actual += player->Packet_R_Pos;
	}
	char FName[strlen(Cfg_Server_DirSave)+14+10+1];
	sprintf(FName, "%s/CHAT_SETTING/%i", Cfg_Server_DirSave, player->Charcter_Id);
	FILE *fp = fopen(FName, "wb");
	if (fp) {
		ushort out = fwrite(data, 1, size, fp);
		if (out < size) DEBUG("%s (%i) :: Can't write output file : \"%s\" %i/%i\n", __func__, __LINE__, FName, out, size);
		fclose(fp);
	} else DEBUG("%s (%i) :: Can't open output file : \"%s\"\n", __func__, __LINE__, FName);
	S_LOAD_CHAT_SETTING_f(player);
//	USER_SETTING_Saved(player, S_LOAD_CHAT_SETTING);
	Player_NextHeaderR(player);
}
