// this file will be compiled as a .o file
//  g++ -m64 -Wall -Wextra -march=native -O3 -c b.cpp
//
#include "interface.h"

void mytest::funB(long *a)
{
	++(*a);
}

struct Z : public mytest::X
{
   virtual void fun(long *a)
   {
	   ++(*a);
   }
};

std::auto_ptr<mytest::X> mytest::createB()
{
	return std::auto_ptr<mytest::X>(new Z());
}
