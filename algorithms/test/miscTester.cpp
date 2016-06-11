#include "utils.h"
#include "algorithms.h"

using std::set;
using namespace test;

#define printContainer(elements)   \
do {   \
    std::cout << "- " << #elements << " Elements:" << std::endl;   \
    for (auto e : elements) std::cout << e << " ";  \
    std::cout << std::endl;   \
    std::cout << "- End Elements" << std::endl;   \
} while(0)

void testSqrtOfPerfectSquare(int n = 100)
{
    int fail = 0, success = 0; 
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
}

int main()
{
    testSqrtOfPerfectSquare();
}
