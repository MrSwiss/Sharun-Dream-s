#include "Sharun.hpp"

#include <dirent.h>
#include <dlfcn.h>

#ifndef WIN32
#define PluginEXT ".so"
#else
#define PluginEXT ".dll"
void *CallBack_Plugin_Link[] = {
	// sharun_t WIN32 Adaptator
	NULL,
	// CHAR16_T WIN32 Adaptator
	(void*)strlen16,
	(void*)strcnmp16,
	(void*)strcmp16,
	(void*)strstr16,
	(void*)strrstr16,
	(void*)strncpy16,
	(void*)strcpy16,
	(void*)strncpy8_16,
	(void*)strcpy8_16,
	(void*)strncpy16_8,
	(void*)strcpy16_8,
	(void*)str_strlen,
	(void*)str_str16_8,
	(void*)str_n_str8_16,
	(void*)str_str8_16,
	// UTILS WIN32 Adaptator
	(void*)l_sizeof,
	(void*)l_malloc,
	(void*)l_free,
	(void*)l_realloc,
	(void*)l_mem2mem,
	(void*)bytedump,
	(void*)strdump,
	(void*)strndump,
	(void*)strupdate,
	(void*)dir_exist,
	(void*)file_exist,
	(void*)file_size,
	(void*)l_mkdir,
	(void*)atof2,
	(void*)atob,
	(void*)is_atoi,
	(void*)microsec,
	(void*)l_realpath,
	(void*)hexdump,
	(void*)DEBUG,
	(void*)DEBUG2,
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
							Plugin_l->init();
							dlclose(handle);
							handle = NULL;
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
	CallBack_Plugin_Link[0] = (void*)Sharun;
#endif
	Plugin_Scan(Sharun->Settings.Dirs.Plugins);
}
