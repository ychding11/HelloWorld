#include <cstdlib>
#include <cassert>
#include <iostream>

#if !defined(BUILD_LIB)
#include<bitset>
using std::bitset;
#endif

int addOne(int n)
{
    return -(~n);
}

bool isPowerOf4(unsigned int n)
{
    if (n > 0 && !(n & (n - 1)))
    {
        for (unsigned int i = 0; i < sizeof(n) * 8; i += 2)
        {
            if ((n >> i) & 0x1) return true;
        }
    }
    return false;
}

/*! \brief return min by bit operation.
 * http://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax 
 * -1 = ~0; b ^ a ^ b = a; a ^ a = 0; a ^ 0 = a;
 */
bool minByBit(int a, int b)
{
    return b ^ ((a ^ b) & -(a < b));
}

#if !defined(BUILD_LIB)

int main()
{
    return 0;
}
#endif
