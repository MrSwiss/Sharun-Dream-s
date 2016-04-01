#include "Player.h"

void C_WHISPER_f(player_t *player)
{
	Player_Reader4(player);

	char text[Packet_Max_Size-20-36];
	char name[141];
	Player_ReaderSI(player, name);
	Player_ReaderSI(player, text);

	Player_NextHeaderR(player);

	if (!strcmp(player->name, name)) {
		S_CHAT_to_Me_f(player, SYSTEM_CHAT, "Your can't send whisper to yourself !");
		return;
	}

	for (int i=0; i<Player_Max; i++) {
		if ( Players[i]
		  && Players[i]->loged
		  && !strcmp(Players[i]->name, name) ) {
			S_WHISPER_f(player, Players[i], text);
			return;
		}
	}

	char tmp[21+strlen(name)];
	sprintf(tmp, "\"%s\" is not online.", name);
	S_CHAT_to_Me_f(player, SYSTEM_CHAT, tmp);
}
