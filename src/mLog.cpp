/*                                
 _|_|_|  _|_|    _|_|_|  _|_|    
 _|    _|    _|  _|    _|    _|  
 _|    _|    _|  _|    _|    _|  
 _|    _|    _|  _|    _|    _|  
                                 
 Matrix Multiplication Project
 Copyright (C) 2011-2012 Andras Joo
 */

#include "mLog.h"

namespace mmga
{

namespace 
{
    static std::mutex log_mutex;
}

//------------------------------------------------------------------------------
// Setting the log level. This has effect to the log commands which follow.
// TODO We need a more elegant/robust implementation for this facility

unsigned loglevel(unsigned level)
{
    const std::lock_guard<std::mutex> guard(log_mutex);
    static unsigned LOGLEVEL = loginfo;
    if (level != logunset) LOGLEVEL = level;
    return LOGLEVEL;
}

//end of mm namespace
}




