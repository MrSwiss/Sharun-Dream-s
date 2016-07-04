#include "Sharun.hpp"

void test();

plugin_t Plugin = {
	.handle = NULL,
	.init = test,
	.uninit = NULL,
};

void test()
{
	printf("\t-=> A plop from plugin !\n");
	fflush(stdout);
	DEBUG("\t-=> Outside function work !\n");
	DEBUG("\t-=> I'm in \"%s\"\n", Sharun->Settings.Dirs.Plugins.c_str());
}
