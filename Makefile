CC=gcc
CPP=g++

SOURCE_DIR = RMM/drivers/can_smbus/src
SOURCE_DIR += RMM/drivers/can_smbus/cplusplus
SOURCE_DIR += RMM/drivers/lrf

SOURCE = $(wildcard $(addsuffix /*.c,$(SOURCE_DIR)))
SOURCE += $(wildcard $(addsuffix /*.cpp,$(SOURCE_DIR)))
HEADER = $(wildcard $(addsuffix /*.h,$(SOURCE_DIR)))

LDFLAGS= -lm -lpthread
LIBRALY_PATH = -Llib/process_comm/ -lprocess_comm
LIBRALY_PATH += -Llib/thread_comm/ -lthread_comm
LIBRALY_PATH += -Llib/collection_class/ -lcollection_class
INCLUDE_PATH = -Ilib/process_comm/
INCLUDE_PATH += -Ilib/thread_comm/
INCLUDE_PATH += -Ilib/collection_class/
OPENCV_PACKAGE = -std=c++11 `pkg-config opencv4 --cflags` `pkg-config opencv4 --libs`
CFLAGS = -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC

OBJ = $(addsuffix .o,$(basename $(SOURCE)))

$(info SOURCE = $(SOURCE))
$(info OBJ = $(OBJ))
$(info HEADER = $(HEADER))

all: main.out

main.out: main.o $(OBJ) $(HEADER) Makefile
	$(CPP) $(INCLUDE_PATH) $(CFLAGS) $(OBJ) $(OPENCV_PACKAGE) main.o $(LIBRALY_PATH) $(LDFLAGS) -o $@ 

%.o: %.cpp $(HEADER) Makefile
	$(CPP) $(INCLUDE_PATH) $(CFLAGS) $(OPENCV_PACKAGE) -c $< -o $@

%.o: %.c $(HEADER) Makefile
	$(CC) $(INCLUDE_PATH) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf main.out $(OBJ)
