#ifndef QXPACK_IC_OCLBUFFER_CXX
#define QXPACK_IC_OCLBUFFER_CXX

#include "qxpack_ic_oclbuffer.hxx"
#include "qxpack_ic_oclsgldev.hxx"
#include "qxpack_ic_oclexec.hxx"

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>

#if !defined( CL_USE_DEPRECATED_OPENCL_1_1_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif
#if !defined( CL_USE_DEPRECATED_OPENCL_1_2_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#include "ocl_libs/include/CL/cl.h"

#include <mutex> //C++11

namespace QxPack {

// //////////////////////////////////////////////
//
// OpenCL buffer private object
//
// //////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcOclBufferPriv *) o )
class QXPACK_IC_HIDDEN  IcOclBufferPriv : public IcPImplPrivTemp< IcOclBufferPriv >{

private:
    cl_command_queue  m_cmd_queue;
    cl_mem       m_mem;
    int          m_cap_size;
    int          m_size;
    std::mutex   m_locker;
public :
    IcOclBufferPriv ( );
    IcOclBufferPriv ( const IcOclBufferPriv & );
    virtual ~IcOclBufferPriv( );
    inline int       size ()  const { return m_size; }
    inline int       capSize()const { return m_cap_size;  }
    inline intptr_t  handle() const { return ( intptr_t ) m_mem; }
    inline void      resize( int size ) { if ( size <= m_cap_size ) { m_size = m_cap_size; }}
    bool  initAsArray ( cl_context ctxt, cl_command_queue cq, int cap_size );
    bool  write ( const IcByteArray & );
    bool  read  (       IcByteArray & );
};

// ====================================================
// CTOR
// ====================================================
    IcOclBufferPriv :: IcOclBufferPriv ( )
{
    m_mem = 0; m_cmd_queue = 0; m_size = 0; m_cap_size = 0;
}

// ====================================================
// CTOR ( Copy )
// ====================================================
    IcOclBufferPriv :: IcOclBufferPriv ( const IcOclBufferPriv & )
{
    qxpack_ic_fatal("IcOclBuffPriv call COPY CTOR is not allowed!");
}

// =====================================================
// DTOR
// =====================================================
    IcOclBufferPriv :: ~IcOclBufferPriv ( )
{
    if ( m_mem       != 0 ) { clReleaseMemObject   ( m_mem ); }
    if ( m_cmd_queue != 0 ) { clReleaseCommandQueue( m_cmd_queue );  }
}

// =====================================================
// initalize the buffer as normal array
// =====================================================
bool   IcOclBufferPriv :: initAsArray (cl_context ctxt, cl_command_queue cq, int cap_size )
{
    if ( ctxt == 0 || cq == 0 ) { return false; }
    if ( cap_size < 1 ) { cap_size = 1024; }

    cl_int c_ret;
    m_mem = clCreateBuffer ( ctxt, CL_MEM_READ_WRITE, cap_size, 0, &c_ret );
    if ( c_ret == CL_SUCCESS ) {
        m_cmd_queue = cq;
        if ( cq != 0 ) { clRetainCommandQueue( cq ); }
        m_size     = cap_size;
        m_cap_size = cap_size;
    } else {
        m_mem = 0; m_size = 0; m_cap_size = 0;
    }

    return ( c_ret == CL_SUCCESS );
}

// =====================================================
// write the data into buffer
// =====================================================
bool   IcOclBufferPriv :: write  ( const IcByteArray &data )
{
    // check paramters
    if ( data.isEmpty( )  ||  0 == m_cmd_queue || 0 == m_mem )
    { return false; }

    // upload the data into OpenCL device memory
    const char *dp  = data.constData( );
    int         dsz = data.size();
    cl_int c_ret = clEnqueueWriteBuffer( m_cmd_queue, m_mem, CL_TRUE, 0,
                                        ( dsz <= m_size ? dsz : m_size ), dp, 0, NULL, NULL
                                       );
    return ( c_ret == CL_SUCCESS );
}

// =====================================================
// read the data from buffer
// =====================================================
bool   IcOclBufferPriv :: read   (       IcByteArray &data )
{
    // check paramters
    if ( data.isEmpty( )  || 0 == m_cmd_queue || 0 == m_mem )
    { return false; }

    // download the data from OpenCL device memory
    char *dp  = data.data();  // note the deep copy if shared.
    int   dsz = data.size();
    cl_int c_ret = clEnqueueReadBuffer( m_cmd_queue, m_mem, CL_TRUE, 0,
                                        ( dsz <= m_size ? dsz : m_size ), dp, 0, NULL, NULL
                                      );
    return ( c_ret == CL_SUCCESS );
}


// ////////////////////////////////////////////////////////
//
// buffer wrapper API
//
// ////////////////////////////////////////////////////////
// =======================================================
// creat an empty object
// =======================================================
    IcOclBuffer :: IcOclBuffer( ) { m_obj = nullptr; }

// =======================================================
// create a valid object
// =======================================================
    IcOclBuffer :: IcOclBuffer( intptr_t ctxt, intptr_t cq, int cap_size )
{
    m_obj = nullptr;
    IcOclBufferPriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->initAsArray( ( cl_context ) ctxt, ( cl_command_queue ) cq, cap_size )) {
        IcOclBufferPriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// =======================================================
// create by other ( attach to other, share the content )
// =======================================================
    IcOclBuffer :: IcOclBuffer( const IcOclBuffer &other )
{
    m_obj = nullptr;
    IcOclBufferPriv::attach( & m_obj, ( void **) & other.m_obj );
}

// =======================================================
// copy assign ( attach to other, share the content )
// =======================================================
IcOclBuffer &  IcOclBuffer :: operator = ( const IcOclBuffer &other )
{
    IcOclBufferPriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// =======================================================
// DTOR
// =======================================================
    IcOclBuffer :: ~IcOclBuffer( )
{
    if ( m_obj != nullptr ) {
        IcOclBufferPriv::attach( & m_obj, nullptr );
    }
}

// =======================================================
// resize the data size
// @note data size <= capacity()
// =======================================================
void    IcOclBuffer :: resize ( int sz )
{
    if ( m_obj != nullptr ) {
        T_PrivPtr( m_obj )->resize( ( sz <= 0 ? 1 : sz ));
    }
}

// =======================================================
// access the property & method
// =======================================================
bool     IcOclBuffer :: isNull ( ) const { return ( m_obj == nullptr ); }
bool     IcOclBuffer :: isReady( ) const { return ( m_obj != nullptr ); }
bool     IcOclBuffer :: isImage( ) const { return false; } // current not image
int      IcOclBuffer :: size   ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->size() : 0 );   }
intptr_t IcOclBuffer :: handle ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->handle() : 0 ); }
int      IcOclBuffer :: capacity() const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->capSize() : 0 );}

bool     IcOclBuffer :: write  ( const IcByteArray &ba )
{
    return ( m_obj != nullptr ? T_PrivPtr( m_obj )->write( ba ) : false );
}

bool     IcOclBuffer :: read   (       IcByteArray &ba )
{
    return ( m_obj != nullptr ? T_PrivPtr( m_obj )->read( ba ) : false );
}




}
#endif

