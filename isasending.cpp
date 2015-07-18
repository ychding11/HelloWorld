#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <set>
using namespace std;

/*
 * read data from stdin and check whether the input sequence
 * is asending or not.
 * reference: 
 * http://www.cplusplus.com/reference/cstdio/scanf/?kw=scanf
 */
int check_asending(void)
{
	int preNum,curNum;
	preNum = 0;
	while (scanf("%d", &curNum) != EOF)
	{
		if (curNum < preNum)
		{
			printf("%d %d \t NOT asending!\n", preNum, curNum);
			return 0; //disorder here
		}
		preNum = curNum;
		
	}
	return 1;
}

int main(int argc, char** argv)
{  
  int ret = check_asending();
  printf("asending: %d\n", ret);
  return 0;
}

