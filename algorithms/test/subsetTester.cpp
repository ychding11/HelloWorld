#include "utils.h"
#include "algorithms.h"
#include<set>

using std::set;
using namespace test;

#define printContainer(elements)   \
do {   \ 
    std::cout << "- " << #elements << " Elements:" << std::endl;   \
    for (auto e : elements) std::cout << e << " ";  \
    std::cout << std::endl;   \
    std::cout << "- End Elements" << std::endl;   \
} while(0)

int main()
{
    vector<int> nums = {1, 2, 3};
    vector<int> sums = allSubsetSum(nums);
    printContainer(sums);
    set<int> sets;
    for (auto a : sums) sets.insert(a);
    printContainer(sets);
    int fail = 0, success = 0; 
    for (auto a : sets)
    {
        if (0)
        {
            ++success;
        }
        else
        {
            ++fail;
        }
    }
    std::cout << "Test result  : "
              << success << " cases success,"
              << fail << " cases fail."
              << std::endl;
}
