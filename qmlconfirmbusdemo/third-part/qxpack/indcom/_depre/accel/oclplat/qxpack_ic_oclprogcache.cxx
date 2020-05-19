#ifndef QXPACK_IC_OCLPROGCACHE_CXX
#define QXPACK_IC_OCLPROGCACHE_CXX

#include "qxpack_ic_oclprogcache.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>

#define QXPACK_IcPImplPrivTemp_new    qxpack_ic_new
#define QXPACK_IcPImplPrivTemp_delete qxpack_ic_delete
#include <qxpack/indcom/common/qxpack_ic_pimplprivtemp.hpp>
#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include "qxpack_ic_oclsgldev.hxx"
#include "qxpack_ic_oclkernel.hxx"

#include <map>    // c++11
#include <string> // C++11
#include <mutex>  // C++11

#if !defined( CL_USE_DEPRECATED_OPENCL_1_1_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif

#if !defined( CL_USE_DEPRECATED_OPENCL_1_2_APIS )
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#include "ocl_libs/include/CL/cl.h"

namespace QxPack {

// //////////////////////////////////////////////////////
//
//  ToDo: manage the programe name and cl program pair ( private class )
//
// //////////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcOclProgCachePriv_MapPriv {
private:
    std::map< std::string, cl_program > m_prog_map;
    std::mutex   m_locker;
protected:
    void        _incrClProgCntr( );
    void        _decrClProgCntr( );
public :
    IcOclProgCachePriv_MapPriv( ) { }
    virtual ~IcOclProgCachePriv_MapPriv( );
    bool        addProg ( const std::string &, cl_program, bool incr_prog_cntr = false );
    bool        rmvProg ( const std::string & );
    cl_program  findProg( const std::string & );

    void        clear ( )
    {
        m_locker.lock();
        this->_decrClProgCntr();
        m_prog_map.swap( std::map<std::string, cl_program>());
        m_locker.unlock();
    }
};

// ======================================================
// decrease the cl program counter all in map [ protected ]
// ======================================================
void  IcOclProgCachePriv_MapPriv :: _decrClProgCntr( )
{
    std::map< std::string, cl_program >::const_iterator citr = m_prog_map.cbegin();
    while ( citr != m_prog_map.cend() ) {
        cl_program p = citr->second; ++ citr;
        if ( p != 0 ) { clReleaseProgram( p ); }
    }
}

// ======================================================
// increase the cl program counter all in map [ protected ]
// ======================================================
void  IcOclProgCachePriv_MapPriv :: _incrClProgCntr()
{
    std::map< std::string, cl_program >::const_iterator citr = m_prog_map.cbegin();
    while ( citr != m_prog_map.cend() ) {
        cl_program p = citr->second; ++ citr;
        if ( p != 0 ) { clRetainProgram( p ); }
    }
}

// ======================================================
//  DTOR
// ======================================================
    IcOclProgCachePriv_MapPriv :: ~IcOclProgCachePriv_MapPriv( )
{
    this->clear();
}

// ======================================================
// add the program ( if already existed, replace the old one )
// ======================================================
bool  IcOclProgCachePriv_MapPriv :: addProg( const std::string &prog_name, cl_program prog, bool incr_prog_cntr )
{
    bool is_added = false;
    m_locker.lock();

    { // release the old program
        std::map< std::string, cl_program >::iterator itr = m_prog_map.find( prog_name );
        if ( itr != m_prog_map.end()) {
           if ( itr->second != 0 ) { clReleaseProgram( itr->second ); }
           m_prog_map.erase( itr );
        }
    }

    // add the program
    if ( prog != 0 ) {
        if ( incr_prog_cntr ) { clRetainProgram( prog ); }
        m_prog_map.insert( std::pair<std::string, cl_program>( prog_name, prog ));
        is_added = true;
    }

    m_locker.unlock();
    return is_added;
}

// =====================================================
// remove the program
// =====================================================
bool   IcOclProgCachePriv_MapPriv :: rmvProg( const std::string &prog_name )
{
    bool is_rmv = false;
    m_locker.lock();

    std::map< std::string, cl_program >::iterator itr = m_prog_map.find( prog_name );
    if ( itr != m_prog_map.end()) {
        if ( itr->second != 0 ) { clReleaseProgram( itr->second ); }
        m_prog_map.erase( itr );
        is_rmv = true;
    }

    m_locker.unlock();
    return is_rmv;
}

// =====================================================
// find the program
// =====================================================
cl_program  IcOclProgCachePriv_MapPriv :: findProg( const std::string &prog_name )
{
    cl_program p = 0;
    m_locker.lock();

    std::map< std::string, cl_program >::const_iterator citr = m_prog_map.find( prog_name );
    if ( citr != m_prog_map.cend()) { p = citr->second; }

    m_locker.unlock();
    return p;
}


// //////////////////////////////////////////////////////
//
// IcOclProgCachePriv  inner class
//
// //////////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcOclProgCachePriv *) o )
class QXPACK_IC_HIDDEN  IcOclProgCachePriv : public IcPImplPrivTemp< IcOclProgCachePriv >{

private:
    cl_context    m_context;
    cl_device_id  m_dev_id;
    IcOclProgCachePriv_MapPriv *m_prog_map; // shared object, do not use COW!
public :
    IcOclProgCachePriv ( );
    IcOclProgCachePriv ( const IcOclProgCachePriv & );
    virtual ~IcOclProgCachePriv( );
    bool   init( cl_context, cl_device_id );

    inline void  clear ( )  { m_prog_map->clear(); }
    inline bool  rmvProgram       ( const char *prog_name ) { return ( m_prog_map->rmvProg ( prog_name )); }
    inline bool  isProgramExisted ( const char *prog_name ) { return ( m_prog_map->findProg( prog_name ) != 0 ); }

    bool         addProgram  ( const char *prog_name, const char *prog_text, IcByteArray  &error, const char *comp_opt = nullptr );
    IcOclKernel  createKernel( const char *krnl_name, const char *prog_name );
};

// =======================================================
// CTOR
// =======================================================
    IcOclProgCachePriv :: IcOclProgCachePriv ( )
{
    m_context  = 0;
    m_dev_id   = 0;
    m_prog_map = qxpack_ic_new( IcOclProgCachePriv_MapPriv );
}

// =======================================================
// CTOR ( Copy )
// =======================================================
    IcOclProgCachePriv :: IcOclProgCachePriv ( const IcOclProgCachePriv & )
{
    qxpack_ic_fatal("IcOclProgCachePriv call COPY CTOR is not allowed!");
}

// =======================================================
// DTOR
// =======================================================
    IcOclProgCachePriv :: ~IcOclProgCachePriv ( )
{
    if ( m_prog_map != nullptr ) {
        qxpack_ic_delete( m_prog_map, IcOclProgCachePriv_MapPriv );
    }
    if ( m_context != 0 ) { clReleaseContext( m_context ); }
    if ( m_dev_id  != 0 ) { clReleaseDevice ( m_dev_id  ); }
}

// =======================================================
// initalize
// =======================================================
bool  IcOclProgCachePriv :: init ( cl_context ctxt, cl_device_id dev )
{
    if ( ctxt == 0 || dev == 0 ) { return false; }
    clRetainContext( m_context = ctxt );

    // in opecl 1.2 spec, if the device is a root device, the counter is not changed.
    clRetainDevice ( m_dev_id = dev );
    return true;
}

// =======================================================
// add the program, if the programe is existed, the old one will be replaced
// =======================================================
bool  IcOclProgCachePriv :: addProgram (const char *prog_name, const char *prog_text, IcByteArray  & error , const char *comp_opt )
{
    cl_program prog = 0;

    // check input paramters
    if ( prog_name == nullptr || prog_text == nullptr ) { return false; }

    // compile the source text
    cl_int c_ret = CL_SUCCESS;
    const char *cl_text[2] = { prog_text, nullptr };
    prog = clCreateProgramWithSource( m_context, 1, cl_text, nullptr, & c_ret );
    if ( c_ret != CL_SUCCESS ) {   }
    if ( c_ret == CL_SUCCESS ) { c_ret = clBuildProgram( prog, 1, & m_dev_id, comp_opt, nullptr, nullptr ); }
    if ( c_ret != CL_SUCCESS ) {
        IcByteArray err_ba( 8192 ); size_t err_len = err_ba.size();
        cl_int err_ret = clGetProgramBuildInfo( prog, m_dev_id, CL_PROGRAM_BUILD_LOG, err_len, err_ba.data(), & err_len );
        if ( err_ret != CL_SUCCESS ) {
           qxpack_ic_fatal("clGetProgramBuildInfo() can not get the build information." );
        }
        error = err_ba;
    }
    if ( c_ret == CL_SUCCESS ) {
        m_prog_map->addProg( prog_name, prog, false );
    } else {
        if ( prog != 0 ) { clReleaseProgram( prog ); }
    }

    return ( c_ret == CL_SUCCESS );
}

// =========================================================
// create the kernel by program text name and kernel name
// =========================================================
IcOclKernel   IcOclProgCachePriv :: createKernel( const char *krnl_name, const char *prog_name )
{
    IcOclKernel  krnl;
    if ( prog_name == nullptr || krnl_name == nullptr ) { return krnl; }

    cl_program prog = m_prog_map->findProg( prog_name );
    if ( prog != 0 ) {
        cl_int    c_ret;
        cl_kernel c_k = clCreateKernel( prog, krnl_name, & c_ret);
        if ( c_ret == CL_SUCCESS ) {
            krnl = IcOclKernel( ( intptr_t ) c_k, ( intptr_t ) m_dev_id );
            clReleaseKernel( c_k ); // because the IcOclKernel will increase the counter.
        }
    }
    return krnl;
}

// ////////////////////////////////////////////////////////
//
// wrapper of IcOclProgCache
//
// ////////////////////////////////////////////////////////
// ========================================================
// create an empty object
// ========================================================
    IcOclProgCache :: IcOclProgCache ( ) { m_obj = nullptr; }

// ========================================================
// create a valid object
// ========================================================
    IcOclProgCache :: IcOclProgCache ( intptr_t ctxt, intptr_t dev )
{
    m_obj = nullptr;
    IcOclProgCachePriv::buildIfNull( & m_obj );
    if ( ! T_PrivPtr( m_obj )->init( ( cl_context ) ctxt, ( cl_device_id ) dev )) {
        IcOclProgCachePriv::attach( & m_obj, nullptr );
        m_obj = nullptr;
    }
}

// ==========================================================
// create object by other one ( attach to other, share the content )
// ==========================================================
    IcOclProgCache :: IcOclProgCache ( const IcOclProgCache &other )
{
    m_obj = nullptr;
    IcOclProgCachePriv::attach( & m_obj, ( void **) & other.m_obj );
}

// =========================================================
// copy assign object, that means attach to other object
// =========================================================
IcOclProgCache & IcOclProgCache :: operator = ( const IcOclProgCache &other )
{
    IcOclProgCachePriv::attach( & m_obj, ( void **) & other.m_obj );
    return *this;
}

// ========================================================
// destructor
// ========================================================
    IcOclProgCache :: ~IcOclProgCache ( )
{
    if ( m_obj != nullptr ) {
        IcOclProgCachePriv::attach( & m_obj, nullptr );
    }
}

// ========================================================
// check if the object is ready ( also check the object is not an empty one )
// ========================================================
bool   IcOclProgCache :: isReady ( ) const { return ( m_obj != nullptr ); }

// ========================================================
// clear all program in the cache.
// ========================================================
void   IcOclProgCache :: clear ( )
{
    if ( m_obj != nullptr ) { T_PrivPtr( m_obj )->clear(); }
}

// ========================================================
// add a program into cache, the program is build form source
// ==========================================================
bool   IcOclProgCache :: addProgram ( const char *prog_name, const char *prog_text, IcByteArray  &error, const char *comp_opt )
{
    return ( m_obj != nullptr ?
             T_PrivPtr( m_obj )->addProgram( prog_name, prog_text, error, comp_opt ) :
             false
           );
}

// ==========================================================
// remove the existed program from cache
// ==========================================================
bool   IcOclProgCache :: rmvProgram ( const char *prog_name )
{
    return ( m_obj != nullptr ?
             T_PrivPtr( m_obj )->rmvProgram( prog_name ) :
             false
           );
}

// ==========================================================
// check if the program is existed or not
// ==========================================================
bool   IcOclProgCache :: isProgramExisted ( const char *prog_name )
{
    return ( m_obj != nullptr ?
             T_PrivPtr( m_obj )->isProgramExisted( prog_name ) :
             false
            );
}

// =========================================================
// create the kernel from the program.
// =========================================================
IcOclKernel  IcOclProgCache :: createKernel ( const char *krnl_name, const char *prog_name )
{
    return ( m_obj != nullptr ?
             T_PrivPtr( m_obj )->createKernel( krnl_name, prog_name ) :
             IcOclKernel()
           );
}


}

#endif
