#include "Sharun.hpp"

#ifdef WIN32

sharun_t *Sharun;

extern "C" {

void sharun_set_init(void *Link)
{
	Sharun = (sharun_t*)Link;
}

}

#endif
