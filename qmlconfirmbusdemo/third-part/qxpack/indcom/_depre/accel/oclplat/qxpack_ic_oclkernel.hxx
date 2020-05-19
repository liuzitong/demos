#ifndef QXPACK_IC_OCLKERNEL_HXX
#define QXPACK_IC_OCLKERNEL_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>
#include <stdint.h>

namespace QxPack {

class IcOclProgCache;
class IcOclBuffer;
class IcOclVector4D;

// ///////////////////////////////////////////
//
// OpenCL kernel wrapper
//
// ///////////////////////////////////////////
class QXPACK_IC_API IcOclKernel {

public:
    IcOclKernel( );
    IcOclKernel( const IcOclKernel & );
    IcOclKernel &  operator = ( const IcOclKernel & );
    virtual ~IcOclKernel( );

    bool      isReady( ) const;
    int       workGroupSize( ) const;
    int       localMemSize ( ) const;
    intptr_t  handle( ) const;

    bool      setArg  ( int idx, const IcOclBuffer & );
    bool      setArg  ( int idx, const IcByteArray & );
    bool      setWorkDim( const IcOclVector4D &gwk_ofv, const IcOclVector4D &gwk_dim,
                          const IcOclVector4D &lwk_dim, int dim_num );
    bool      getWorkDim( IcOclVector4D &gwk_ofv, IcOclVector4D &gwk_dim,
                          IcOclVector4D &lwk_dim, int &dim_num  ) const;

protected:
    IcOclKernel( intptr_t krnl_id, intptr_t dev_id );

friend class IcOclProgCachePriv;

private:
    void *m_obj;
};

}
#endif
