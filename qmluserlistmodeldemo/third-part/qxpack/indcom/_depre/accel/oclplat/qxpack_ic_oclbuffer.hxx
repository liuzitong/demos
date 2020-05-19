#ifndef QXPACK_IC_OCLBUFFER_HXX
#define QXPACK_IC_OCLBUFFER_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>
#include <stdint.h>

namespace QxPack {

// ///////////////////////////////////////////////////////////
//
// OpenCL buffer 
//
// ///////////////////////////////////////////////////////////
class IcOclBuffCachePriv;

class QXPACK_IC_API IcOclBuffer {

public:
    IcOclBuffer(  );
    IcOclBuffer( const IcOclBuffer & );    
    IcOclBuffer &  operator = ( const IcOclBuffer & );
    virtual ~IcOclBuffer(  );

    bool      isNull ( ) const;
    bool      isReady( ) const;
    bool      isImage( ) const;

    int       size   ( ) const;  // NOTE: the size() is the required data size
    intptr_t  handle ( ) const;
    bool      write  ( const IcByteArray & );
    bool      read   (       IcByteArray & );

    int       capacity( ) const; // nw: 20180802 added. means the actual size

protected:
    IcOclBuffer( intptr_t ctxt, intptr_t cmd_queue, int cap_size );
    void      resize ( int sz );
friend class IcOclBuffCachePriv;
private:
    void *m_obj;
};

}

#endif
