/*
 * RandomTextGenerator.cpp
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
#include "RandomTextGenerator.h"

//using namespace std;

RandomTextGenerator::RandomTextGenerator( Logger & log) 
                    : mBytes(0)
                    , mlog(log)
{
    mlog << "Construct RandomTextGenerator." << std::endl;
    mpInStream = new std::istringstream(LOREM_IPSUM); // try-catch should be used.
    buildStateTable();
    mlog << "Construct RandomTextGenerator Finish." << std::endl;
}

RandomTextGenerator::RandomTextGenerator(const char *file, Logger & log )
                    : mBytes(0)
                    , mlog(log)
{
    mlog << "Construct RandomTextGenerator." << std::endl;
    mpInStream = new std::ifstream(file); // try-catch should be used.
    buildStateTable();
    mlog << "Construct RandomTextGenerator Finish." << std::endl;
}

RandomTextGenerator::~RandomTextGenerator() 
{
    if (mpInStream) delete mpInStream;
}

void RandomTextGenerator::buildStateTable()
{
  char prev = 0;
  while ( !mpInStream->eof() ) 
  {
    char c;
    mpInStream->get( c );
    mlog << "Read char: " << c << std::endl;
    mStateTable[prev][c]++;
    prev = c;
  }
  mStateTable[prev][0]++;
}
char RandomTextGenerator::next(std::map<char,int> &table)
{
    int sum = 0;
    for (auto i = table.begin(); i != table.end(); i++ ) 
    {
        sum += i->second;
    }
    if (0 == sum ) { return 0; }
    int r = rand() % sum;
    for ( auto i = table.begin(); i != table.end(); i++ ) 
    {
        r -= i->second;
        if (r <= 0) { return i->first; }
    }

    return 0;
}

void RandomTextGenerator::randomText(int bytes)
{
    int prv = 0;
    srand( time( NULL ) );
    while (bytes != 0) 
    {
        char c = next( mStateTable[prv] );
        if ( !c && (bytes == -1) ) 
        {
          break;
        }
        std::cout << c;
        prv = c;
        if (bytes > 0) { --bytes; }
    }
    std::cout << std::endl;
}

