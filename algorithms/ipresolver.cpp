/*
 * ipresolver.cpp
 * demostrate recursion algorithms to solve some classic problems, such as
 * ip address resolve.
 *
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
#include <map>
#include <vector>
#include <iostream>
#include "Logger.h"

using namespace std;

#define PERFORMANCE_METER

/*************************************************
 *  The class is to resolve the IP V4 Address.
 *
 *  NOTE: max N supported is 32.
*************************************************/
class IpAddressResolver 
{
public:
    vector<string> resolveIpAddresses(string s) 
    {
        vector<string> ret;
        string ip; 
        restoreIpAddressesDFS(ret,ip, s, 0, 0);
        return ret;
    }
    
private:  
    /*************************************************
     *  The helper to resolve the IP V4 Address.
     *
     *  result: store all resolved results.
    *************************************************/
    void restoreIpAddressesDFS(vector<string> &result, string ip,  string &s, int startIndex, int depth) 
    {
        /* search end condition */
        if (startIndex == s.size() && depth == 4) //ip address has 4 segments.
        {  
            ip.resize(ip.size() - 1); //remove tail '.'
            result.push_back(ip);
            return;
        }
        
        if (depth > 4) return; //invalid case, skip
        
        int num = 0;
        for (int i = startIndex; i < startIndex + 3; i++) 
        {
            num = num * 10 + (s[i] - '0');

            if (num <= 255) 
            {  
                ip += s[i];
                restoreIpAddressesDFS(result, ip + '.',  s, i + 1, depth + 1);
            }
            
            /* 
             * s[startIndex] == '0' is invalid 01.xxx   03.xxx, 00.xxx
             * but 0.0.0.0 is a valid ip address.
             * a tricky here to make 0.0.0.0 recoginsed as an invalid address.
             */
            if (num == 0 || num > 255)  break; 
        }
    }
};

void printResult(vector<string> &ret)
{
    int n = ret.size();
    for (int i = 0; i < n; i++)
    {
        cout << ret[i] << std::endl;
    }
}

/*************************************************
 * just for a simple testing.
 *
*************************************************/
int main(int argc, char** argv)
{
  logger.setLevel(DEBUG);
  logger.setLineLevel(DEBUG);
  ENTER_FUNCTION;
  
  string ip;
  IpAddressResolver solver;
  while (true)
  {
    cout << "Enter ip string(enter 'q' to quit):";
    cin >> ip;
    if ("q" == ip)
    {
        cout << "Game Over." << std::endl;
        return 0;
    }
    cout << "Ip Address: " << ip << std::endl ;
    vector<string> ret = solver.resolveIpAddresses(ip);
    cout << "Result:" << std::endl;
    printResult(ret);
  }
  
  EXIT_FUNCTION;
  return 0;
}

