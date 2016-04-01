#ifndef __OPCODES_H__
#define __OPCODES_H__

#define OpCodes_Max 65536

#include "OpCodes_def_recv.h"
#include "OpCodes_def_send.h"
#include "OpCodes_Gen.h"

typedef struct OpCode_recv_t {
	const char *name;
	void (*callback)(player_t*);
	ushort code;
} OpCode_recv_t;

typedef struct OpCode_send_t {
	const char *name;
	void *(*callback)(player_t*);
	ushort code;
} OpCode_send_t;

extern const char *OpCodes_Name[];
extern OpCode_recv_t OpCode_LstR[];
extern OpCode_send_t OpCode_LstS[];

#endif // __OPCODES_H__
