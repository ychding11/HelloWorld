SRC_DIR=$(HOME)/workspace/HelloWorld/ml/knn

CXX=g++
#CXXFLAGS=-c -g -W -I$(SRC_DIR)/utility -I.
CXXFLAGS=-c -O2 -W -std=c++11 -I. -DCHECK_IO

TESTER=KNearestNeighboursTester
SRCS=InputDataSet.cpp	\
	KNearestNeighbours.cpp	\
	$(SRC_DIR)/Timing.cpp	\
	test/$(TESTER).cpp

OBJS=InputDataSet.o	\
	KNearestNeighbours.o	\
	$(SRC_DIR)/Timing.o	\
	test/$(TESTER).o

.PHONY: knn
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TESTER)
	rm -rf *~
