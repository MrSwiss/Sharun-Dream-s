L_VERSION = 2.0.0

HEADERS_P = 
DIRS_P = $(wildcard ${SubDirX}src/*)
SRC_P = 
OBJS_P = 
OBJS_L_P = 
OBJS_W_P = resource_plugin.o $(notdir $(wildcard ${SubDirX}src/common/*))
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
LDFLAGS_P += -L. -L./${SubDirX}

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
LDFLAGS_P += -Lbuild/$(SYS) -lSharun

DIRS_P := $(DIRS_P:${SubDirX}src/common=)
DIRS_P := $(wildcard $(DIRS_P)/*)
DIRS_P := $(wildcard $(DIRS_P))
PLG_P := $(foreach dirX,$(DIRS_P),$(addsuffix $(DLL), $(OUT_PLUGIN_PATH)$(dir $(subst ${SubDirX}src/, ,$(dirX)))$(notdir $(dirX))))

PLGD_P = $(addprefix build/,$(subst ${OUT_PLUGIN_PATH},$(SYS)/Plugins/,$(PLG_P:=.d)))

OBJS_P += $(SRC_P)
OBJS_P := $(addprefix build/$(SYS)/Plugins/, $(OBJS_P))
OBJS_P := $(OBJS_P:.c=.o)
OBJS_P := $(OBJS_P:.cpp=.o)
OBJS_P := $(OBJS_P:.rc=.o)

OBJSD_P = $(OBJS_P:.o=.d)
#OBJSD_P := $(OBJSD_P:build/$(SYS)/Plugins/resource_plugin.d=)
OBJSD_P += $(wildcard build/$(SYS)/Plugins/*$(DLL).d)

LIBSHARUN := build/$(SYS)/libSharun.a

WORK2 := $(addsuffix $(EXT), ${WORK})
Virg = ,
R_VERSION = $(subst .,$(Virg),$(L_VERSION))

all: build ${LIBSHARUN} ${WORK2} plugins

-include $(OBJSD_P) $(PLGD_P)

clean:
	@rm -rf build ${WORK2} ${LIBSHARUN} $(OUT_PLUGIN_PATH)*.{so,dll}

build:
	@mkdir -p $@/$(SYS)/Plugins

reset_plugins:
	@rm -f build/$(SYS)/Plugins/{,*/}*$(DLL).d

plugins: build $(OBJSD_P) $(PLGD_P) ${PLG_P} ${LIBSHARUN}


%$(DLL).d:
	@echo "   "$(dir $(subst build/$(SYS)/Plugins/, ,$@))$(notdir $@)
	@mkdir -p $*
	@echo -n $(subst build/$(SYS)/Plugins/,${OUT_PLUGIN_PATH},$(basename $(subst $(OUT_PLUGIN_PATH),${SubDirX}src/,$@)))": " > $@
	@echo -en ""$(foreach dirX, $(subst ${SubDirX}src/,build/$(SYS)/Plugins/,$(OBJS_P)), $(dirX)" \\\\\\n") >> $@
	@echo -en $(foreach dirX, $(subst ${SubDirX}src/,build/$(SYS)/Plugins/,$(subst .c,.o,$(wildcard $(addsuffix /*.c, $(subst build/$(SYS)/Plugins/,${SubDirX}src/,$(subst $(OUT_PLUGIN_PATH),${SubDirX}src/,$*)))))), $(dirX)" \\\\\\n") >> $@
	@echo -en $(foreach dirX, $(subst ${SubDirX}src/,build/$(SYS)/Plugins/,$(subst .cpp,.o,$(wildcard $(addsuffix /*.cpp, $(subst build/$(SYS)/Plugins/,${SubDirX}src/,$(subst $(OUT_PLUGIN_PATH),${SubDirX}src/,$*)))))), $(dirX)" \\\\\\n") >> $@
	@echo "" >> $@
	@echo -n ""$(foreach dirX, $(wildcard $(addsuffix /*.cpp, $(subst build/$(SYS)/Plugins/,${SubDirX}src/,$(subst $(OUT_PLUGIN_PATH),${SubDirX}src/,$*)))), `${C+} -MM ${CPPFLAGS_P} ${INCLUDE_DIRS_P} $(dirX) | sed -e 's/.*:/$(subst /,\/,$(subst .cpp,.o,$(dirX))):/' >> $@ && echo '' >> $@`)
	@echo -e $*"/%.o: ${SubDirX}src/*/*/%.cpp\\n\t@echo \"	\"\$$(dir \$$(subst build/\$$(SYS)/Plugins/, ,\$$@))\$$(notdir \$$@)\\n\t@\$${C+} -c \$${CPPFLAGS_P} \$${INCLUDE_DIRS_P} \$$< -o \$$@\n" >> $@

%$(DLL):
	@echo "	"$(dir $(subst $(OUT_PLUGIN_PATH), ,$@))$(notdir $@)
	@mkdir -p $(dir $@)
	@${C+} ${CPPFLAGS_P} ${INCLUDE_DIRS_P} $^ -shared -Wl,-soname,$(notdir $@) -o $@ ${LDFLAGS_P}

build/$(SYS)/Plugins/%.o: ${SubDirX}src/common/%.cpp build
	@echo "	"$(notdir $@)
	@${C+} -c ${CPPFLAGS_P} ${INCLUDE_DIRS_P} $< -o $@
build/$(SYS)/Plugins/%.d: ${SubDirX}src/common/%.cpp build
	@echo "   "$(notdir $@)
	@${C+} -MM ${CPPFLAGS_P} ${INCLUDE_DIRS_P} $< | sed -e 's/.*:/$(subst /,\/,$(subst .d,.o,$@)):/' > $@

build/$(SYS)/Plugins/resource_plugin.o: ${SubDirX}res/resource.rc build
	@echo "	"$(notdir $@)
	@${RC} -I./${SubDirX}include -I./${SubDirX}res  -DR_VERSION=$(R_VERSION) -DL_VERSION=\\\"$(L_VERSION)\\\" -i $< -o $@
build/$(SYS)/Plugins/resource_plugin.d: ${SubDirX}res/resource.rc build
	@echo "   "$(notdir $@)
	@echo "build/$(SYS)/Plugins/resource_plugin.o: ${SubDirX}res/resource.rc" > $@
