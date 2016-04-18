//This is a tester 
//How to compile it to be a tester
// g++ -m64 -Wall -Wextra -march=native -O3 -c test.cpp 
// g++ -m64 -Wl,-rpath,'$ORIGIN' -Wl,-z,now -o test test.o b.o liba.so -lrt

#include "interface.h"
#include <cstdio>
#include <ctime>

typedef unsigned long long nsec_t;

nsec_t now()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * nsec_t(1000000000) + ts.tv_nsec;
}

int main()
{
	std::auto_ptr<mytest::X> pa = mytest::createA();
	std::auto_ptr<mytest::X> pb = mytest::createB();
	long num = 0;
	nsec_t start, stop;
	nsec_t times[4] = {0};
	const int M = 10000 * 10;
	const int N = 10000 * 100;
	for (int i = 0; i < M; ++i)
	{
		mytest::funA(&num);
		mytest::funB(&num);
		pa->fun(&num);
		pb->fun(&num);
	}
	num = 0;
	for (int i = 0; i < N; ++i)
	{
		start = now();
		mytest::funA(&num); // normal function in shared lib
		stop = now(), times[0] += stop - start;

		start = now();
		mytest::funB(&num); // normal function in .o
		stop = now(), times[1] += stop - start;

		start = now();
		pa->fun(&num); // virutal function in shared lib
		stop = now(), times[2] += stop - start;

		start = now();
		pb->fun(&num); // virtual function in .o
		stop = now(), times[3] += stop - start;
	}

	times[0] /= N;
	times[1] /= N;
	times[2] /= N;
	times[3] /= N;
    #if 0
	printf("num = %ld\t"
		   "normal shared= %lld\t" 
		   "normal .o    = %lld\t" 
		   "virtual shared= %lld\t" 
		   "virtual .o  = %lld\n", 
		   num, times[0], times[1], times[2], times[3]);
    #else
	printf("%lld\t" 
		   "%lld\t" 
		   "%lld\t" 
		   "%lld\n", 
		   times[0], times[1], times[2], times[3]);

    #endif
}
