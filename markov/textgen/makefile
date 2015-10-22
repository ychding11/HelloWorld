SRC_DIR=$(HOME)/workspace/HelloWorld/markov/textgen

CXX=g++
CXXFLAGS=-c -O2 -W -std=c++11 -I. -DCHECK_IO

TESTER=RandomTextGeneratorTester

SRCS=RandomTextGenerator.cpp  Logger.cpp  test/$(TESTER).cpp

OBJS=RandomTextGenerator.o   Logger.o test/$(TESTER).o

.PHONY: textgen
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TESTER)
	rm -rf *~
