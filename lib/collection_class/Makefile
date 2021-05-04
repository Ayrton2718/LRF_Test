ALIB = libcollection_class.a
OBJDIR = ./obj/

SOURCE_PATH = ./src/
CC = gcc
CPP = g++
AR = ar
ARFLAGS = -rcs
LDFLAGS = -lm -pthread 
CFLAGS = -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC


SOURCE = cc_log.c
SOURCE += cc_type.c
SOURCE += cc_locker.c
SOURCE += cc_mem_debugger.c
SOURCE += cc_auto_buffer.c
SOURCE += cc_base_object.c
SOURCE += cc_auto_release.c
SOURCE += cc_null_object.c
SOURCE += cc_bool.c
SOURCE += cc_int_64.c
SOURCE += cc_variable.c
SOURCE += cc_int_32.c
SOURCE += cc_double.c
SOURCE += cc_float.c
SOURCE += cc_string.c
SOURCE += cc_array.c
SOURCE += cc_dictionary.c
SOURCE += cc_queue.c
SOURCE += cc_raw_binary.c
SOURCE += cc_binary.c
SOURCE += cc_bytes.c
SOURCE += cc_object.c
SOURCE += cc_json_serializer.c
SOURCE += cc_initializer.c
SOURCE += cc_dead_queue.c
SOURCE += json_parser/jsmn.c
SOURCE += json_parser/json_parser.c

OBJ = $(subst .c,.o,$(SOURCE))

OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard src/*.h) Makefile collection_class.h

$(info SOURCE = $(SOURCE))
$(info OBJ = $(OBJ))

$(info OBJS = $(OBJS))
$(info DEPS = $(DEPS))

all: obj $(ALIB)

$(ALIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)%.o: $(SOURCE_PATH)%.cpp $(DEPS)
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: $(SOURCE_PATH)%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj obj/json_parser

.PHONY: clean

clean:
	rm -rf $(OBJS) $(ALIB) $(OBJDIR)

