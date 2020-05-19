#ifndef QMLCONFIRMBUSDEMO_LOGGING_HXX
#define QMLCONFIRMBUSDEMO_LOGGING_HXX

#include "qmlconfirmbusdemo_def.h"

namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCONFIRMBUSDEMO_API  Logging {

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

#define qmlconfirmbusdemo_fatal    qmlconfirmbusdemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlconfirmbusdemo_info     qmlconfirmbusdemo:Logging().info
#define qmlconfirmbusdemo_debug       qmlconfirmbusdemo::Logging( __FILE__, __LINE__ ).info
#define qmlconfirmbusdemo_flush_info  qmlconfirmbusdemo::Logging().flushInfo

#endif
