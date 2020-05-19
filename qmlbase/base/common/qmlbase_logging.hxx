#ifndef QMLBASE_LOGGING_HXX
#define QMLBASE_LOGGING_HXX

#include "qmlbase_def.h"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API  Logging {

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

#define qmlbase_fatal    qmlbase::Logging( __FILE__, __LINE__ ).fatal
#define qmlbase_info     qmlbase:Logging().info
#define qmlbase_debug       qmlbase::Logging( __FILE__, __LINE__ ).info
#define qmlbase_flush_info  qmlbase::Logging().flushInfo

#endif
