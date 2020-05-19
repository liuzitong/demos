#ifndef QXPACK_IC_OCLSGLDEV_CXX
#define QXPACK_IC_OCLSGLDEV_CXX

#include "qxpack_ic_oclsgldev.hxx"
#include "qxpack_ic_oclprogcache.hxx"
#include "qxpack_ic_oclbuffcache.hxx"

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

#include <mutex>   //C++11
#include <cstring> //C++11
#include <memory>  //C++11
#include <cstdlib> //C++11

namespace QxPack {

// /////////////////////////////////////////////////////
//
// inner class
//
// /////////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcOclSglDevPriv *) o )
class QXPACK_IC_HIDDEN  IcOclSglDevPriv : public IcPImplPrivTemp< IcOclSglDevPriv > {

private:
    IcOclProgCache  m_prog_cache;
    IcOclBuffCache  m_buff_cache;
    cl_context      m_context;
    cl_device_id    m_dev_id;
    cl_platform_id  m_plat_id;
    cl_command_queue m_cmd_queue;
    std::mutex      m_locker;
    IcByteArray     m_plat_name, m_vendor_name, m_plat_version;

protected:
    bool   createDeviceByPlat ( cl_platform_id, cl_device_id );
    bool   enumAndCreate ( cl_platform_id plat_id, cl_device_type, int dev_idx );

    inline static int      platNum( ) { cl_uint plat_num = 0; clGetPlatformIDs( 0, 0, & plat_num ); return ( int )( plat_num ); }
    inline static cl_uint  devNum ( cl_platform_id  p_id, cl_device_type dev_type ) { cl_uint dev_num  = 0;  clGetDeviceIDs( p_id, dev_type, 0, nullptr, & dev_num ); return dev_num; }
    inline static bool     platIDs( cl_platform_id *p_id, int num ) { return ( clGetPlatformIDs( ( cl_uint )num, p_id, 0 ) == CL_SUCCESS ? true : false ); }

public :
    IcOclSglDevPriv ( );
    IcOclSglDevPriv ( const IcOclSglDevPriv & );
    virtual ~IcOclSglDevPriv( );

    bool   init( IcOclSglDev::Type, int, int  );

    inline intptr_t  context   ( ) const { return ( intptr_t )( m_context ); }
    inline intptr_t  deviceId  ( ) const { return ( intptr_t )( m_dev_id  ); }
    inline intptr_t  platformId( ) const { return ( intptr_t )( m_plat_id ); }
    inline intptr_t  cmdQueue  ( ) const { return ( intptr_t )( m_cmd_queue); }
    inline IcOclProgCache  progCache( ) const { return m_prog_cache; }
    inline IcOclBuffCache  buffCache( ) const { return m_buff_cache; }
};

// =======================================================
// CTOR
// =======================================================
    IcOclSglDevPriv :: IcOclSglDevPriv ( )
{
    m_context = 0; m_dev_id = 0; m_plat_id = 0; m_cmd_queue = 0;
}

// =======================================================
// CTOR ( copy )
// =======================================================
    IcOclSglDevPriv :: IcOclSglDevPriv ( const IcOclSglDevPriv & )
{
    qxpack_ic_fatal("IcOclSglDevPriv call COPY CTOR is not allowed!");
}

// =======================================================
// DTOR
// =======================================================
    IcOclSglDevPriv :: ~IcOclSglDevPriv ( )
{
    m_buff_cache = IcOclBuffCache();
    m_prog_cache = IcOclProgCache();
    if ( m_cmd_queue != 0 ) { clReleaseCommandQueue( m_cmd_queue ); }
    if ( m_dev_id    != 0 ) { clReleaseDevice ( m_dev_id  ); }
    if ( m_context   != 0 ) { clReleaseContext( m_context ); }
}

// =======================================================
// create the device by platform id [ protected ]
// =======================================================
bool  IcOclSglDevPriv :: createDeviceByPlat( cl_platform_id p_id, cl_device_id d_id )
{
    char   tmp_buff[ 1024 ];
    cl_int c_ret;

    std::memset( tmp_buff, 0, sizeof( tmp_buff ));
    c_ret = clGetPlatformInfo( p_id, CL_PLATFORM_NAME, sizeof( tmp_buff ) - 2, tmp_buff, NULL );
    if ( c_ret == CL_SUCCESS ) { m_plat_name = IcByteArray( tmp_buff ); }

    if ( c_ret == CL_SUCCESS ) {
        std::memset( tmp_buff, 0, sizeof( tmp_buff ));
        c_ret = clGetPlatformInfo( p_id, CL_PLATFORM_VENDOR, sizeof( tmp_buff ) - 2, tmp_buff, NULL );
    }
    if ( c_ret == CL_SUCCESS ) { m_vendor_name = IcByteArray( tmp_buff ); }

    if ( c_ret == CL_SUCCESS ) {
        std::memset( tmp_buff, 0, sizeof( tmp_buff ));
        c_ret = clGetPlatformInfo( p_id, CL_PLATFORM_VERSION, sizeof( tmp_buff ) - 2, tmp_buff, NULL );
    }
    if ( c_ret == CL_SUCCESS ) { m_plat_version= IcByteArray( tmp_buff ); }

    if ( c_ret == CL_SUCCESS ) {
        m_plat_id = p_id;
        m_dev_id  = d_id;
        m_context = clCreateContext( 0, 1, & m_dev_id, 0, 0, & c_ret );
    }
    if ( c_ret == CL_SUCCESS ) {
        m_cmd_queue = clCreateCommandQueue( m_context, m_dev_id, 0, & c_ret );
    }

    if ( c_ret == CL_SUCCESS ) {
        m_prog_cache = IcOclProgCache( ( intptr_t ) m_context, ( intptr_t ) m_dev_id    );
        m_buff_cache = IcOclBuffCache( ( intptr_t ) m_context, ( intptr_t ) m_cmd_queue );
    } else {
        if ( m_cmd_queue != 0 ) { clReleaseCommandQueue( m_cmd_queue ); }
        if ( m_dev_id    != 0 ) { clReleaseDevice ( m_dev_id  ); }
        if ( m_context   != 0 ) { clReleaseContext( m_context ); }

        m_plat_id = 0; m_dev_id = 0; m_context = 0; m_cmd_queue = 0;
        m_plat_name    = IcByteArray( );
        m_vendor_name  = IcByteArray( );
        m_plat_version = IcByteArray( );
    }

    return ( c_ret == CL_SUCCESS );
}

// =======================================================
// enum and create
// =======================================================
bool   IcOclSglDevPriv :: enumAndCreate(cl_platform_id plat_id, cl_device_type dev_type, int dev_idx )
{
    bool is_found = false; cl_int ret;

    // get all device ids in this platform
    cl_uint dev_num = IcOclSglDevPriv::devNum( plat_id, dev_type );
    if ( dev_idx >= ( int ) dev_num ) { return false; }
    cl_device_id  dev_ids[ 32 ];
    ret = clGetDeviceIDs( plat_id, dev_type, ( dev_num > 32 ? 32 : dev_num ), & dev_ids[0], & dev_num );
    if ( ret != CL_SUCCESS  ) { return false; }
    if ( dev_idx >= ( int ) dev_num ) { return false; }

    // check if device is auto found or specified index
    if ( dev_idx > 0 ) { // only test the specified index of device
        return this->createDeviceByPlat( plat_id, dev_ids[ dev_idx ] );
    } else {
        for ( int i = 0; i < ( int ) dev_num; i ++ ) {
            if ( this->createDeviceByPlat( plat_id, dev_ids[i])) {
                is_found = true;
                break;
            }
        }
    }
    return is_found;
}

// =======================================================
// initalize the device
// =======================================================
bool   IcOclSglDevPriv :: init (IcOclSglDev::Type type, int plat_idx , int dev_idx )
{
    // first query out the platform list
    int plat_num = IcOclSglDevPriv::platNum( );
    if ( plat_idx >= plat_num ) { return false; }
    cl_platform_id plat_ids[ 32 ];
    if ( ! IcOclSglDevPriv::platIDs( plat_ids, ( plat_num > 32 ? 32 : plat_num ))) { return false; }
    if ( plat_idx >= plat_num ) { return false; }

    // found and create device
    bool is_found = false;
    if ( plat_idx > 0 ) {
        // create the specified device
        if ( type == IcOclSglDev::Type_Auto ) {
            is_found = this->enumAndCreate( plat_ids[ plat_idx ], CL_DEVICE_TYPE_GPU, dev_idx );
            if ( !is_found ) { is_found = this->enumAndCreate( plat_ids[ plat_idx ], CL_DEVICE_TYPE_CPU, dev_idx ); }
        } else {
            is_found = this->enumAndCreate( plat_ids[ plat_idx ], type, dev_idx );
        }
    } else {
        // enum all platform, try found the device
        if ( type == IcOclSglDev::Type_Auto ) {
            for ( int i = 0; i < plat_num; i ++ ) {
                is_found = this->enumAndCreate( plat_ids[i], CL_DEVICE_TYPE_GPU, dev_idx );
                if ( is_found ) { break; }
            }
            if ( !is_found ) {
                for ( int i = 0; i < plat_num; i ++ ) {
                    is_found = this->enumAndCreate( plat_ids[i], CL_DEVICE_TYPE_CPU, dev_idx );
                    if ( is_found ) { break; }
                }
            }
        } else {
            for ( int i = 0; i < plat_num; i ++ ) {
                is_found = this->enumAndCreate( plat_ids[i], type, dev_idx );
                if ( ! is_found ) { break; }
            }
        }
    }

    return is_found;
}

// //////////////////////////////////////////////////////////
//
//  IcOclSglDev wrapper API
//
// //////////////////////////////////////////////////////////
// ==========================================================
// CTOR
// ==========================================================
    IcOclSglDev :: IcOclSglDev( ) { m_obj = nullptr;  }

// ==========================================================
// CTOR ( create an valid object )
// ==========================================================
    IcOclSglDev :: IcOclSglDev( Type type, int plat_idx , int dev_idx)
{
    m_obj = nullptr;
    IcOclSglDevPriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->init( type, plat_idx, dev_idx )) {
        IcOclSglDevPriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// =========================================================
// CTOR ( Copy )
// =========================================================
    IcOclSglDev :: IcOclSglDev( const IcOclSglDev &other )
{
    m_obj = nullptr;
    IcOclSglDevPriv::attach( & m_obj, ( void **) &other.m_obj );
}

// =========================================================
// DTOR
// =========================================================
    IcOclSglDev :: ~IcOclSglDev( )
{
    if ( m_obj != nullptr ) {
        IcOclSglDevPriv::attach( & m_obj, nullptr );
    }
}

// =========================================================
//  copy assign
// =========================================================
IcOclSglDev &    IcOclSglDev :: operator = ( const IcOclSglDev &other )
{
    IcOclSglDevPriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// =========================================================
// check if the object is ready or not
// =========================================================
bool             IcOclSglDev :: isReady   ( ) const
{   return ( m_obj != nullptr );  }

// =========================================================
// property access
// =========================================================
intptr_t  IcOclSglDev :: platformId( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->platformId() : 0 ); }
intptr_t  IcOclSglDev :: deviceId  ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->deviceId()   : 0 ); }
intptr_t  IcOclSglDev :: context   ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->context()    : 0 ); }
intptr_t  IcOclSglDev :: cmdQueue  ( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->cmdQueue()   : 0 ); }
IcOclProgCache  IcOclSglDev :: progCache( ) const { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->progCache() : IcOclProgCache()); }
IcOclBuffCache  IcOclSglDev :: buffCache() const  { return ( m_obj != nullptr ? T_PrivPtr( m_obj )->buffCache() : IcOclBuffCache()); }

}
#endif

