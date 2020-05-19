#ifndef QMLMSGBUSDEMO_LOGGING_HXX
#define QMLMSGBUSDEMO_LOGGING_HXX

#include "qmlmsgbusdemo_def.h"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_API  Logging {

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

#define qmlmsgbusdemo_fatal    qmlMsgbusDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlmsgbusdemo_info     qmlMsgbusDemo:Logging().info
#define qmlmsgbusdemo_debug       qmlMsgbusDemo::Logging( __FILE__, __LINE__ ).info
#define qmlmsgbusdemo_flush_info  qmlMsgbusDemo::Logging().flushInfo

#endif
