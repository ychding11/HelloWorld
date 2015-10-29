/*
 * Logger.h
 *
 * Header Header
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
#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// Adapted from: http://stackove__mCostFunctionlow.com/questions/2212776/overload-handling-of-stdendl

class Logger: public std::ostream
{
    class InternalStreamBuf: public std::stringbuf
    {
        private:
            std::ostream&  __mScreenStream;
            std::ostream&  __mFileStream;
        public:
            InternalStreamBuf(std::ostream& screenStream, std::ostream& fileStream) 
                        : __mScreenStream(screenStream), __mFileStream(fileStream)
            { }

            /*  implement the virtual function to sync string buffer content */
            virtual int sync ( )
            {
			    __mFileStream << str();
			    __mScreenStream << str();
                str(""); /* clear string buffer content */
			    __mFileStream.flush();
			    __mScreenStream.flush();
                return 0;
            }
    };

    private:
        std::ofstream       __mLogFile;
        InternalStreamBuf   __mBuffer;
    public:
		Logger(const char* filename) 
		: std::ostream(&__mBuffer), __mLogFile(filename), __mBuffer(std::cout, __mLogFile)
        { /* do nothing */ }

		virtual ~Logger() { __mLogFile.close(); }
};
#endif /* MYLOG_H_ */
