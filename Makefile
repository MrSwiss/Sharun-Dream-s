L_VERSION = 2.0.0

SubDirX = ${SubDir}plugins/
WORK = Sharun_Dream
OUT_PLUGIN_PATH = Sharun_Saves/Plugins/
include ${SubDirX}Makefile

HEADERS = 
DIRS = ${SubDir}src ${SubDir}src/DB ${SubDir}src/DB/2_Ram ${SubDir}src/OpCodes ${SubDir}src/Calc ${SubDir}src/network ${SubDir}src/network/teraCrypt ${SubDir}src/network/Broadcast
OBJS = 
OBJS_L = 
OBJS_W = resource.o
INCLUDE_DIRS = -I. -I./${SubDir} -I./${SubDir}include -I./${SubDir}src -I./${SubDir}src/include -I./${SubDir}plugins/include -I./${SubDir}res
EXT = 

CC = gcc
C+ = g++
AR = ar
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
LDFLAGS += -lz -lm -ldl -L. -L./${SubDir}

ifeq ($(OS), Windows_NT)
	CFLAGS += -D WIN32 -D _WIN32_IE=0x0501 -D WINVER=0x600 -D _WIN32_WINNT=0x600 -D UNICODE -D _UNICODE
	LDFLAGS += -L./lib/win32 -lws2_32 -static-libgcc -static-libstdc++ -Wl,--export-all-symbols -Wl,--enable-auto-import
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
	CFLAGS += -fPIC `mysql_config --cflags`
	LDFLAGS += -fPIC `mysql_config --libs` -Wl,--export-dynamic
	OBJS += ${OBJS_L}
	SYS := unix
	DLL = .so
endif

CPPFLAGS := ${CFLAGS} -std=c++11
CFLAGS += -std=gnu99
LDFLAGS += -Lbuild/$(SYS) -lSharun

SRC = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
SRC += $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
SRC := $(notdir $(SRC))

OBJS += $(SRC)
OBJS := $(addprefix build/$(SYS)/, $(OBJS))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)
OBJS := $(OBJS:.rc=.o)

DIRS_LIB = ${SubDir}libSharun ${SubDir}libSharun/network ${SubDir}libSharun/DB
OBJS_LIB = $(foreach dir,${DIRS_LIB},$(wildcard $(dir)/*.c))
OBJS_LIB += $(foreach dir,${DIRS_LIB},$(wildcard $(dir)/*.cpp))
OBJS_LIB := $(notdir $(OBJS_LIB))
OBJS_LIB := $(addprefix build/$(SYS)/, $(OBJS_LIB))
OBJS_LIB := $(OBJS_LIB:.c=.o)
OBJS_LIB := $(OBJS_LIB:.cpp=.o)
LIBSHARUN := build/$(SYS)/libSharun.a

OBJS_I = $(OBJS_LIB:.o=.d) $(OBJS:.o=.d)

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
	./$(WORK2) 1

valgrind: all
	valgrind --tool=memcheck --track-origins=yes --main-stacksize=100 ./$(WORK2) 1

$(LIBSHARUN): ${OBJS_LIB}
	@echo $(notdir $@)
	@rm -f ${WORK2} ${LIBSHARUN} $(OUT_PLUGIN_PATH){,*/}*$(DLL)
	@${AR} rc $@ $^

$(WORK2): ${OBJS}
	@echo $(notdir $@)
	@${C+} -o "$@" $^ ${LDFLAGS}

-include $(OBJS_I)

build/$(SYS)/%.o: */%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/%.o: */*/%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */*/%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */*/%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */*/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/%.o: */*/*/%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */*/*/%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */*/*/%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */*/*/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/%.o: */*/*/*/%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */*/*/*/%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */*/*/*/%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */*/*/*/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/%.o: */*/*/*/*/%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */*/*/*/*/%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */*/*/*/*/%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */*/*/*/*/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/%.o: */*/*/*/*/*/%.c build
	@echo $(notdir $@)
	@${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.o: */*/*/*/*/*/%.cpp build
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
build/$(SYS)/%.d: */*/*/*/*/*/%.c build
	@echo "   "$(notdir $@)
	@${CC} -MM ${CFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@
build/$(SYS)/%.d: */*/*/*/*/*/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS} ${INCLUDE_DIRS} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/resource.o: ${SubDir}res/resource.rc ${SubDir}res/resource.hpp build
	@echo $(notdir $@)
	@${RC} -I./${SubDir}include -I./${SubDir}res -DR_VERSION=$(R_VERSION) -DL_VERSION=\\\"$(L_VERSION)\\\" -i $< -o $@
build/$(SYS)/resource.d: ${SubDir}res/resource.rc build
	@echo "   "$(notdir $@)
	@echo -e "build/$(SYS)/Plugins/resource.o: ${SubDir}res/resource.rc \\\\\\n ${SubDir}res/resource.hpp" > $@


