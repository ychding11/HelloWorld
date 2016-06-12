#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string>
#include <set>

#if !defined(BUILD_LIB)
#include<bitset>
using std::bitset;
#endif

using std::set;
using std::string;

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

unsigned int bitCount(unsigned int n)
{
    int c = 0;
    while (n)
    {
        ++c; n = n & (n - 1);
    }
    return c;
}

static void swap(string & str, int i, int j)
{
    // inner function no need to check input parameter.
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
}

void permutation(string str, int i, int j, set<string> &ret)
{
    assert(!str.empty() && i <= j);
    if (i == j)
    {
        ret.insert(str);
        return;
    }
    for (int k = i; k <= j; ++k)
    {
        string temp = str;
        if (k != i) swap(temp, i, k);
        permutation(temp, i + 1, j, ret);
    }
}

#if !defined(BUILD_LIB)
static void testbitCount()
{
    bitset<10> binNum;
    unsigned long n = binNum.to_ulong();
    for (unsigned int i = 0; i < 10; ++i)      
    {
        binNum[i] = 1;
        n = binNum.to_ulong();
        unsigned long ret = bitCount(n);
        if (i + 1 != ret)
        {
            std::cout << "- Test Failed. Input:" << n << " Output:" << ret  << std ::endl;
        }
        else
        {
            std::cout << "- Test Success. Input:" << n << " Output:" << ret  << std ::endl;
        }
    }
}
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
//    test();
    testbitCount();
    return 0;
}
#endif
