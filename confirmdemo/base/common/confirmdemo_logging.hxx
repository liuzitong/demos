#ifndef CONFIRMDEMO_LOGGING_HXX
#define CONFIRMDEMO_LOGGING_HXX

#include "confirmdemo_def.h"

namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API  Logging {

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

#define confirmdemo_fatal    confirmDemo::Logging( __FILE__, __LINE__ ).fatal
#define confirmdemo_info     confirmDemo:Logging().info
#define confirmdemo_debug       confirmDemo::Logging( __FILE__, __LINE__ ).info
#define confirmdemo_flush_info  confirmDemo::Logging().flushInfo

#endif
