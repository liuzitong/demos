#ifndef QMLUSERLISTMODELDEMO_LOGGING_HXX
#define QMLUSERLISTMODELDEMO_LOGGING_HXX

#include "qmluserlistmodeldemo_def.h"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API  Logging {

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

#define qmluserlistmodeldemo_fatal    qmlUserListmodelDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmluserlistmodeldemo_info     qmlUserListmodelDemo:Logging().info
#define qmluserlistmodeldemo_debug       qmlUserListmodelDemo::Logging( __FILE__, __LINE__ ).info
#define qmluserlistmodeldemo_flush_info  qmlUserListmodelDemo::Logging().flushInfo

#endif
