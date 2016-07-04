#include "Sharun.hpp"

#include <dirent.h>
#include <dlfcn.h>

#ifndef WIN32
#define PluginEXT ".so"
#else
#define PluginEXT ".dll"
void *utils_init_Link[] = {
	NULL,
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
							void (*utils_init_l)(void**) = (void(*)(void**))dlsym(handle, "utils_init");
							if (utils_init_l)
								(utils_init_l)(utils_init_Link);
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
	utils_init_Link[0] = (void*)Sharun;
#endif
	Plugin_Scan(Sharun->Settings.Dirs.Plugins);
}
