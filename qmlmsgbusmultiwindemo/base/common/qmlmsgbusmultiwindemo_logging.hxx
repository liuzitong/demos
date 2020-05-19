#ifndef QMLMSGBUSMULTIWINDEMO_LOGGING_HXX
#define QMLMSGBUSMULTIWINDEMO_LOGGING_HXX

#include "qmlmsgbusmultiwindemo_def.h"

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API  Logging {

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

#define qmlmsgbusmultiwindemo_fatal    qmlMsgbusMultiwinDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlmsgbusmultiwindemo_info     qmlMsgbusMultiwinDemo:Logging().info
#define qmlmsgbusmultiwindemo_debug       qmlMsgbusMultiwinDemo::Logging( __FILE__, __LINE__ ).info
#define qmlmsgbusmultiwindemo_flush_info  qmlMsgbusMultiwinDemo::Logging().flushInfo

#endif
