#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>
#include "bits.h"

#if !defined(BUILD_LIB)
#include<bitset>
using std::bitset;
#endif

using std::vector;

/*! \brief an array of number, all number occur twice except one, find that number. */
int unpairNumber(const vector<int> &nums)
{
    int ret = 0;
    for (auto a : nums) ret ^= a;
    return ret;
}

/*! \brief an array of number, all number occur twice except two, find them. */
void unpairNumbers(const vector<int> &nums, int &x, int &y)
{
    int ret = 0;
    for (auto a : nums) ret ^= a;
    ret = ret & (1 << firstSettedBit(ret)); 
    x = y = 0;
    for (auto a : nums)
    {
        if (ret & a) x ^= a;
        else y ^= a;
    }
}

#if !defined(BUILD_LIB)
int main()
{
    return 0;
}
#endif
