
typedef struct Item_t {
	uint	id;
	char	category[30];
	uchar	level;
	char	type[30];
	short	cooldown;
	short	rank;
	char	rareGrade[10];
	uchar	slotLimit;
	short	maxStack;
	uchar	ability;
	uint	buyPrice;
	uint	sellPrice;
	uint	skill_id;
	uchar	c_slots;
	ushort	atk;
	ushort	def;
	ushort	bal;
	ushort	ipt;
	uchar	bind;
	void*	next;      
} Item_t;

typedef struct Item_slot_t {
	uint	id;
	Item_t*	item;
	ushort	count;
	uchar	enchant[2];
	uint	crystal[4];
	uint	binded;
	char	binder[141];
} Item_slot_t;

enum ITEM_CAN {
	ITEM_CAN_TRADE	 = 1,
	ITEM_CAN_DESTROY = 2,
	ITEM_CAN_BANK	 = 4,
	ITEM_CAN_GUILD	 = 8
};
