# Project environment
# SIMULATOR_ROOT, defined by setup_env.sh
BENCHMARK_ROOT=$(SIMULATOR_ROOT)/benchmark/quick_sort_Homogeneous

# Compiler environment of C/C++
CC=g++
CFLAGS=-Wall -Werror -g -I$(SIMULATOR_ROOT)/interchiplet/includes
LDFLAGS=-lrt -lpthread
INTERCHIPLET_C_LIB=$(SIMULATOR_ROOT)/interchiplet/lib/libinterchiplet_c.a

# C/C++ Source files
QUICK_SORT_SRCS= quick_sort_start.cpp 
QUICK_SORT_OTHER_SRCS= quick_sort_other.cpp

# Object files
QUICK_SORT_OBJS=$(patsubst %.cpp, obj/%.o, $(QUICK_SORT_SRCS))
QUICK_SORT_OTHER_OBJS=$(patsubst %.cpp, obj/%.o, $(QUICK_SORT_OTHER_SRCS))

# Targets
QUICK_SORT_TARGET=bin/quick_sort
QUICK_SORT_OTHER_TARGET=bin/quick_sort_other

all: bin_dir obj_dir quick_sort quick_sort_other

# Quick Sort target
quick_sort: $(QUICK_SORT_OBJS)
	$(CC) $(QUICK_SORT_OBJS) $(INTERCHIPLET_C_LIB) $(LDFLAGS) -o $(QUICK_SORT_TARGET)

# Quick Sort Other target
quick_sort_other: $(QUICK_SORT_OTHER_OBJS)
	$(CC) $(QUICK_SORT_OTHER_OBJS) $(INTERCHIPLET_C_LIB) $(LDFLAGS) -o $(QUICK_SORT_OTHER_TARGET)

# Rule for Quick Sort object
obj/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Directory for binary files.
bin_dir:
	mkdir -p bin

# Directory for object files for C.
obj_dir:
	mkdir -p obj

# Clean generated files.
clean:
	rm -rf bench.txt delayInfo.txt buffer* message_record.txt
	rm -rf proc_r*_t* *.log
clean_all: clean
	rm -rf obj bin

run:
	../../interchiplet/bin/interchiplet quick_sort.yaml