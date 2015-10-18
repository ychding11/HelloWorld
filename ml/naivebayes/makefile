SRC_DIR=$(HOME)/workspace/HelloWorld/ml/naivebayes

CXX=g++
CXXFLAGS=-c -g -W -std=c++11 -I/usr/include -I. -DDEBUG -DUSE_VECTOR
#CXXFLAGS=-c -O2 -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I/usr/include -I.

TESTER=OfflineNBCTester
SRCS=$(SRC_DIR)/IndexOutOfBound.cpp	\
	$(SRC_DIR)/Timing.cpp	\
	$(SRC_DIR)/Utils.cpp	\
	InputDataSet.cpp	\
	FileTokenizer.cpp	\
	OfflineNaiveBayesClassifier.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/IndexOutOfBound.o	\
	$(SRC_DIR)/Timing.o	\
	$(SRC_DIR)/Utils.o	\
	InputDataSet.o	\
	FileTokenizer.o	\
	OfflineNaiveBayesClassifier.o	\
	test/$(TESTER).o

.PHONY: nbc
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TESTER)
	rm -rf *~
