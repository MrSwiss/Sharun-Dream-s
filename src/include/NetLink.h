typedef struct netlink_t {
	void*	C_Thread;
	SOCKET	sock;
	char	IP[16];
	char*	Head;
	short	Size;
	bool	noHead;
} netlink_t;
