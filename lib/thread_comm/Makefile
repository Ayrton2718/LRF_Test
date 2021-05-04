ALIB = libthread_comm.a
OBJDIR = ./obj/

SOURCE_PATH = ./src/
CC = gcc
CPP = g++
AR = ar
ARFLAGS = -rcs
INCLUDE_PATH = -I../collection_class
LDFLAGS = -lm -pthread 
CFLAGS = -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC

SOURCE = tc_launcher.c
SOURCE += tc_directory.c
SOURCE += tc_share.c
SOURCE += tc_signal.c
SOURCE += tc_initializer.c
SOURCE += tc_type.c

OBJ	= $(subst .c,.o,$(SOURCE))

OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard src/*.h) Makefile thread_comm.h

$(info SOURCE = $(SOURCE))
$(info OBJ = $(OBJ))

$(info OBJS = $(OBJS))
$(info DEPS = $(DEPS))

all: obj $(ALIB)

$(ALIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)%.o: $(SOURCE_PATH)%.cpp $(DEPS)
	$(CPP) $(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: $(SOURCE_PATH)%.c $(DEPS)
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(ALIB) $(OBJDIR)

