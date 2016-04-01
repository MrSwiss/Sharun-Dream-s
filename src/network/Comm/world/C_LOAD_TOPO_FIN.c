#include "Player.h"

void C_LOAD_TOPO_FIN_f(player_t *player)
{
	Player_NextHeaderR(player);

	*player->Still_Online = time(NULL);

	World_Spam(player);
	OpCodes_Write(player, S_PLAYER_STAT_UPDATE);

	S_INVEN_f(player, 0);
}

/*
	@Override
	public void runImpl()
	{
		// получаем менеджера по игрокам
		PlayerManager playerManager = PlayerManager.getInstance();

		// обрабатываем вход в мир
		playerManager.enterInWorld(getOwner()); ==
	
		player.sendPacket(S_Spawn_Me.getInstance(player), true);

		player.spawnMe();

		//player.sendPacket(Test35.getInstance(player), true);
		player.sendPacket(S_Player_Stat_Update.getInstance(player), true);

		player.sendReuseSkills(); // todo
		player.sendReuseItems(); // todo
		player.sendEffects(); // todo

		if(player.isDead())
		{
			player.broadcastPacket(CharDead.getInstance(player, true));
			player.sendPacket(PlayerDeadWindow.getInstance(), true);
		}

		//if(player.isPK())
		//	player.sendPacket(NameColor.getInstance(NameColor.COLOR_ORANGE, player), true);
		//else 
		if(player.isPvPMode())
			player.sendPacket(NameColor.getInstance(NameColor.COLOR_RED, player), true);
	}
}
	
*/
