#ifndef QMLPAGELAYOUTDEMO_LOGGING_HXX
#define QMLPAGELAYOUTDEMO_LOGGING_HXX

#include "qmlpagelayoutdemo_def.h"

namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API  Logging {

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

#define qmlpagelayoutdemo_fatal    qmlPagelayoutDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlpagelayoutdemo_info     qmlPagelayoutDemo:Logging().info
#define qmlpagelayoutdemo_debug       qmlPagelayoutDemo::Logging( __FILE__, __LINE__ ).info
#define qmlpagelayoutdemo_flush_info  qmlPagelayoutDemo::Logging().flushInfo

#endif
