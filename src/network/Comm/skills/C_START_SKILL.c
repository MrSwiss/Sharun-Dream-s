#include "Player.h"

void C_START_SKILL_f(player_t *player)
{
	uint Skill_ID = Player_Reader4(player);
	Skill_ID -= 67108864;

//	Player_Reader2(player); //Heading
/*
	Player_ReaderF(player); // X
	Player_ReaderF(player); // Y
	Player_ReaderF(player); // Z

	Player_ReaderF(player); // target X
	Player_ReaderF(player); // target Y
	Player_ReaderF(player); // target Z

	Player_Reader2(player);
	Player_Reader1(player);

	Player_Reader4(player); // target ID
	Player_Reader4(player); // target SubID
*/
	Player_NextHeaderR(player);
	World_PlayerSkill(player, NULL, Skill_ID);
}
/*
	@Override
	public void runImpl()
	{
		Player player = getPlayer();

		if(player == null)
			return;

		Skill skill = player.getSkill(skillId);

		if(skill == null)
		{
			player.sendMessage("Этого скила у вас нету.");
			return;
		}

		float startX = getStartX();
		float startY = getStartY();
		float startZ = getStartZ();

		Character target = null;

		if(player.getSquareDistance(startX, startY) > Config.WORLD_MAX_SKILL_DESYNC)
		{
			startX = player.getX();
			startY = player.getY();
		}

		int targetId = getTargetId();
		int targetSubId = getTargetSubId();

		if(targetId > 0)
			target = World.getAroundById(Character.class, player, targetId, targetSubId);

		player.setTarget(target);

		float targetX = getTargetX();
		float targetY = getTargetY();
		float targetZ = getTargetZ();

		int heading = getHeading();

		if(target != null && skill.isCorrectableTarget() && targetSubId == Config.SERVER_PLAYER_SUB_ID)
		{
			targetX = target.getX();
			targetY = target.getY();
			targetZ = target.getZ() + target.getGeomHeight() * 0.5F;

			heading = Angles.calcHeading(startX, startY, targetX, targetY);
		}

		player.getAI().startCast(startX, startY, startZ, skill, 0, heading, targetX, targetY, targetZ);
	}

*/