#ifndef CORE_LOGGING_HXX
#define CORE_LOGGING_HXX

#include "core_def.h"

namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API  Logging {

public:
    Logging ( );
    Logging ( const char *file_ptr, int );
    virtual ~Logging( );
    void  fatal( const char *msg, ... );
    void  info ( const char *msg, ... );
    void  flushInfo( );
private:
    void       *m_rsvd;
    const char *m_file_ptr;
    const char *m_func_ptr;
    int         m_ln_num;
};


}

#define core_fatal    core::Logging( __FILE__, __LINE__ ).fatal
#define core_info     core:Logging().info
#define core_debug       core::Logging( __FILE__, __LINE__ ).info
#define core_flush_info  core::Logging().flushInfo

#endif
