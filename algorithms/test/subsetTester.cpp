#include "utils.h"
#include "algorithms.h"
#include<set>

using std::set;
using namespace test;

#define printContainer(elements)   \
do {   \
    std::cout << "- " << #elements << " Elements: ";   \
    for (auto e : elements) std::cout << e << " ";  \
    std::cout << std::endl;   \
} while(0)

int main()
{
    DataSource datasource;
    vector<int> nums(4);
    datasource.fillInData(nums, 100);
    printContainer(nums);
    vector<int> sums = allSubsetSum(nums);
    printContainer(sums);
    set<int> sets;
    for (auto a : sums) sets.insert(a);
    printContainer(sets);
    vector<int> exludeDataset = datasource.dataSetExlude(sets, 100);
    printContainer(exludeDataset);
    int fail = 0, success = 0; 
    for (auto a : exludeDataset)
    {
        if (subsetExist(nums, a))
        {
            ++fail;
            std::cout << "- [Test Failed]";
            std::cout << "  Exlude dataset Test." << std::endl;
            std::cout << "- -------------" << std::endl;
            printContainer(nums);
            std::cout << "- Target sum: " << a << std::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            ++success;
        }
    }
    for (auto a : sums)
    {
        if (!subsetExist(nums, a))
        {
            ++fail;
            std::cout << "- [Test Failed]";
            std::cout << "  Include dataset Test." << std::endl;
            std::cout << "- -------------" << std::endl;
            printContainer(nums);
            std::cout << "- Target sum: " << a << std::endl;
            std::cout << "- -------------" << std::endl;
        }
        else
        {
            ++success;
        }
    }
    std::cout << "- Test result  : "
              << success << " cases success,"
              << fail << " cases fail."
              << std::endl;
}
