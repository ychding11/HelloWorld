/*
 * number.cpp
 * Source File
 *
 * Copyright (C) 2014-2015  Yaochuang Ding - <ych_ding@163.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution, and in the same 
 *    place and form as other copyright, license and disclaimer information.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include "Logger.h"

class NumberToWords 
{
private:
    static std::string mTensNames[10];
    static std::string mNumNames[20];
public:
    std::string convertLessThanOneThousand(int number) 
    {
        std::string str;

        if (number % 100 < 20)
        {
          str = mNumNames[number % 100];
          number /= 100;
        }
        else 
        {
          str = mNumNames[number % 10];
          number /= 10;

          str = mTensNames[number % 10] + str;
          number /= 10;
        }
        if (number == 0) 
        {
            return str;
        }
        return mNumNames[number] + " hundred" + str;
    }
};

std::string NumberToWords::mTensNames[10] = 
{
    "",
    " ten",
    " twenty",
    " thirty",
    " forty",
    " fifty",
    " sixty",
    " seventy",
    " eighty",
    " ninety"
};

std::string NumberToWords::mNumNames[20] = 
{
    "",
    " one",
    " two",
    " three",
    " four",
    " five",
    " six",
    " seven",
    " eight",
    " nine",
    " ten",
    " eleven",
    " twelve",
    " thirteen",
    " fourteen",
    " fifteen",
    " sixteen",
    " seventeen",
    " eighteen",
    " nineteen"
};


int main(int argc, char** argv)
{
    logger.setLevel(DEBUG);
    logger.setLineLevel(INFO);
    ENTER_FUNCTION;
    
  	int n;
  	if (argc != 2)
  	{
        printf("Usage Error!\n""%s [number less than 1000]\n", argv[0]); 
   	 	return -1;
 	}
	n = atoi(argv[1]);  /* input count*/
	
	logger << "n = " << n << std::endl;
    if (n < 0 || n >= 1000) //fix bugs here.
    {
        printf("ERROR, input error. line : %d\n", __LINE__);
        return -2;
    }
	NumberToWords ntw;
	std::string ret = ntw.convertLessThanOneThousand(n);
	std::cout << ret << std::endl;
	
    EXIT_FUNCTION;
    return 0;
}

