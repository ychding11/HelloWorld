// This will be compiled as a shared library.
// To research function call overhead for
// virtual function and normal function.
//
// How to compile:
//   g++ -m64 -Wall -Wextra -march=native -O3 -fpic -c a.cpp
//
// How to link it to shared library
//   g++ -m64 -Wl,-rpath,'$ORIGIN' -Wl,-z,now -shared -o liba.so a.o

#include "interface.h"

void mytest::funA(long *a)
{
	++(*a);
}

struct Y : public mytest::X
{
   virtual void fun(long *a)
   {
	   ++(*a);
   }
};

std::auto_ptr<mytest::X> mytest::createA()
{
   return std::auto_ptr<mytest::X> (new Y());
}
