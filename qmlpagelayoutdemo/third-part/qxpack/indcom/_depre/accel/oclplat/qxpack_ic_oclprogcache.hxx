#ifndef QXPACK_IC_OCLPROGCACHE_HXX
#define QXPACK_IC_OCLPROGCACHE_HXX

#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>

namespace QxPack {

// //////////////////////////////////////////////////////////
//
//               OpenCL program cache
// this class used for store all compiled programs.
//
//
// /////////////////////////////////////////////////////////
class IcOclKernel;

class QXPACK_IC_API IcOclProgCache {

public:
    IcOclProgCache ( );
    IcOclProgCache ( intptr_t ctxt, intptr_t dev_id );
    IcOclProgCache ( const IcOclProgCache & );       
    IcOclProgCache &  operator = ( const IcOclProgCache & );
    virtual ~IcOclProgCache ( );

    bool   isReady ( ) const;
    void   clear   ( );
    bool   addProgram       ( const char *prog_name, const char *prog_text, IcByteArray  &error, const char *comp_opt = nullptr );
    bool   rmvProgram       ( const char *prog_name );
    bool   isProgramExisted ( const char *prog_name );
    
    IcOclKernel  createKernel ( const char *krnl_name, const char *prog_name );

private:
    void *m_obj;
};

}

#endif
