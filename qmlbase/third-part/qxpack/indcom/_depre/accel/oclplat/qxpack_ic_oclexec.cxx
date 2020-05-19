#ifndef QXPACK_IC_OCLEXEC_CXX
#define QXPACK_IC_OCLEXEC_CXX

#include "qxpack_ic_oclexec.hxx"
#include "qxpack_ic_oclkernel.hxx"
#include "qxpack_ic_oclsgldev.hxx"
#include "qxpack_ic_oclvector4d.hxx"

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>

#if !defined( CL_USE_DEPRECATED_OPENCL_1_1_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif
#if !defined( CL_USE_DEPRECATED_OPENCL_1_2_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#include "ocl_libs/include/CL/cl.h"

#include <mutex>  //c++ 11
#include <list>   //c++ 11

namespace QxPack {

// //////////////////////////////////////////////////
//
//  IcOclExecPriv inner class
//
// //////////////////////////////////////////////////
#define T_PrivPtr( o ) (( IcOclExecPriv *) o )
class QXPACK_IC_HIDDEN  IcOclExecPriv : public IcPImplPrivTemp< IcOclExecPriv > {

private:
    cl_command_queue          m_cmd_queue;
    std::list< IcOclKernel >  m_krnl_list;
    std::mutex                m_locker;
protected:
    void     _clearKrnls( );
    cl_int   _enqKernel ( const IcOclKernel & );
public:
    IcOclExecPriv ( );
    IcOclExecPriv ( const IcOclExecPriv &other );
    virtual ~IcOclExecPriv ( );
    inline void      begin( ) { this->_clearKrnls(); }
    inline void      end  ( ) { }
    inline intptr_t  cmdQueueHandle( ) { return ( intptr_t ) m_cmd_queue; }

    bool  init ( cl_context, cl_device_id );
    bool  push ( const IcOclKernel & );
    bool  exec ( );
};

// =====================================================
// CTOR
// =====================================================
    IcOclExecPriv :: IcOclExecPriv ( )
{
    m_cmd_queue = 0;
}

// =====================================================
// CTOR ( copy )
// =====================================================
    IcOclExecPriv :: IcOclExecPriv ( const IcOclExecPriv & )
{
    qxpack_ic_fatal("IcOclExecPriv call COPY CTOR is not allowed!");
}

// =====================================================
// DTOR
// =====================================================
    IcOclExecPriv :: ~IcOclExecPriv ( )
{
    if ( m_cmd_queue != 0 ) { clReleaseCommandQueue( m_cmd_queue ); }
}

// =====================================================
// initalize
// =====================================================
bool  IcOclExecPriv :: init( cl_context ctxt , cl_device_id dev_id )
{
    if ( ctxt == 0 || dev_id == 0 ) { return false; }

    cl_int c_ret;   
    m_cmd_queue = clCreateCommandQueue( ctxt, dev_id, 0, & c_ret );
    if ( ! c_ret == CL_SUCCESS ) { m_cmd_queue = 0; }
    return true;
}

// =====================================================
// clear kernels [ protected ]
// =====================================================
void   IcOclExecPriv :: _clearKrnls( )
{
    m_krnl_list.swap( std::list< IcOclKernel >());
}

// =====================================================
// enqueue the kernel [ protected ]
// =====================================================
cl_int  IcOclExecPriv :: _enqKernel ( const IcOclKernel &k )
{
    if ( ! k.isReady() )    { return CL_INVALID_KERNEL; }

    // read out the dimension about the kernel
    IcOclVector4D  gwk_ofv, gwk_dim, lwk_dim; int dim_num;
    if ( ! k.getWorkDim( gwk_ofv, gwk_dim, lwk_dim, dim_num ))
    {  return CL_INVALID_WORK_GROUP_SIZE;  }
    if ( dim_num < 1 ) { return CL_INVALID_WORK_DIMENSION; }

    // try enqueue the kernel
    size_t t_gwk_ofv[3], t_gwk_dim[3], t_lwk_dim[3];
    t_gwk_ofv[0] = ( size_t ) gwk_ofv[0];  t_gwk_ofv[1] = ( size_t ) gwk_ofv[1];
    t_gwk_ofv[2] = ( size_t ) gwk_ofv[2];
    t_gwk_dim[0] = ( size_t ) gwk_dim[0];  t_gwk_dim[1] = ( size_t ) gwk_dim[1];
    t_gwk_dim[2] = ( size_t ) gwk_dim[2];
    t_lwk_dim[0] = ( size_t ) lwk_dim[0];  t_lwk_dim[1] = ( size_t ) lwk_dim[1];
    t_lwk_dim[2] = ( size_t ) lwk_dim[2];

    return clEnqueueNDRangeKernel( m_cmd_queue, ( cl_kernel ) k.handle( ), ( cl_uint )( dim_num ),
                                    t_gwk_ofv, t_gwk_dim, t_lwk_dim, 0, NULL,
                                    NULL );
}

// ===================================================
// push the kernel into list
// ===================================================
bool   IcOclExecPriv :: push ( const IcOclKernel &k )
{
    bool is_pushed = false;

    if ( k.isReady()) {
        m_locker.lock();
        m_krnl_list.push_back( k );
        m_locker.unlock();
    } else {
        qxpack_ic_fatal("pushed an empty opencl kernel!" );
    }
    return is_pushed;
}

// ====================================================
// execute the kernel list
// ====================================================
bool    IcOclExecPriv :: exec ( )
{
    if ( m_cmd_queue == 0 ) { return false; }

    m_locker.lock();

    std::list<IcOclKernel>::const_iterator c_itr = m_krnl_list.cbegin();
    while ( c_itr != m_krnl_list.cend()) {
        cl_int c_ret = this->_enqKernel( (* c_itr ++ ) );
        if ( c_ret != CL_SUCCESS ) {
            qxpack_ic_fatal("clEnqueueNDRangeKernel() can not push the kernel!" );
        }
    }
    clFlush( m_cmd_queue );
    clFinish( m_cmd_queue );

    m_locker.unlock();

    return true;
}

// ///////////////////////////////////////////////////////
//
// IcOclExec wrapper ( NOTE: do not use COW!)
//
// ///////////////////////////////////////////////////////
// ========================================================
// create an empty object
// ========================================================
    IcOclExec :: IcOclExec ( ) { m_obj = nullptr; }

// ========================================================
// create an valid object
// ========================================================
    IcOclExec :: IcOclExec ( const IcOclSglDev &dev )
{
    m_obj = nullptr;
    IcOclExecPriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->init( ( cl_context ) dev.context(), ( cl_device_id ) dev.deviceId() )) {
        IcOclExecPriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// ========================================================
// attach an existed object ( share the content )
// ========================================================
    IcOclExec :: IcOclExec ( const IcOclExec &other )
{
    m_obj = nullptr;
    IcOclExecPriv::attach( & m_obj, ( void **) & other.m_obj );
}

// ========================================================
// copy assign the object ( attach to another, share the content )
// ========================================================
IcOclExec &  IcOclExec :: operator = ( const IcOclExec &other )
{
    IcOclExecPriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// ===========================================================
// DTOR
// ===========================================================
    IcOclExec :: ~IcOclExec( )
{
    if ( m_obj != nullptr ) { IcOclExecPriv::attach( & m_obj, nullptr );  }
}

// ===========================================================
// access the property
// ============================================================
bool  IcOclExec :: isReady( ) const { return ( m_obj != nullptr ); }
void  IcOclExec :: begin( )  { if ( m_obj != nullptr ) { T_PrivPtr( m_obj )->begin(); }}
void  IcOclExec :: end  ( )  { if ( m_obj != nullptr ) { T_PrivPtr( m_obj )->end();   }}
bool  IcOclExec :: exec ( )  { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->exec() : false ); }

// return the command queue handle
intptr_t  IcOclExec :: cmdQueueHandle() const
{
    return ( m_obj != nullptr ? T_PrivPtr( m_obj )->cmdQueueHandle() : 0 );
}

// push the kernel into inner list for ready to use.
bool  IcOclExec :: push( const IcOclKernel &k )
{
    return ( m_obj != nullptr ? T_PrivPtr( m_obj )->push( k ) : false );
}

}
#endif

