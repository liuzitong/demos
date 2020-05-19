#ifndef QXPACK_IC_OCLKERNEL_CXX
#define QXPACK_IC_OCLKERNEL_CXX

#include "qxpack_ic_oclkernel.hxx"
#include "qxpack_ic_oclvector4d.hxx"
#include "qxpack_ic_oclbuffer.hxx"
#include "qxpack_ic_oclsgldev.hxx"

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>

#if !defined( CL_USE_DEPRECATED_OPENCL_1_1_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif

#if !defined( CL_USE_DEPRECATED_OPENCL_1_2_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#include "ocl_libs/include/CL/cl.h"

#include <mutex> // C++ 11

#define BUFF_ARG_NUM 16

namespace QxPack {

// //////////////////////////////////////////////////
//
// OclKernelPriv
//
// //////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcOclKernelPriv *) o )
class QXPACK_IC_HIDDEN IcOclKernelPriv : public IcPImplPrivTemp< IcOclKernelPriv >{

private:
    cl_kernel      m_krnl;
    int            m_wkg_sz, m_loc_mem_size, m_dim_num;
    IcOclVector4D  m_global_wk_ofv, m_global_wk_dim;
    IcOclVector4D  m_local_wk_dim;
    IcOclBuffer    m_buff_arg[ BUFF_ARG_NUM ];
    std::mutex     m_locker;

protected:
    inline bool   isArgIndexOK( int idx ) { return ( idx >= 0 && idx < BUFF_ARG_NUM ); }
public :
    IcOclKernelPriv ( );
    IcOclKernelPriv ( const IcOclKernelPriv & );
    virtual ~IcOclKernelPriv( );

    inline int       workgroupSize( ) { return m_wkg_sz; }
    inline int       localMemSize ( ) { return m_loc_mem_size; }
    inline intptr_t  handle( )        { return ( intptr_t ) m_krnl; }

    bool   init   ( cl_kernel, cl_device_id );
    bool   setArg ( int idx, const IcOclBuffer & );
    bool   setArg ( int idx, const IcByteArray & );
    bool   setWorkDim( const IcOclVector4D &gwk_ofv, const IcOclVector4D &gwk_dim,
                       const IcOclVector4D &lwk_dim, int dim_num );
    bool   getWorkDim( IcOclVector4D &gwk_ofv, IcOclVector4D &gwk_dim,
                           IcOclVector4D &lwk_dim, int &dim_num  );
};

// ======================================================
// CTOR
// ======================================================
    IcOclKernelPriv :: IcOclKernelPriv ( )
{
    m_krnl   = 0;
    m_wkg_sz = 0; m_loc_mem_size = 0; m_dim_num = 0;
}

// ======================================================
// CTOR ( Copy )
// ======================================================
    IcOclKernelPriv :: IcOclKernelPriv ( const IcOclKernelPriv &other )
{
    qxpack_ic_fatal("IcOclKernelPriv call COPY CTOR is not allowed!");
}

// ======================================================
// DTOR
// ======================================================
    IcOclKernelPriv :: ~IcOclKernelPriv( )
{
    if ( m_krnl  != 0 ) { clReleaseKernel ( m_krnl );  m_krnl = 0;   }
}

// ======================================================
// initalize the kernel [ called by IcOclKernel ]
// ======================================================
bool   IcOclKernelPriv :: init ( cl_kernel krnl, cl_device_id dev )
{
    if ( krnl == 0 || dev == 0 ) { return false; }
    m_locker.lock();

    m_krnl = krnl;
    clRetainKernel ( krnl );

    // query the workgroup information
    size_t sz  = 0;
    cl_int ret = clGetKernelWorkGroupInfo( krnl, dev ,
                                           CL_KERNEL_WORK_GROUP_SIZE, sizeof( sz ), & sz, nullptr );
    if ( ret == CL_SUCCESS ) { m_wkg_sz = ( int )( sz ); }
    ret = clGetKernelWorkGroupInfo( krnl, dev,
                                           CL_KERNEL_LOCAL_MEM_SIZE,  sizeof( sz ), & sz, nullptr );
    if ( ret == CL_SUCCESS ) { m_loc_mem_size = ( int )( sz ); }

    m_locker.unlock();
    return true;
}

// =====================================================
// setup the argument by buffer
// =====================================================
bool   IcOclKernelPriv :: setArg ( int idx, const IcOclBuffer &buff )
{
    if ( ! this->isArgIndexOK( idx ) || ! buff.isReady() ) { return false; }

    cl_mem mem = ( cl_mem ) buff.handle();

    m_locker.lock();
    cl_int c_ret = clSetKernelArg ( m_krnl, ( cl_uint )( idx ), sizeof( cl_mem ), & mem );
    if ( c_ret == CL_SUCCESS ) { m_buff_arg[ idx ] = buff; }
    m_locker.unlock();

    return ( c_ret == CL_SUCCESS ? true : false );
}

// ======================================================
// setup the argument by byte array
// ======================================================
bool    IcOclKernelPriv :: setArg ( int idx, const IcByteArray &ba )
{
    if ( ! this->isArgIndexOK( idx ) || ba.isEmpty() ) { return false; }

    m_locker.lock  ();
    cl_int c_ret = clSetKernelArg( m_krnl, ( cl_uint )( idx ), ( size_t )( ba.size() ), ba.constData() );
    if ( c_ret == CL_SUCCESS ) { m_buff_arg[ idx ] = IcOclBuffer(); }
    m_locker.unlock();

    return ( c_ret == CL_SUCCESS ? true : false );
}

// ======================================================
// setup the work dimension
// ======================================================
bool    IcOclKernelPriv ::  setWorkDim( const IcOclVector4D &gwk_ofv, const IcOclVector4D &gwk_dim,
                          const IcOclVector4D &lwk_dim, int dim_num )
{
    // NOTE: here only remember the work dim. this will be used in command queue
    m_locker.lock();

    m_global_wk_ofv = gwk_ofv;
    m_global_wk_dim = gwk_dim;
    m_local_wk_dim  = lwk_dim;
    m_dim_num       = dim_num;

    m_locker.unlock();

    return true;
}

// ======================================================
// readout the dimension
// ======================================================
bool   IcOclKernelPriv ::  getWorkDim( IcOclVector4D &gwk_ofv, IcOclVector4D &gwk_dim,
                                       IcOclVector4D &lwk_dim, int &dim_num  )
{
    m_locker.lock ( );

    gwk_ofv = m_global_wk_ofv;
    gwk_dim = m_global_wk_dim;
    lwk_dim = m_local_wk_dim;
    dim_num = m_dim_num;

    m_locker.unlock( );

    return true;
}

// //////////////////////////////////////////////////////////
//
// IcOclKernel wrapper
// NOTE: here we use share object, not use COW!
//
// //////////////////////////////////////////////////////////
// ==========================================================
// create an empty object
// ==========================================================
    IcOclKernel :: IcOclKernel( ) { m_obj = nullptr; }

// ===========================================================
// create the object by other ( attach to other, share the content )
// ===========================================================
    IcOclKernel :: IcOclKernel ( const IcOclKernel &other )
{
    m_obj = nullptr;
    IcOclKernelPriv::attach( & m_obj, ( void **) & other.m_obj );
}

// ===========================================================
// copy assign ( attach to other, share the content )
// ===========================================================
IcOclKernel &  IcOclKernel :: operator = ( const IcOclKernel &other )
{
    IcOclKernelPriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// ===========================================================
// DTOR
// ===========================================================
    IcOclKernel :: ~IcOclKernel ( )
{
    if ( m_obj != nullptr ) {
        IcOclKernelPriv::attach( & m_obj, nullptr );
    }
}

// ==========================================================
//  create a valid object
// ==========================================================
    IcOclKernel :: IcOclKernel ( intptr_t krnl_id, intptr_t dev_id )
{
    m_obj = nullptr;
    IcOclKernelPriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->init( ( cl_kernel ) krnl_id, ( cl_device_id ) dev_id )) {
        IcOclKernelPriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// =========================================================
// access the property
// =========================================================
bool   IcOclKernel :: isReady( )       const { return ( m_obj != nullptr ); }
int    IcOclKernel :: workGroupSize( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->workgroupSize() : 0 ); }
int    IcOclKernel :: localMemSize ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->localMemSize()  : 0 ); }
intptr_t  IcOclKernel :: handle() const      { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->handle() : 0 ); }

bool   IcOclKernel :: setArg  ( int idx, const IcOclBuffer &buf )
{  return ( m_obj != nullptr ? T_PrivPtr( m_obj )->setArg( idx, buf ) : false ); }

bool   IcOclKernel :: setArg  ( int idx, const IcByteArray &ba )
{  return ( m_obj != nullptr ? T_PrivPtr( m_obj )->setArg( idx, ba ) : false ); }

bool   IcOclKernel :: setWorkDim( const IcOclVector4D &gwk_ofv, const IcOclVector4D &gwk_dim,
                                  const IcOclVector4D &lwk_dim, int dim_num )
{  return ( m_obj != nullptr ? T_PrivPtr( m_obj )->setWorkDim( gwk_ofv, gwk_dim, lwk_dim, dim_num ) : false ); }

bool   IcOclKernel :: getWorkDim( IcOclVector4D &gwk_ofv, IcOclVector4D &gwk_dim,
                                  IcOclVector4D &lwk_dim, int &dim_num  ) const
{  return ( m_obj != nullptr ? T_PrivPtr( m_obj )->setWorkDim( gwk_ofv, gwk_dim, lwk_dim, dim_num ) : false ); }



}
#endif
