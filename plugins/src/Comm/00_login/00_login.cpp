#include "Sharun.hpp"

void* test(void**);

void C_LOGIN_ARBITER_f(player*, packet*);

plugin_t Plugin = {
	.handle = NULL,
	.init = (void(*)())test,
	.uninit = NULL,
	.recv = (plugin_opcode_t[]){
		{ C_LOGIN_ARBITER, "C_LOGIN_ARBITER", (void*(*)(void**)) C_LOGIN_ARBITER_f },
		{ OPC_MAX, NULL, NULL }
	},
	.send = (plugin_opcode_t[]){
		{ I_TELEPORT, "TEST", (void*(*)(void**)) test },
		{ OPC_MAX, NULL, NULL }
	},
};

void* test(void **arg)
{
	printf("\t-=> A plop from plugin !\n");
	fflush(stdout);
	DEBUG("\t-=> Outside function work !\n");
	DEBUG("\t-=> I'm in \"%s\"\n", Sharun->Settings.Dirs.Plugins.c_str());
	return NULL;
}
