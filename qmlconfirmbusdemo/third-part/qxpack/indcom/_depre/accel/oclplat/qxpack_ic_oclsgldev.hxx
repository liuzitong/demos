#ifndef QXPACK_IC_OCLSGLDEV_HXX
#define QXPACK_IC_OCLSGLDEV_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <stdint.h>

namespace QxPack {

class IcOclProgCache;
class IcOclBuffCache;

// ////////////////////////////////////////////////////////////
//
// opencl device by specifed system
//
// ////////////////////////////////////////////////////////////
class QXPACK_IC_API  IcOclSglDev {

public:
    enum Type {
        Type_Auto = 0, Type_GPU = 1, Type_CPU = 2
    };

    IcOclSglDev( );

    // ------------------------------------------------------------
    // plat_idx == -1 auto found a usable platform.
    // dev_idx  == -1 auto found a usable device
    // type     == Type_Auto first test the GPU, then test the CPU
    // -------------------------------------------------------------
    IcOclSglDev( Type type, int plat_idx = -1, int dev_idx = -1 );
    IcOclSglDev( const IcOclSglDev & ); 
    IcOclSglDev &  operator = ( const IcOclSglDev & );
    virtual ~IcOclSglDev ( );

    bool      isReady   ( ) const;
    intptr_t  platformId( ) const;
    intptr_t  deviceId  ( ) const; 
    intptr_t  context   ( ) const;
    intptr_t  cmdQueue  ( ) const;

    IcOclBuffCache  buffCache( ) const;
    IcOclProgCache  progCache( ) const;
    
private:
    void *m_obj;
};

}

#endif
