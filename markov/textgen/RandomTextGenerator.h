/*
 * RandomTextGenerator.h
 *
 * Header File
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
#ifndef __RANDOM_TEXT_GENERATOR_H__
#define __RANDOM_TEXT_GENERATOR_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <cstdlib>
#include <ctime>
#include "Logger.h"

//using namespace std;

// default source text, if no file is specified
#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipisicing elit, \
sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad  \
minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea  \
commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit   \
esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat \
non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

class RandomTextGenerator
{
private:
    int mBytes;
    std::istream * mpInStream; /* in this case, pointer is more useful than reference. */
    std::map<char,std::map<char,int> > mStateTable;
    Logger & mlog;
public:
    RandomTextGenerator( Logger & log = logger);
    RandomTextGenerator(std::istream &in, Logger & log = logger) :mlog(log) {};
    RandomTextGenerator(const char *file,  Logger & log = logger);
    ~RandomTextGenerator();
    
    /*************************************************
     *  generate random text with default 20 bytes.
     *************************************************/
    void randomText(int bytes = 20);
   
private:
    void buildStateTable(void);
    char next(std::map<char,int> &table);
};
#endif
