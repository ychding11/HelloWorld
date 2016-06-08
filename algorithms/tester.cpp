#include "Logger.h"
#include "utils.h"
#include<set>

using std::set;
using namespace test;

#define printContainer(elements)   \
{   \ 
    std::cout << "- " << #elements << " Elements:" << std::endl;   \
    for (auto e : elements) std::cout << e << " ";  \
    std::cout << std::endl;   \
    std::cout << "- End Elements" << std::endl;   \
}

int main()
{
	Logger log("./log.txt");
	log << "Test log!" << std::endl;

    vector<int> nums = {1, 2, 3};
    vector<int> sums = allSubsetSum(nums);
    printContainer(sums);
    set<int> sets;
    for (auto a : sums) sets.insert(a);
    printContainer(sets);
}
