SRC_DIR=$(HOME)/workspace/machine_learning-master/classification/decisiontree

CXX=g++
CXXFLAGS=-c -W -std=c++11 -I.  -O2

TESTER=DecisionTreeTester
SRCS=IndexOutOfBound.cpp	\
	 StringDataReader.cpp	\
	 Timing.cpp	\
	 Node.cpp	\
	 DecisionTree.cpp	\
	 test/$(TESTER).cpp

OBJS=IndexOutOfBound.o	\
	 StringDataReader.o	\
	 Timing.o	\
	 Node.o	\
	 DecisionTree.o	\
	 test/$(TESTER).o

.PHONY: dt
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TESTER)
	rm -rf *~
