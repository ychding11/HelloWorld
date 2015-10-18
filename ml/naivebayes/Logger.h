/************************************************************************/
/* Logger													            */
/* 														                */
/************************************************************************/
#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// Adapted from: http://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

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
