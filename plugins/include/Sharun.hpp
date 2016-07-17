#ifndef _SHARUN_HPP_
#define _SHARUN_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
#endif

#include "data_type.hpp"
#include "settings.hpp"
#include "DB.hpp"
#include "player.hpp"
#include "packet.hpp"
#include "OpCodes.hpp"
#include "connexion.hpp"
#include "utils.hpp"
#include "plugins.hpp"

/*********************************************************************
 * WARNING :
 *	If something have changed inside "sharun_t" structure or
 *	sub-part, please incrase "sharun_t.version" of 1 more.
 * 		(A plugin without same "sharun_t.version" will be not
 * 		loaded by Sharun Plugin Manager).
 *********************************************************************/
typedef struct sharun_t {
	uint	version = 1;
	settings	Settings;
	database	DB;
	opcodes 	OpCodes;
} sharun_t;

extern sharun_t *Sharun;

#endif // _SHARUN_HPP_
