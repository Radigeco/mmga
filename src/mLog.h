/*                                
 _|_|_|  _|_|    _|_|_|  _|_|    
 _|    _|    _|  _|    _|    _|  
 _|    _|    _|  _|    _|    _|  
 _|    _|    _|  _|    _|    _|  
                                 
 Matrix Multiplication Project
 Copyright (C) 2011-2012 Andras Joo
 */

#pragma once

#include <ctime>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <mutex>
#include <iostream>
#include <iomanip>
#include "mColor.h"

namespace mmga
{

//------------------------------------------------------------------------------
// Four different log levels

enum
{
    logoff, logerr, logwarn, loginfo, logunset
};

//------------------------------------------------------------------------------
// Setting the log level. This has effect to the log commands which follow.
// TODO We need a more elegant/robust implementation for this facility

unsigned loglevel(unsigned level = logunset);


//------------------------------------------------------------------------------
// class: logger
// Thread-safe logger class. It is not meant to be used directly, rather through
// the log/warn/err macros.
//------------------------------------------------------------------------------

class logger : public std::stringstream
{
    const char * mFile, *mFunction;
    unsigned mLine;
    const char *mColor;
    FILE * mStream;
    unsigned mLevel;

    //file name is truncated/padded to 18 chars
    //function name is truncated/padded to 16 chars
    //line number is reserved 3 positions
    enum {mFileWidth = 18, mFunctionWidth = 16, mLineWidth = 3};

    //--------------------------------------------------------------------------

    //once created, no copies allowed
    //http://www2.research.att.com/~bs/C++0xFAQ.html#default
    logger(const logger &) = delete;
    logger& operator =(const logger &) = delete;

    //--------------------------------------------------------------------------

    void buff_preamb()
    {
        time_t rawtime;
        time(&rawtime);
        tm * timeinfo;
        timeinfo = localtime(&rawtime);

        *this << mColor << "[" << timeinfo->tm_mon+1 << "/" << timeinfo->tm_mday 
        << " " << std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":" 
        << std::setw(2) << timeinfo->tm_min << ":" << std::setw(2) << 
        timeinfo->tm_sec << " " << compact(basename(mFile), mFileWidth) << " "
        << compact(mFunction, mFunctionWidth) << " " << std::setw(mLineWidth) << 
        std::setfill(' ') << mLine << "] " << color::reset;
    }
	
    std::string compact(std::string s, size_t len)
    {
        if (s.length() < len)
            return s + std::string(len - s.length(), '-');

        return s.substr(0, len-4) + '~' + s.substr(s.length() - 3, 3);	
    }


//------------------------------------------------------------------------------
// The POSIX standard requires that by default the stream operations are atomic. 
// I.e., issuing two stream operations for the same stream in two threads at the 
// same time will cause the operations to be executed as if they were issued 
// sequentially. The buffer operations performed while reading or writing are 
// protected from other uses of the same stream. To do this each stream has an 
// internal lock object which has to be (implicitly) acquired before any work 
// can be done.

public:

    logger(const char * file, const char * function, unsigned line,
    const char * color, FILE * stream, unsigned level) : mFile(file),
    mFunction(function), mLine(line), mColor(color), mStream(stream),
    mLevel(level)
    {
        if (loglevel() < mLevel) return;
        buff_preamb();
    }

    ~logger()
    {
        if (loglevel() < mLevel) return;
        fprintf(mStream, "%s\n", this->str().c_str());
        fflush(mStream);       
    }
};

//------------------------------------------------------------------------------
// Macros producing stream-like objects. 
// TODO Optimize this code; at the moment 25s are needed to print 65536 messages

#define mm_log  logger(__FILE__, __func__, __LINE__, mmga::color::green, stdout, mmga::loginfo)
#define mm_warn logger(__FILE__, __func__, __LINE__, mmga::color::yellow, stdout, mmga::logwarn)
#define mm_err  logger(__FILE__, __func__, __LINE__, mmga::color::red, stderr, mmga::logerr)


//==============================================================================
//end of mm namespace
}




