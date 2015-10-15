SRC_DIR=$(HOME)/workspace/ml/kmeans

CXX=g++
CXXFLAGS=-c -g -W -std=c++11 -I.  #-DOKM_DBG
#CXXFLAGS=-c -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. -O2

TESTER=OfflineKMeansTester
SRCS=$(SRC_DIR)/IndexOutOfBound.cpp	\
	$(SRC_DIR)/FloatDataReader.cpp	\
	$(SRC_DIR)/Samples.cpp	\
	OfflineKMeans.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/IndexOutOfBound.o	\
	$(SRC_DIR)/FloatDataReader.o	\
	$(SRC_DIR)/Samples.o	\
	OfflineKMeans.o	\
	test/$(TESTER).o

.PHONY: okm
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTER)
	rm -rf *~
