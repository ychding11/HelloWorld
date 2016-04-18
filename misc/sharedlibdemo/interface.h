#pragma once

#include<memory>

namespace mytest
{

void funA(long*); 
void funB(long*); 

struct X
{
    virtual ~X() {}
    virtual void fun(long*) = 0;
};
std::auto_ptr<X> createA(); 
std::auto_ptr<X> createB(); 

}
