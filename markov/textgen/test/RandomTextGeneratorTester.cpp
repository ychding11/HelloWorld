/*
 * RandomTextGeneratorTester.cpp
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
#include "Logger.h"
#include <unistd.h>

char *progName = NULL;

void showhelp() 
{
  std::cout << "Usage: " << progName << " [options] [file]\n"
       << "Generates gibberish in the style of file, or Lorem Ipsum if no file is given.\n"
       << "\n"
       << "Options:\n"
       << "-c n\tNumber of bytes to generate (optional)\n"
       << "-h\tDisplay this message\n\n";
}

/*************************************************
 * a simple tester for random text generator.
 *
 * it can build markov model from user-defined text
 * or a default text.
*************************************************/
int main(int argc, char* argv[]) 
{
    logger.setLevel(DEBUG);
    logger.setLineLevel(INFO);
    logger << "Tester..." << std::endl;
    
    if (argc == 1) //no user cmd parameter
    {
        RandomTextGenerator textgen;
        textgen.randomText(100);
    } 
    else if (argc > 1) 
    {
        if (argc == 3)
        {
            int bytes = atoi(argv[1]); //text bytes.
            const char *file = argv[2];
            logger << bytes << " " << file << std::endl;
            RandomTextGenerator textgen(file);
            textgen.randomText(bytes);
        }
    }    
    return 0;
}
