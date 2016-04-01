/*
00 id
01 name
02 huntingZone
03 class
04 level
05 attack
06 impact
07 defense
08 balance
09 hpBase
10 size
11 scale
12 skills
13 npc_id
14 npc_type
15 x
16 y
17 z
18 map_id
19 heading
*/
typedef struct NPC_t {
	uchar	struct_type;
	char	spawned; // >1=spawned , 0=dead
	uint	Id;
	uint	SUBID;
	uint	npc_id;
	uint	npc_type;
	char	name[141];
	int	huntingZone;
	ushort	level;
	uint	hpBase;
	uint	attack;
	uint	impact;
	uint	defense;
	uint	balance;
	CHAR_LOC_t	World;
	short*	skills;
	void*	next;
} NPC_t;
