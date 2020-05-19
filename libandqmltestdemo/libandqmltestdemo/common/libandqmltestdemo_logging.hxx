#ifndef LIBANDQMLTESTDEMO_LOGGING_HXX
#define LIBANDQMLTESTDEMO_LOGGING_HXX

#include "libandqmltestdemo_def.h"

namespace libandqmltestdemo {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class LIBANDQMLTESTDEMO_API  Logging {

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

#define libandqmltestdemo_fatal    libandqmltestdemo::Logging( __FILE__, __LINE__ ).fatal
#define libandqmltestdemo_info     libandqmltestdemo:Logging().info
#define libandqmltestdemo_debug       libandqmltestdemo::Logging( __FILE__, __LINE__ ).info
#define libandqmltestdemo_flush_info  libandqmltestdemo::Logging().flushInfo

#endif
