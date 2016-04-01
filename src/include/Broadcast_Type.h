
typedef struct spawS_t {
	void*	link;
	void*	next;
} spawS_t;

enum BROADCAST_TYPE
{
	BC_TYPE_MAIN	= 1,
	BC_TYPE_NOT_ME	= 2,
	BC_TYPE_PARTY	= 4,
	BC_TYPE_RAID	= 8,
	BC_TYPE_GUILD	= 16,
	BC_TYPE_ZONE	= 32,
	BC_TYPE_ARROUND	= 64,
	BC_TYPE_TARGET	= 128,
	BC_TYPE_FORCE	= 256
};
