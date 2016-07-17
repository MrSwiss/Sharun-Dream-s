#include "Sharun.hpp"

#include <dirent.h>
#include <dlfcn.h>

extern void	(*MySQL_Data_Seek_PLG)(void*, ulong);
extern char**	(*MySQL_Fetch_Row_PLG)(void*);
extern void	(*MySQL_Free_Result_PLG)(void*);
extern void	(*MySQL_Add_PLG)(SQL_QUEUE*);

#ifndef WIN32
#define PluginEXT ".so"
#else
#define PluginEXT ".dll"
void *CallBack_Plugin_Link[] = {
	// sharun_t WIN32 Adaptator
	NULL,
	// DEBUG WIN32 Adaptator
	(void*) DEBUG,
	(void*) DEBUG2,
	// MySQL WIN32 Adaptator
	NULL,
	NULL,
	NULL,
	NULL,
	// NEW_* WIN32 Adaptator
	(void*) new_player,
	(void*) new_packet,
	// SECURITY STOP
	NULL
};
#endif

void Plugin_Scan(std::string Path)
{
	DIR *pdir;
	struct dirent *dirp;
	if ((pdir = opendir(Path.c_str()))) {
		while ((dirp = readdir(pdir))) {
			if ((strcmp(dirp->d_name, ".")) && (strcmp(dirp->d_name, ".."))) {
				std::string filename = Path + "/" + dirp->d_name;
				if (dir_exist(filename.c_str()))
					Plugin_Scan(filename);
				else {
					if (!strcmp(&dirp->d_name[strlen(dirp->d_name)-strlen(PluginEXT)], PluginEXT)) {
						void *handle = dlopen(filename.c_str(), RTLD_LAZY);
						if (handle) {
#ifdef WIN32
							void (*CallBack_Plugin)(void**) = (void(*)(void**))dlsym(handle, "CallBack_Plugin");
							if (CallBack_Plugin)
								(CallBack_Plugin)(CallBack_Plugin_Link);
#endif
							plugin_t* Plugin_l = (plugin_t*)dlsym(handle, "Plugin");
							Plugin_l->handle = handle;
							if (Plugin_l->init)
								Plugin_l->init();
DEBUG("-->\n");
							for (int i=0; Plugin_l->recv && Plugin_l->recv[i].name; i++) {
								DEBUG("  %s\n", Plugin_l->recv[i].name);
								if (Plugin_l->recv[i].func)
									Sharun->OpCodes.Set_Recv(Plugin_l->recv[i].name, Plugin_l->recv[i].func);
							}
/*							dlclose(handle);
							handle = NULL;*/
						}
					}
				}
			}
		}
		closedir(pdir);
	}

  
}


void Plugin_Init()
{
#ifdef WIN32
	CallBack_Plugin_Link[0] = (void*) Sharun;
	CallBack_Plugin_Link[3] = (void*) MySQL_Data_Seek_PLG;
	CallBack_Plugin_Link[4] = (void*) MySQL_Fetch_Row_PLG;
	CallBack_Plugin_Link[5] = (void*) MySQL_Free_Result_PLG;
	CallBack_Plugin_Link[6] = (void*) MySQL_Add_PLG;
#endif
	Plugin_Scan(Sharun->Settings.Dirs.Plugins);
}
