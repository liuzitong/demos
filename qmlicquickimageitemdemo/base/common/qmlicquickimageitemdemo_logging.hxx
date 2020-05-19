#ifndef QMLICQUICKIMAGEITEMDEMO_LOGGING_HXX
#define QMLICQUICKIMAGEITEMDEMO_LOGGING_HXX

#include "qmlicquickimageitemdemo_def.h"

namespace qmlIcquickimageitemDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLICQUICKIMAGEITEMDEMO_API  Logging {

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

#define qmlicquickimageitemdemo_fatal    qmlIcquickimageitemDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlicquickimageitemdemo_info     qmlIcquickimageitemDemo:Logging().info
#define qmlicquickimageitemdemo_debug       qmlIcquickimageitemDemo::Logging( __FILE__, __LINE__ ).info
#define qmlicquickimageitemdemo_flush_info  qmlIcquickimageitemDemo::Logging().flushInfo

#endif
