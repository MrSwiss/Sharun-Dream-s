
enum Stat_type {
	Add,
	Mul,
	Unbuffed,
	Buffed,
	STAT_TYPE_MAX
};

enum Stat_Name {
	maxHP,
	maxMP,
	regHp,
	regMp,
	powerFactor,
	defenceFactor,
	impactFactor,
	balanceFactor,
	atkSpd,
	runSpd,
	critRate,
	critRcpt,
	critDmg,
	stunRcpt,
	weakRcpt,
	dmgRcpt,
	atk,
	def,
	cAtk,
	agrMod,
	healPower,
	STAT_NAME_MAX
};

typedef struct stat_t {
	uint	Stamina[2];
	uint	HP;
	uint	MP;

	uint	maxStamina;
	uint	maxHP[2];
	uint	maxMP[2];

	uint	attack[2];
	uint	defense[2];
	uint	impact[2];
	uint	balance[2];
	uint	atkSpeed[2];
	uint	runSpeed[2];

	ushort	buff[3][20];

	float	stat_detail[STAT_TYPE_MAX][STAT_NAME_MAX];
} stat_t;
