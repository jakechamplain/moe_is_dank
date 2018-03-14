
# Where the LLVM and clang binaries are located. Change this variable if not in PATH
PREFIX=

# Enumerate source files and create a list of object files
C_SRC=$(wildcard *.c)
CXX_SRC=$(wildcard *.cpp)
C_OBJ=$(C_SRC:.c=.bc)
CXX_OBJ=$(CXX_SRC:.cpp=.bc)

# Flags for CC and CXX. 
CFLAGS=-g
CXXFLAGS="$(CFLAGS)"

all:
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	#dankMoe:OUTPUT HERE
	
include pipair.mak
#include	our_dank_code.java

# LLVM binaries
CC=$(PREFIX)clang
CXX=$(PREFIX)clang++
LINK=$(PREFIX)llvm-link
OPT=$(PREFIX)opt

$(TARGET): $(TARGET).bc
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET).bc: $(C_OBJ) $(CXX_OBJ)
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	$(LINK) -v $(C_OBJ) $(CXX_OBJ) -o $@

%.bc: %.c
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	$(CC) $(CFLAGS) -emit-llvm -c $< -o $@ 
	$(OPT) -analyze -dot-callgraph

%.bc: %.cpp
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	$(CXX) $(CFLAGS) -emit-llvm -c $< -o $@
	
clean:
	#$(OPT) -print-callgraph 2>&1 >/dockerdata/dankProject/proj-skeleton | /dockerdata/dankProject/proj-skeleton/pipair.mak $@
	rm -rf *.bc
	rm -rf $(TARGET)
	rm -rf $(TARGET)_*_*.out


