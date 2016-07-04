L_VERSION = 2.0.0

HEADERS_P = 
DIRS_P = $(wildcard ${SubDirX}src/*)
SRC_P = 
OBJS_P = 
OBJS_L_P = 
OBJS_W_P = CallBack_Plugin_2.o resource_plugin.o
INCLUDE_DIRS_P = -I. -I./${SubDirX} -I./${SubDirX}include -I./${SubDirX}src -I./${SubDirX}src/include -I./${SubDirX}res
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

CFLAGS_P += ${WARNS} $(MACHDEP) -DL_VERSION=\"$(L_VERSION)\" -DNO_ERROR_FILE
#-DNO_ERROR
LDFLAGS_P += -lz -lm

ifeq ($(OS), Windows_NT)
	CFLAGS_P += -DDLL_VERSION -D WIN32 -D _WIN32_IE=0x0501 -D WINVER=0x600 -D _WIN32_WINNT=0x600 -D UNICODE -D _UNICODE
	LDFLAGS_P += -L./${SubDirX} -L./lib/win32 -lws2_32 -static-libgcc -static-libstdc++
#	-mwindows -Wl,--subsystem,windows
	ifeq ($(MSYSTEM), MXE)
		CFLAGS_P += -DMXE
		LDFLAGS_P +=
	else
		CFLAGS_P += -DPTW32_STATIC_LIB -DNDEBUG
		LDFLAGS_P += -L/usr/lib
	endif
	OBJS_P += ${OBJS_W_P}
	SYS := win32
	EXT := .exe
	DLL := .dll
	BIN_PATH = /bin/
else
	CFLAGS_P += -fPIC
	LDFLAGS_P += -fPIC
	OBJS_P += ${OBJS_L_P}
	SYS := unix
	DLL := .so
endif

CPPFLAGS_P := ${CFLAGS_P} -std=c++11
CFLAGS_P += -std=gnu99

PLG_P = $(foreach dir,$(DIRS_P),$(addsuffix $(DLL), $(dir)))
PLG_P := $(PLG_P:common$(DLL)=)

OBJS_P += $(SRC_P)
OBJS_P := $(addprefix build/$(SYS)_, $(OBJS_P))
OBJS_P := $(OBJS_P:.c=.o)
OBJS_P := $(OBJS_P:.cpp=.o)
OBJS_P := $(OBJS_P:.rc=.o)

WORK2 := $(addsuffix $(EXT), ${WORK})
Virg = ,
R_VERSION = $(subst .,$(Virg),$(L_VERSION))

all: build ${WORK2} plugins

clean:
	@rm -rf build ${WORK2} $(OUT_PLUGIN_PATH)*.{so,dll}

build:
	@mkdir $@

plugins: build ${PLG_P}

%$(DLL): ${OBJS_P}
	@echo "	"$(notdir $@)
	@${C+} ${CPPFLAGS_P} ${LDFLAGS_P} ${INCLUDE_DIRS_P} $(wildcard $(addsuffix /*.c, $(basename $@))) $(wildcard $(addsuffix /*.cpp, $(basename $@))) ${RCF} ${OBJS_P} -shared -Wl,-soname,$@ -o $(addprefix $(OUT_PLUGIN_PATH),$(notdir $@))

build/$(SYS)_CallBack_Plugin_1.o: ${SubDirX}src/common/CallBack_Plugin.c
	@echo "	"$(notdir $@)
	@${CC} ${CFLAGS_P} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_CallBack_Plugin_2.o: ${SubDirX}src/common/CallBack_Plugin.cpp
	@echo "	"$(notdir $@)
	@${C+} ${CPPFLAGS_P} ${INCLUDE_DIRS} -c $< -o $@

build/$(SYS)_resource_plugin.o: ${SubDirX}res/resource.rc
	@echo "	"$(notdir $@)
	@${RC} -I./${SubDirX}include -I./${SubDirX}res  -DR_VERSION=$(R_VERSION) -DL_VERSION=\\\"$(L_VERSION)\\\" -i $< -o $@
