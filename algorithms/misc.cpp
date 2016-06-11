#include <cstdlib>
#include <cassert>
#include <iostream>

int sqrtOfPerfectSquare(int n)
{
    assert(n >= 0); // perfect squre >= 0
    int low = 0, high = n;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (mid * mid > n) high = mid - 1;
        else if (mid * mid < n) low = mid + 1;
        else return mid;
    }
    return -1; // n is not a perfect squre.
}

#if !defined(BUILD_LIB)
static void test()
{
    for (int i = 0; i < 20; ++i)
    {
        if (i != sqrtOfPerfectSquare(i * i))
        {
            std::cout << "- Test Failed. input:" << i * i << std ::endl;
        }
        else
        {
            std::cout << "- Test Success. input:" << i * i << std ::endl;
        }
    }
}

int main()
{
    test();
    return 0;
}
#endif
