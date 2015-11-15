#makefile for random text generator

$(warning .......... Run Makefile ..........)
$(warning default CXX = $(CXX))
$(warning default CURDIR = $(CURDIR))
$(warning default CXXFLAGS = $(CXXFLAGS))

CXX=g++
CXXFLAGS=-c -O2 -W -std=c++11 -I. -I./common

BUILD_DIR:=build

SRCS:= $(wildcard *.cpp)
OBJS:= $(subst cpp,o,$(SRCS))
COMMONSRC:=$(wildcard common/*.cpp)
COMMONOBJ:=$(subst cpp,o,$(COMMONSRC))
TESTSRCS:=$(wildcard test/*.cpp)
TESTOBJS:=$(subst cpp,o,$(TESTSRCS))
TESTER:=$(subst .cpp,,$(TESTSRCS)) #no very good

LINKOBJS:=$(foreach obj,$(COMMONOBJ),$(BUILD_DIR)/$(obj))
LINKOBJS+=$(foreach obj,$(OBJS),$(BUILD_DIR)/$(obj))
LINKOBJS+=$(foreach obj,$(TESTOBJS),$(BUILD_DIR)/$(obj))

	
$(info SRCS         = $(SRCS))
$(info OBJS         = $(OBJS))
$(info COMMONSRC    = $(COMMONSRC))
$(info COMMONOBJ    = $(COMMONOBJ))
$(info TESTSRCS     = $(TESTSRCS))
$(info TESTOBJS     = $(TESTOBJS))
$(info TESTER       = $(TESTER))
$(info LINKOBJS     = $(LINKOBJS))
	
$(info ---------------------------------------------------)

.PHONY: all
all: $(TESTER) $(BUILD_DIR) 

$(TESTER): $(COMMONOBJ) $(OBJS) $(TESTOBJS)
	@echo "Linking [$(LINKOBJS)]"	
	$(CXX) $(LINKOBJS) -o $@

$(BUILD_DIR): 
	@echo "Creating build directory: $@"
	@mkdir -p $@ $@/test $@/common
	
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "[move $@ ---> $(BUILD_DIR)/$@]"
	mv $@ $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(COMMONOBJ) $(OBJS) $(TESTOBJS) $(LINKOBJS) $(TESTER)
	rm -rf *~
