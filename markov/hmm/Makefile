CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=
TARGET=./test

all: train test

train: train.o hmm.o Logger.o
	mkdir -p ${TARGET}
	${CC} hmm.o train.o Logger.o -o ${TARGET}/train

train.o: train.cpp
	$(CC) $(CFLAGS) train.cpp

hmm.o:  hmm.h hmm.cpp
	$(CC) $(CFLAGS) hmm.cpp
	
Logger.o: Logger.h Logger.cpp
	$(CC) $(CFLAGS) Logger.cpp
    
clean:
	rm -rf  *.o
	rm -rf  ${TARGET}
	rm -rf  *~

	
