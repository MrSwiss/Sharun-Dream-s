#include "Player.h"

void S_GET_USER_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_GET_USER_LIST);

	char **row, *sql = "SELECT * FROM `characters`, `character_appearances`, `character_detail_2` WHERE `character_appearances`.`object_id`=`characters`.`object_id` AND `character_detail_2`.`object_id`=`characters`.`object_id` AND `end_ban`<NOW()+0 AND `AccoundId`='%i' AND `Server_ID` IN (0, %i) ORDER BY `Order`";
	char Query[strlen(sql)+MAX_UINT*2];
	sprintf(Query, sql, player->Account_Id, Cfg_Server_ID);
	void *rslt = SQL_Query_Fast(Query, 1);

	char *sql2 = "SELECT `owner_id`, `item_id`, `index`, `enchant_level`, `enchant_max` FROM `character_items`, `characters` WHERE `index` < '39' AND `owner_id` = `characters`.`object_id` AND `AccoundId`='%i' AND `Server_ID` IN (0, %i)  ORDER BY `owner_id`, `index`";
	char Query2[strlen(sql2)+MAX_UINT*2];
	sprintf(Query2, sql2, player->Account_Id, Cfg_Server_ID);
	void *rslt2 = SQL_Query_Fast(Query2, 1);

	ushort nextPos = 0;
	player->Characters_Count = SQL_Max_Row(rslt);
	Player_Writer2(&buffer, player->Characters_Count);

	// remeber where is next head in this  head
	nextPos = buffer.Pos;
	Player_Writer2(&buffer, 0);
	if(!player->Characters_Count) {
		Player_Writer4(&buffer, 0);
	} else {
		Player_Writer4(&buffer, 1);
	}
	Player_Writer4(&buffer, 0);

	Player_Writer1(&buffer, 0);
	Player_Writer4(&buffer, Charcters_Max_Per_Account);
	Player_Writer4(&buffer, 1);
	Player_Writer2(&buffer, 0);
	Player_Writer4(&buffer, 40);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 24);

	for (int i=0; (row = SQL_Next_Row(rslt)); i++) {

		uint owner_id = atoi(row[01]);
		// Write Pos of this Head into previous Head
		if (nextPos) Player_WriterSet2(&buffer, buffer.Pos, nextPos);

		// Write this Pos
		Player_Writer2(&buffer, buffer.Pos);

		// remeber where is next head in this  head
		nextPos = buffer.Pos;
		// Write Next Head Pos 0 by def, updated by next next loop 
		Player_Writer2(&buffer, 0);
		// ZERO (Character Data begin !
		Player_Writer4(&buffer, 0);

		// remeber Name_Pos pos
		ushort Name_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later

		// remeber Detail1_Pos pos
		ushort Detail1_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later
		Player_Writer2(&buffer, 32);	// Detail1 Len

		// remeber Detail2_Pos pos
		ushort Detail2_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later
		Player_Writer2(&buffer, 64);	// Detail2 Len

		// remeber Unknow_Pos pos ... New!
		ushort Unknow_Pos = buffer.Pos;
		buffer.Pos += 2;	// Write Later
/*
00	AccoundId
01	object_id
02	class_id
03	race_id
04	sex
05	char_name
06	heading
07	online_time
08	create_time
09	end_ban
10	end_chat_ban
11	title
12	guild_id
13	access_level
14	level
15	exp
16	hp
17	mp
18	x
19	y
20	z
21	heart
22	attack_counter
23	pvp_count
24	pve_count
25	guild_rank
26	zone_id
27	guild_note
28	karma
29	collect_mining
30	collect_plant
31	collect_energy
32	last_online
33	continent_id
34	Order
35	Server_ID
36	object_id
37	face_color
38	face_skin
39	adorments_skin
40	features_skin
41	features_color
42	voice
43	bone_structure_brow
44	bone_structure_cheekbones
45	bone_structure_jaw
46	bone_structure_jaw_jut
47	ears_rotation
48	ears_extension
49	ears_trim
50	ears_size
51	eyes_width
52	eyes_height
53	eyes_separation
54	Unk1
55	eyes_angle
56	eyes_inner_brow
57	eyes_outer_brow
58	Unk2
59	nose_extension
60	nose_size
61	nose_bridge
62	nose_nostril_width
63	nose_tip_width
64	nose_tip
65	nose_nostril_flare
66	mouth_pucker
67	mouth_position
68	mouth_width
69	mouth_lip_thickness
70	mouse_corners
71	eyes_shape
72	nose_bend
73	bone_structure_jaw_width
74	mouth_gape
*/
		Player_Writer4(&buffer, owner_id); // object_id
		Player_Writer4(&buffer, atoi(row[04])); // sex
		Player_Writer4(&buffer, atoi(row[03])); // race_id
		Player_Writer4(&buffer, atoi(row[02])); // class_id
		Player_Writer4(&buffer, atoi(row[14])); // level

		Player_Writer4(&buffer, 100000); // AD040000
		Player_Writer4(&buffer, 100000); // AD040000

/*		Player_Writer4(&buffer, 1);
		Player_Writer4(&buffer, atoi(row[33]));
		Player_Writer4(&buffer, atoi(row[26]));*/
		Player_Writer4(&buffer, atoi(row[26]));
		Player_WriterF(&buffer, atof2(row[18]));
		Player_WriterF(&buffer, atof2(row[19]));

		Player_Writer8(&buffer, atoi(row[32])); // Last Online Time
		Player_Writer1(&buffer, 0); // 
		Player_Writer8(&buffer, 0); // Deletion Time ?

		Player_Writer4(&buffer, 0xA9C1BD41); // HEAD STUFF
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_WEAPON, owner_id)); // SLOT_WEAPON
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_EARRING_L, owner_id)); // SLOT_EAR;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_EARRING_R, owner_id)); // SLOT_EAR;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_ARMOR, owner_id)); // SLOT_ARMOR;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_GLOVES, owner_id)); // SLOT_GLOVES;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_BOOTS, owner_id)); // SLOT_BOOTS;
		Player_Writer4(&buffer, 0);
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_RING_L, owner_id)); // SLOT_RING_L;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_RING_R, owner_id)); // SLOT_RING_R;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_SHIRT, owner_id)); // SLOT_SHIRT;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_HAT, owner_id)); // SLOT_HAT;
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_MASK, owner_id)); // SLOT_MASK;
//		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_NECKLACE, owner_id)); // SLOT_NECKLACE

		Player_Writer1(&buffer, 101); // HEAD APPEARANCE
		for (int j = 0; j < 6; j++)
			Player_Writer1(&buffer, atoi(row[DB_detail_1+j]));
		Player_Writer1(&buffer, 0); // temp[14]

		Player_Writer8(&buffer, 0); //8
		Player_Writer4(&buffer, 0); // 4
		Player_Writer2(&buffer, 0); // 2

		Player_Writer4(&buffer, 0xA9C06BC1); // BF1857B0
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);
		Player_Writer8(&buffer, 0);

		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_APP_HAT, owner_id)); // new SLOT_APP_HAT
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_APP_MASK, owner_id)); // new SLOT_APP_MASK
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_APP_BACK, owner_id)); // new SLOT_APP_BACK
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_APP_WEAPON, owner_id)); // new SLOT_APP_WEAPON
		Player_Writer4(&buffer, DB_getEquipment(rslt2, SLOT_APP_ARMOR, owner_id)); // new SLOT_APP_ARMOR
		Player_Writer4(&buffer, 0); // new 

		Player_Writer1(&buffer, DB_getEquipment_Ench(rslt2, SLOT_WEAPON, owner_id)); // weapon.getEnchantLevel
		Player_Writer1(&buffer, DB_getEquipment_Ench(rslt2, SLOT_ARMOR, owner_id)); // armor.getEnchantLevel
		Player_Writer1(&buffer, DB_getEquipment_Ench(rslt2, SLOT_GLOVES, owner_id)); // glove.getEnchantLevel
		Player_Writer1(&buffer, DB_getEquipment_Ench(rslt2, SLOT_BOOTS, owner_id)); // boot.getEnchantLevel

		Player_Writer4(&buffer, 416294); //218506); // Rested
		Player_Writer4(&buffer, 2430063); //1815139); // Rest Max
		Player_Writer1(&buffer, 1);

		if (atoi(row[07]) > 60)
//		if (atoi(row[14]) < 2)
			Player_Writer1(&buffer, 0);
		else
			Player_Writer1(&buffer, 1);

		Player_Writer4(&buffer, 0);

		Player_Writer1(&buffer, 1); // new

		Player_Writer4(&buffer, 100);

		Player_Writer4(&buffer, 820); // Achievements
		Player_Writer4(&buffer, 0);
		Player_Writer4(&buffer, 0); // new
		Player_Writer4(&buffer, 0); // new

		Player_WriterSet2(&buffer, buffer.Pos, Name_Pos);
		Player_WriterS(&buffer, row[05], 36);

		Player_WriterSet2(&buffer, buffer.Pos, Detail1_Pos);
		for (int j = 0; j<32; j++)
			Player_Writer1(&buffer, atoi(row[DB_detail_1+6+j]));

		Player_WriterSet2(&buffer, buffer.Pos, Detail2_Pos);
		for (int j = 0; j < 64; j++)
			Player_Writer1(&buffer, atoi(row[DB_detail_1+6+32+1+j]));

		// Unknow... New!
		Player_WriterSet2(&buffer, buffer.Pos, Unknow_Pos);
		Player_Writer2(&buffer, 0);
	}

	SQL_Result_Clear(rslt2);
	SQL_Result_Clear(rslt);

	Player_Send(player, &buffer);
}
