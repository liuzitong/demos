#ifndef QMLCFMBUSMULTICFMDEMO_LOGGING_HXX
#define QMLCFMBUSMULTICFMDEMO_LOGGING_HXX

#include "qmlcfmbusmulticfmdemo_def.h"

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API  Logging {

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

#define qmlcfmbusmulticfmdemo_fatal    qmlCfmBusMulticfmDemo::Logging( __FILE__, __LINE__ ).fatal
#define qmlcfmbusmulticfmdemo_info     qmlCfmBusMulticfmDemo:Logging().info
#define qmlcfmbusmulticfmdemo_debug       qmlCfmBusMulticfmDemo::Logging( __FILE__, __LINE__ ).info
#define qmlcfmbusmulticfmdemo_flush_info  qmlCfmBusMulticfmDemo::Logging().flushInfo

#endif
