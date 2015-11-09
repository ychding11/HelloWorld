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
 *  The class is to solver N Queens problem.
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
    void restoreIpAddressesDFS(vector<string> &result, string ip,  string &s, int startIndex, int depth) 
    {
        /* search end condition */
        if (startIndex == s.size() && depth == 4) 
        {  
            ip.resize(ip.size() - 1);
            result.push_back(ip);
            return;
        }
        
        if (depth > 4) return;
        
        int num = 0;
        for (int i = startIndex; i < startIndex + 3; i++) 
        {
            num = num * 10 + (s[i] - '0');

            if (num <= 255) 
            {  
                ip += s[i];
                restoreIpAddressesDFS(result, ip + '.',  s, i + 1, depth + 1);
            }
            if (num == 0) break; /* s[startIndex] == '0' is invalid 01.xxx   03.xxx, but 0.0.0.0 is valid ip */
        }
    }
};
/*************************************************
 * just call the demo string.
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
    cout << "Enter Queens number:";
    cin >> ip;
    if ("" == ip)
    {
        cout << "Game Over." << std::endl;
        return 0;
    }
    solver.resolveIpAddresses(ip);
  }
  
  EXIT_FUNCTION;
  return 0;
}

