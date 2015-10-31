# Makefile for Huffman Tree

$(warning .......... Run Makefile ..........)
$(warning default CXX = $(CXX))
$(warning default CURDIR = $(CURDIR))
$(warning default CXXFLAGS = $(CXXFLAGS))

BUILD_PATH:=build

compress_SOURCES = compress.cpp HCTree.cpp HCNode.cpp \
                   BitInputStream.cpp BitOutputStream.cpp
                   
uncompress_SOURCES = uncompress.cpp HCTree.cpp HCNode.cpp \
                     BitInputStream.cpp BitOutputStream.cpp

CXX=g++
CXXFLAGS=-c -std=c++11 -Wall -Wextra -g -O2 -I.
LINK_FLAGS= -g -O2
$(warning  CXXFLAGS = $(CXXFLAGS))

SRCS:=$(wildcard *.cpp)
OBJS:=$(subst cpp,o,$(SRCS))
LINKOBJS:=$(foreach obj,$(OBJS),$(BUILD_PATH)/$(obj))
$(warning  OBJS = $(OBJS))
$(warning  SRCS = $(SRCS))
$(warning  LINKOBJS = $(LINKOBJS))

TESTSRCS:=$(wildcard test/*.cpp)
TESTOBJS:=$(subst cpp,o,$(TESTSRCS))
TESTER:=$(subst .o,,$(TESTOBJS))

$(warning   TESTSRCS = $(TESTSRCS))
$(warning   TESTOBJS = $(TESTOBJS))
$(warning   TESTER   = $(TESTER))

$(info ---------------------------------------------------)
.PHONY: all
all: $(BUILD_PATH) $(TESTER)

$(TESTER): $(OBJS) $(TESTOBJS)  
	@echo "Linking: $@ [$(LINKOBJS) $(BUILD_PATH)/$@.o]"
	@$(CXX)  $(LINKOBJS) $(BUILD_PATH)/$@.o -o $@
	
$(BUILD_PATH): 
	@echo "Creating build directory: $@"
	@mkdir -p $@ $@/test
	
%.o : %.cpp
	@echo "compiling [ $< ==> $@ ]"
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "move [$@ ---> $(BUILD_PATH)/$@]"
	mv $@	$(BUILD_PATH)/$@
		
.PHONY: clean
clean:
	@$(RM) -rf $(BUILD_PATH) $(TESTER)
	@$(RM) -rf *~ *.o test/*~
	
