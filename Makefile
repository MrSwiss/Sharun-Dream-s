L_VERSION = 2.0.0

HEADERS = 
DIRS = ${SubDir}src ${SubDir}src/DB ${SubDir}src/DB/2_Ram ${SubDir}src/OpCodes ${SubDir}src/Calc ${SubDir}src/network ${SubDir}src/network/teraCrypt ${SubDir}src/network/Client ${SubDir}src/network/Server ${SubDir}src/network/Broadcast ${SubDir}src/network/Comm/*
OpCodes_def_recv_DIR = $(foreach dir,${SubDir}src/network/Comm/*,$(wildcard $(dir)/C_*)) $(wildcard src/network/Client/*.c)
OpCodes_def_send_DIR = $(foreach dir,${SubDir}src/network/Comm/*,$(wildcard $(dir)/S_*)) $(wildcard ${SubDir}src/network/Server/*.c) $(wildcard ${SubDir}src/network/Broadcast/*.c)
OBJS = 
OBJS_L = 
OBJS_W = build/win32_resource.o
INCLUDE_DIRS = -I. -I./${SubDir} -I./${SubDir}src -I./${SubDir}src/include -I./${SubDir}res
EXT = 
WORK = Sharun_Dream
LIBS = $(notdir $(wildcard ${SubDir}Libs/*))
LIB = 

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
LDFLAGS += -lz -lm $(addprefix -L${SubDir}Libs/, $(LIB))

ifeq ($(OS), Windows_NT)
	CFLAGS += -D WIN32 -D _WIN32_IE=0x0501 -D WINVER=0x600 -D _WIN32_WINNT=0x600 -D UNICODE -D _UNICODE
	LDFLAGS += -L./${SubDir} -L./lib/win32 -lws2_32 -static-libgcc -static-libstdc++
#	-mwindows -Wl,--subsystem,windows
	ifeq ($(MSYSTEM), MXE)
		CFLAGS += -DMXE `mysql_config --cflags`
		LDFLAGS += -lpthread `mysql_config --libs`
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
	CFLAGS += -fPIC `mysql_config --cflags`
	LDFLAGS += $(LDFLAGS_C) `mysql_config --libs`
	OBJS += ${OBJS_L}
	SYS := unix
	DLL = .so
endif

CPPFLAGS := ${CFLAGS} -std=c++11
CFLAGS += -std=gnu99

LIBS_SRC = $(addprefix ${SubDir}Libs/, $(LIBS))
LIBS_SRC := ${LIBS_SRC} $(addsuffix /src, $(LIBS_SRC))

INCLUDE_DIRS += $(addprefix -I./, $(LIBS_SRC))
DIRS += $(LIBS_SRC)

SRC = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
SRC += $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
SRC := $(notdir $(SRC))

OBJS += $(addprefix build/$(SYS)_, $(SRC))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

LIB_F := $(addprefix -l, $(LIB))
LIB_F := $(addsuffix _$(SYS), $(LIB_F))
LDFLAGS := $(LIB_F) $(LDFLAGS)

OpCodes_def_recv_DIR := $(basename $(notdir $(OpCodes_def_recv_DIR)))
OpCodes_def_send_DIR := $(basename $(notdir $(OpCodes_def_send_DIR)))

WORK := $(addsuffix $(EXT), $(WORK))
Virg = ,
R_VERSION = $(subst .,$(Virg),$(L_VERSION))

all: build $(addsuffix .a, $(LIB)) ${WORK}

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

Sharun_Dream$(EXT): ${OBJS} $(addsuffix .a, $(LIB))
	@echo $(notdir $@)
	@${C+} -o "$@" ${OBJS} ${LDFLAGS}

clean: $(addsuffix .E, $(LIB))
	@rm -rf build ${WORK}

build:
	@mkdir $@

%.a: lib
	@echo $(notdir $*)
	@${MAKE} -C libs/$*

%.E: lib
	@${MAKE} -C libs/$* clean

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

build/win32_resource.o: ${SubDir}res/resource.rc ${SubDir}res/resource.h
	@echo $(notdir $@)
	@${RC} -I./${SubDir}include -I./${SubDir}res -DR_VERSION=$(R_VERSION) -DL_VERSION=\\\"$(L_VERSION)\\\" -DRX_VERSION=$(R_VERSION) -DLX_VERSION=\\\"$(L_VERSION)\\\" -i $< -o $@

