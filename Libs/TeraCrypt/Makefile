# This Makefile will build the MinGW Win32 application.

L_VERSION = 1.0.0

HEADERS = 
DIRS = src
OBJS = 
OBJS_L = 
OBJS_W = 
INCLUDE_DIRS = -I. -I./src
LIB = $(notdir $(shell pwd))

AR = ar
CC = gcc
C+ = g++
RAR = rar
RC = windres
BIN2H = bin2h
WARNS = -Wall
#CPPFLAGS = -O3 
CPPFLAGS = -g ${WARNS} $(MACHDEP) -DL_VERSION=\"$(L_VERSION)\" -DNDEBUG

ifeq ($(OS), Windows_NT)
	BIN2H := ../tools/${BIN2H}.exe
	RAR := ../tools/rar.exe
	CFLAGS += -D WIN32 -D _WIN32_IE=0x0501 -D WINVER=0x600 -D _WIN32_WINNT=0x600 -D UNICODE -D _UNICODE
	OBJS += ${OBJS_W}
	SYS := win32
	DLL = .dll
else
	BIN2H := tools/${BIN2H}
	CFLAGS += -fPIC
	OBJS += ${OBJS_L}
	SYS := unix
	DLL = .so
endif

SRC = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
SRC := $(notdir $(SRC))

OBJS += $(addprefix build/$(SYS)_, $(SRC))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

LIB := $(addprefix lib, $(LIB))
LIB := $(addsuffix _$(SYS).a, $(LIB))
Virg = ,
R_VERSION = $(subst .,$(Virg),$(L_VERSION))

all: build ${LIB}

lib%_$(SYS).a: ${OBJS}
	@echo $(notdir $@)
	@${AR} rc "$@" ${OBJS} ${LDFLAGS}

clean:
	@rm -rf build ${LIB}

build:
	@mkdir $@

build/$(SYS)_%.o: */%.cpp
	@echo $(notdir $@)
	@${C+} ${CPPFLAGS} ${INCLUDE_DIRS} -c $< -o $@
