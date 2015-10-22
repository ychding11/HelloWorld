/*
 * Logger.hpp
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
 
#ifndef __LOGGER__
#define __LOGGER__


#include <fstream>
#include <cassert>
#include <ctime>
#include <sstream>


// Log levels
typedef enum 
{
  VERBOSE = 0,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  CRITICAL
} logger_level;

class Logger : public std::ostringstream 
{
private:
    std::ofstream  _file;
    std::ostream&  _log; 
    logger_level   _level;
    logger_level   _line_level;
public:
    Logger(const char *f) 
    : _file(f, std::ios::out | std::ios::app)
	, _log(_file), _level(INFO), _line_level(VERBOSE)
    {
      assert(_file.is_open());
    }

    Logger(const std::string& f) 
    : _file(f.c_str(), std::ios::out | std::ios::app)
	, _log(_file), _level(INFO), _line_level(VERBOSE)
    {
      assert(_file.is_open());
    }

    ~Logger()
    {
      if (_file.is_open())
      {
        _log.flush();
        _file.close();
      }
    }
  
  Logger (const Logger &) = delete;
  Logger &operator= (const Logger &) = delete;

  void set_level(const logger_level& level);  
  void flush()
  {
      if (_line_level >= _level) 
      {
        _log << get_time() << " -- [" << level_str(_line_level) << "] -- " << str();
        _log.flush();
      }

      str("");
      _line_level = VERBOSE;
  }
  
  template <typename T>
  Logger& operator<<(const T& t)
  {
    std::cout << "call operator << " << std::endl;
    *static_cast<std::ostringstream *>(this) << t;
    return (*this);
  }
  
  Logger& operator<<(const logger_level& level)
  {
      _line_level = level;
      return (*this);
  }
  
  typedef Logger& (* LoggerManip)(Logger&);
  Logger& operator<<(LoggerManip m)
  { return m(*this);    }
  
private:
  std::string get_time() const
  {
      struct tm *timeinfo;
      time_t rawtime;
      char *time_buf;
      
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      time_buf = asctime(timeinfo);
      
      std::string ret(time_buf);
      if (!ret.empty() && ret[ret.length() - 1] == '\n') 
      {
        ret.erase(ret.length()-1);
      }
      
      return (ret);
  }
  
  inline const char* level_str(const logger_level& level)
  {
      switch (level) 
      {
      case VERBOSE:
        return ("VRB");
      case DEBUG:
        return ("DBG");
      case INFO:
        return ("INF");
      case WARNING:
        return ("WRN");
      case ERROR:
        return ("ERR");
      case CRITICAL:
        return ("CRT");
      default:
        assert(false);
      }
  } 
};

namespace std 
{ 
  inline Logger& endl(Logger& out) 
  { 
    out.put('\n'); 
    out.flush(); 
    return (out); 
  } 
}

extern Logger gLogger;

#endif
