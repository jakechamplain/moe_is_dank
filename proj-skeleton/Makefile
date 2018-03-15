
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

#stuff for java
SRC = ./src
BIN = ./bin
JC = javac
JFLAGS = -d $(BIN)
CLASSES = \
	$(SRC)/Main.java \
	
CLASS_FILES=$(CLASSES:$(SRC)/%.java=$(BIN)/%.class)

all:	
	$(CC) pipair.c -o pipairBinary.bc 
	#so either we have to compile it into binary executable (this above) or to bitcode (this below). however, the pdf says "You can implement pipair as an executable binary, or you can implement pipair as a bash script." so let's go with the binary 
	#$(CC) $(CFLAGS) -emit-llvm -c pipair.c -o pipairBinary.bc
	./pipair
	
include pipair.mak

# LLVM binaries
CC=$(PREFIX)clang
CXX=$(PREFIX)clang++
LINK=$(PREFIX)llvm-link
OPT=$(PREFIX)opt

$(TARGET): $(TARGET).bc
	$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET).bc: $(C_OBJ) $(CXX_OBJ)
	$(LINK) -v $(C_OBJ) $(CXX_OBJ) -o $@

%.bc: %.c
	$(CC) $(CFLAGS) -emit-llvm -c $< -o $@ 

%.bc: %.cpp
	$(CXX) $(CFLAGS) -emit-llvm -c $< -o $@
	
clean:
	rm -f pipairbinary.bc
	rm -rf *.bc
	rm -rf $(TARGET)
	rm -rf $(TARGET)_*_*.out


