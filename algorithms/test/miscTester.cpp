#include "utils.h"
#include "algorithms.h"
#include <string>
#include <vector>

using std::set;
using std::vector;
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

void testUnpairNumber(int testcases = 10, int numElements = 10)
{
    DataSource datasource;
    vector<int> nums(numElements), temp(numElements);
    datasource.fillInDistinctData(nums, 100);
    printContainer(nums);
    temp = nums;
    for (auto a : temp) nums.push_back(a);
    printContainer(nums);
    for (int i = 0; i < testcases; ++i)
    {
        nums.push_back(100 + i);
        int temp = unpairNumber(nums);
        if (100 + i != temp)
        {
            // test failed
            std::cout << "- Test Failed."
                      << " Output:" << temp 
                      << " Expect:" << 100 + i << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            // test success
            std::cout << "- Test Success."
                      << " Output:" << temp << std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        nums.pop_back();
    }
    for (int i = 0; i < testcases; ++i)
    {
        nums.push_back(100 + i);
        nums.push_back(100 + i + 1);
        int x, y;
        unpairNumbers(nums, x, y);
        if (x != 100 + i && x != 100 + i + 1)
        {
            // test failed
            std::cout << "- Test Failed."
                      << " Output:" << x << " " << y 
                      << " Expect:" << 100 + i << " " << 100 + i + 1<< std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            // test success
            std::cout << "- Test Success."
                      << " Output:" << 100 + i << " " << 100 + i + 1<< std ::endl;
            std::cout << "- -------------" << std::endl;
        }
        nums.pop_back();
        nums.pop_back();
    }
}

int main()
{
    //testSqrtOfPerfectSquare();
   // testPermutation();
    testUnpairNumber();
}
