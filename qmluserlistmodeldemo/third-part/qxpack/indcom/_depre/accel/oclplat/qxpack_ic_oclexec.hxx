#ifndef QXPACK_IC_OCLEXEC_HXX
#define QXPACK_IC_OCLEXEC_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>

namespace QxPack {

class IcOclKernel;
class IcOclSglDev;

// ///////////////////////////////////////////////
//
// OpenCL Executor
//
// ///////////////////////////////////////////////
class QXPACK_IC_API IcOclExec {

public:
    IcOclExec ( );
    IcOclExec ( const IcOclSglDev & );
    IcOclExec ( const IcOclExec & );
    IcOclExec & operator = ( const IcOclExec & );
    virtual ~IcOclExec( );

    bool      isReady( ) const;
    intptr_t  cmdQueueHandle( ) const;

    void  begin( );
    bool  push ( const IcOclKernel & );
    void  end  ( );
    bool  exec ( );

private:
    void *m_obj;
};

}
#endif
