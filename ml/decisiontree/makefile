#Makefile for DecisionTree

SRC_DIR=$(HOME)/workspace/machine_learning-master/classification/decisiontree
$(warning .......... Run Makefile ..........)
$(warning default CXX = $(CXX))
$(warning default CURDIR = $(CURDIR))
$(warning default CXXFLAGS = $(CXXFLAGS))

BUILD_PATH:=build

CXX=g++
CXXFLAGS=-c -W -std=c++11 -I. -g -O2
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
all : $(BUILD_PATH) $(TESTER)

$(BUILD_PATH): 
	@echo "Creating build directory: $@"
	@mkdir -p $@ $@/test
	
$(TESTER): $(OBJS) $(TESTOBJS)  
	@echo "Linking: $@ [$(LINKOBJS) $(BUILD_PATH)/$@.o]"
	@$(CXX)  $(LINKOBJS) $(BUILD_PATH)/$@.o -o $@
	
%.o : %.cpp
	@echo "compiling [ $< ---> $@ ]"
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "moving    [$@ ---> $(BUILD_PATH)/$@]"
	mv $@	$(BUILD_PATH)/$@

.PHONY: clean
clean:
clean:
	@$(RM) -rf $(BUILD_PATH) $(TESTER)
	@$(RM) -rf *~ *.o test/*~
