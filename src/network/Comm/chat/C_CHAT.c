#include "Player.h"

void C_CHAT_f(player_t *player)
{
	Player_Reader2(player); // Text Pos

	short type = Player_Reader4(player);

	char text[Packet_Max_Size-20-36];
	Player_ReaderSI(player, text);
	Player_NextHeaderR(player);

	World_Chat(player, type, text);
}
