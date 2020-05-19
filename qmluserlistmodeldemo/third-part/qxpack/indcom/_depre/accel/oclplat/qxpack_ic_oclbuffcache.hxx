#ifndef QXPACK_IC_OCLBUFFCACHE_HXX
#define QXPACK_IC_OCLBUFFCACHE_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>

namespace QxPack {

class IcOclBuffer;

// ////////////////////////////////////////////////////////
//
// OpenCL Buffer Cache
//
// ////////////////////////////////////////////////////////
class QXPACK_IC_API  IcOclBuffCache {

public:
    IcOclBuffCache( );
    IcOclBuffCache( intptr_t ctxt, intptr_t cmd_queue );
    IcOclBuffCache( const IcOclBuffCache & );
    IcOclBuffCache & operator = ( const IcOclBuffCache & );
    virtual ~IcOclBuffCache( );

    bool isReady( ) const;

    IcOclBuffer   create  ( int req_size );
    void          recycle ( IcOclBuffer & );

private:
    void *m_obj;
};


}

#endif
