L_VERSION = 2.0.0

SubDirX = ${SubDir}plugins/
WORK = Sharun_Dream
OUT_PLUGIN_PATH = Sharun_Saves/Plugins/
include ${SubDirX}Makefile

HEADERS = 
DIRS = ${SubDir}src ${SubDir}src/DB ${SubDir}src/DB/2_Ram ${SubDir}src/OpCodes ${SubDir}src/Calc ${SubDir}src/network ${SubDir}src/network/teraCrypt ${SubDir}src/network/Broadcast
OBJS = 
OBJS_L = 
OBJS_W = build/win32_resource.o
INCLUDE_DIRS = -I. -I./${SubDir} -I./${SubDir}include -I./${SubDir}src -I./${SubDir}src/include -I./${SubDir}plugins/include -I./${SubDir}res
EXT = 

CC = gcc
C+ = g++
RC = windres
WARNS = -Wall

ifeq ($(DEBUG), yes)
	CFLAGS = -g -g3 -ggdb -gdwarf-2
	LDFLAGS = -g -g3 -ggdb -gdwarf-2
else
	CFLAGS = -O3 -s
	LDFLAGS = -O3 -s
endif

CFLAGS += ${WARNS} $(MACHDEP) -DL_VERSION=\"$(L_VERSION)\" -DNO_ERROR_FILE
#-DNO_ERROR
LDFLAGS += -lz -lm -ldl

ifeq ($(OS), Windows_NT)
	CFLAGS += -D WIN32 -D _WIN32_IE=0x0501 -D WINVER=0x600 -D _WIN32_WINNT=0x600 -D UNICODE -D _UNICODE
	LDFLAGS += -L./${SubDir} -L./lib/win32 -lws2_32 -static-libgcc -static-libstdc++ -Wl,--export-all-symbols -Wl,--enable-auto-import
#	-mwindows -Wl,--subsystem,windows
	ifeq ($(MSYSTEM), MXE)
		CFLAGS += -DMXE `mysql_config --cflags`
		LDFLAGS += `mysql_config --libs` -lpthread
	else
		CFLAGS += -DPTW32_STATIC_LIB -DNDEBUG
		LDFLAGS += -L/usr/lib -lpthreadGC2 -lmysql
	endif
	OBJS += ${OBJS_W}
	SYS := win32
	EXT = .exe
	DLL = .dll
	BIN_PATH = /bin/
else
	CFLAGS += `mysql_config --cflags`
	LDFLAGS += `mysql_config --libs` -Wl,--export-dynamic
	OBJS += ${OBJS_L}
	SYS := unix
	DLL = .so
endif

CPPFLAGS := ${CFLAGS} -std=c++11
CFLAGS += -std=gnu99

SRC = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
SRC += $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
SRC := $(notdir $(SRC))

OBJS += $(addprefix build/$(SYS)_, $(SRC))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

WORK2 := $(addsuffix $(EXT), ${WORK})
Virg = ,
R_VERSION = $(subst .,$(Virg),$(L_VERSION))

zip:
	@echo Make src.tar.bz2
	@tar -cjf src.tar.bz2 src

unzip:
	@echo Deleting SRC dir
	@rm -r src
	@echo Extracting src.tar.bz2
	@tar -xjf src.tar.bz2

run: all
	./Sharun_Dream 1

valgrind: all
	valgrind --tool=memcheck --track-origins=yes --main-stacksize=100 ./Sharun_Dream 1

$(WORK2): ${OBJS}
	@echo $(notdir $@)
	@${C+} -o "$@" ${OBJS} ${LDFLAGS}

build/$(SYS)_%.o: */%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_%.o: */*/%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */*/%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_%.o: */*/*/%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */*/*/%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_%.o: */*/*/*/%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */*/*/*/%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_%.o: */*/*/*/*/%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */*/*/*/*/%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_%.o: */*/*/*/*/*/%.c
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)_%.o: */*/*/*/*/*/%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_resource.o: ${SubDir}res/resource.rc ${SubDir}res/resource.hpp
	@echo $(notdir $@)
	@${RC} -I./${SubDir}include -I./${SubDir}res -DR_VERSION=$(R_VERSION) -DL_VERSION=\\\"$(L_VERSION)\\\" -i $< -o $@

