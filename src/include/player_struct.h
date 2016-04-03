typedef struct skill_t {
	uchar	id;
	uchar	type;
	uchar	level;
} skill_t;

typedef struct skill_2_t {
	uint	id;
	void*	next;
} skill_2_t;

typedef struct player_t {
	uchar	struct_type;
	uchar	Characters_Count;
	char	spawned; // >0=spawned , 2=dead
	uint	Player_Id;
	uint	Account_Id;
	uint	Character_Id;
	uint	SUBID;
	uint	Class_Id;
	uint	Race_Id;
	uint	Sex;
	uint	Level;
	ulong	exp;
	ulong	money;
	long	Saved_Time;
	CHAR_LOC_t	World;
	CHAR_LOC_t*	DjW;
	CHAR_LOC_t	DJ;
	skill_t		skill[50];
	skill_2_t*	skill2;
	uchar	collect[3];
	char	name[141];
	char	title[141];
	uchar	appearance[6+32+64];
	mount_t*	mount;
	ushort	moveType[2];
	Item_slot_t	Item_slot[39+20*8];
	uchar	Item_slot_count;
	long	loged;
	long*	Still_Online;
	spawS_t*	spawS;
	void*	spawS_locker;
	stat_t	Stats;
	uchar	exit_type;
	long	exit_time;
	void*	Regen_Thread;
	void*	TERACrypt;
	netlink_t*	NetLink;
	ushort	Right;
	ulong	end_pay;
	short	OpCode_GID;
	void*	send_locker;
	ushort	Packet_R_Pos;
	int	Packet_R_Size;
	ushort	Packet_R_HeadPos;
	byte	Packet_R[Packet_Max_Size];
} player_t;
