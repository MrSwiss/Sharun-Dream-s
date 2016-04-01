
typedef struct mount_t {
	uint	Skill_Id;
	ushort	Mount_Id;
	ushort	Speed;
	uchar	R_Stamina;
	uchar	R_HP;
	uchar	R_MP;
	void *next;
} mount_t;
