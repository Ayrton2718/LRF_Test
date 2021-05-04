ALIB = libprocess_comm.a
OBJDIR = ./obj/

SOURCE_PATH = ./src/
CC = gcc
CPP = g++
AR = ar
ARFLAGS = -rcs
INCLUDE_PATH = -I../collection_class
INCLUDE_PATH += -I../thread_comm
LDFLAGS = -lm -pthread 
CFLAGS = -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC

SOURCE = pc_requester.c
SOURCE += pc_server.c
SOURCE += pc_type.c
SOURCE += pc_initializer.c

OBJ	= $(subst .c,.o,$(SOURCE))

OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard src/*.h) Makefile process_comm.h

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

