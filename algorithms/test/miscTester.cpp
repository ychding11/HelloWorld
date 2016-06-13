#include "utils.h"
#include "algorithms.h"
#include <string>

using std::set;
using std::string;
using namespace test;

void testPermutation()
{
    string str = "abc";
    set<string> ret;
    permutation(str, 0, 2, ret);
    printContainer(ret);
}

void testSqrtOfPerfectSquare(int n = 100)
{
    int fail = 0, success = 0; 
    printTestName(sqrtOfPerfectSquare);
    for (int i = 0; i < n; ++i)
    {
        int ret = sqrtOfPerfectSquare(i * i);
        if (i != ret)
        {
            ++fail;
            std::cout << "- Test Failed."
                      << " input:" << i * i
                      << " output:" << ret << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            ++success;
            std::cout << "- Test Success."
                      << " input:" << i * i
                      << " output:" << ret << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
    }
    for (int i = n * n + 1; i < (n + 1) * (n + 1); ++i)
    {
        int ret = sqrtOfPerfectSquare(i);
        if (-1 != ret)
        {
            ++fail;
            std::cout << "- Test Failed."
                      << " input:" << i
                      << " output:" << ret << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            ++success;
            std::cout << "- Test Success."
                      << " input:" << i
                      << " output:" << ret << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
    }
    printTestResult(success, fail);
}

int main()
{
    //testSqrtOfPerfectSquare();
    testPermutation();
}
