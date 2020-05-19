#ifndef $D{Key2_ProjNameU}_LOGGING_HXX
#define $D{Key2_ProjNameU}_LOGGING_HXX

#include "$D{Key2_ProjNameL}_def.h"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
// simple logging console
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API  Logging {

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

#define $D{Key2_ProjNameL}_fatal    $D{Key2_ProjNameN}::Logging( __FILE__, __LINE__ ).fatal
#define $D{Key2_ProjNameL}_info     $D{Key2_ProjNameN}:Logging().info
#define $D{Key2_ProjNameL}_debug       $D{Key2_ProjNameN}::Logging( __FILE__, __LINE__ ).info
#define $D{Key2_ProjNameL}_flush_info  $D{Key2_ProjNameN}::Logging().flushInfo

#endif
