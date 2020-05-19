#ifndef  QXPACK_IC_SYSSHM_WIN32_CXX
#define  QXPACK_IC_SYSSHM_WIN32_CXX

#include "qxpack_ic_sysshm.hxx"
#include <windows.h>
#include <cstring>
#include <new>
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"

namespace QxPack {

// /////////////////////////////////////////////////////////////////////
//  local [ shm object ]
// /////////////////////////////////////////////////////////////////////
#define T_SysShm( o )  T_ObjCast( IcSysShmObj*, o )
class QXPACK_IC_HIDDEN IcSysShmObj {
private:
    intptr_t  m_locker;
    intptr_t  m_data_size; 
    intptr_t  m_data[2];
public:
    IcSysShmObj ( int data_size ) { m_data_size = data_size; }
    ~IcSysShmObj( ) {  }
    inline int    size( ) const { return T_PtrCast( int, m_data_size ); }
    inline void*  data( )       { return m_data + 2; }
};

// /////////////////////////////////////////////////////////////////////
//  local [ memory object ]
// /////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcSysShmPriv*, o )
class QXPACK_IC_HIDDEN IcSysShmPriv {
private:
    HANDLE       m_shm;
    IcSysShmObj *m_shm_ptr;
    bool         m_is_creator;
public:
    IcSysShmPriv ( int size, const char *name );
    IcSysShmPriv ( const char *name );
    ~IcSysShmPriv (  );
    inline  bool   isWorked(  ) const { return ( m_shm_ptr != NULL ? true : false ); }   
    inline  void*  data (  )          { return ( m_shm_ptr != NULL ? m_shm_ptr->data( ) : nullptr ); }
    inline  int    size (  )          { return ( m_shm_ptr != NULL ? m_shm_ptr->size( ) : nullptr ); }
    inline  bool   isCreator( ) const { return m_is_creator; }
};

// =====================================================
// CTOR -- create 
// =====================================================
        IcSysShmPriv :: IcSysShmPriv ( int data_size, const char *share_name )
{
    // -----------------------------------------
    // 0) fixed the allocated size
    // -----------------------------------------
    data_size = ( data_size + 63 ) / 64 * 64;
    int size = data_size + sizeof( IcSysShmObj );
    if ( size < 64 ) { size = 64; }

    // -----------------------------------------
    // 1) create the share memory
    // -----------------------------------------
    WCHAR  w_tmp[ 256 ];
    WCHAR *w_share_name = & w_tmp[0];
    int a_count = std::strlen( share_name );
    int w_count = ::MultiByteToWideChar( CP_ACP, 0, share_name, a_count, NULL, 0 ) + 1;
    if ( w_count > 250 ) { w_share_name = T_PtrCast( wchar_t *, std::malloc( w_count * 2 )); }
    ::MultiByteToWideChar( CP_ACP, 0, share_name, a_count, w_share_name, w_count );
    w_share_name[ w_count - 1 ] = 0;

    // NOTE: if the shared memory is existed, create file also return
    //       the handle, but the GetLastError( ) will return ERROR_ALREADY_EXISTS
    // WARNING: we should use a atomic string for shared memory OR we need to check
    //       the shared data structure to ensure the data !
    HANDLE  hMapFile = ::CreateFileMappingW( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, ( DWORD )( size ),  w_share_name );
    bool    is_already_exist = ( GetLastError( ) == ERROR_ALREADY_EXISTS ? true : false );

    // maybe here need to add some code to ensure the data.
    if ( w_share_name != & w_tmp[0]) { std::free( w_share_name ); }

    // ---------------------------------------
    // 2) try map the region into process
    // ---------------------------------------
    if ( hMapFile != NULL ) {
        m_shm_ptr = ( IpcShmObj *) ::MapViewOfFile( hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size );
        if ( m_shm_ptr == NULL ) { ::CloseHandle( hMapFile ); hMapFile = NULL; }
    }

    // --------------------------------------
    // 3) assign variable
    // --------------------------------------
    if ( hMapFile != NULL ) {
        m_shm = hMapFile;
        if ( ! is_already_exist ) {
            new( m_shm_ptr ) IcSysShmObj( data_size ); // call the CTOR
        }
    } else {
        m_shm = NULL; m_shm_ptr = NULL;
    }
    m_is_creator = true;
}

// =====================================================
// CTOR -- open
// =====================================================
        IcSysShmPriv :: IcSysShmPriv ( const char *share_name )
{
    //0) try open the share memory
    WCHAR  w_tmp[ 256 ]; // for Windows, wchar_t is always 2 bytes
    WCHAR *w_share_name = & w_tmp[0];
    int a_count = strlen( share_name );
    int w_count = ::MultiByteToWideChar( CP_ACP, 0, share_name, a_count, NULL, 0 ) + 1;
    if ( w_count > 250 ) { w_share_name = T_PtrCast( wchar_t *, malloc( w_count * 2 )); }
    ::MultiByteToWideChar( CP_ACP, 0, share_name, a_count, w_share_name, w_count );
    w_share_name[ w_count - 1 ] = 0;

    HANDLE  hMapFile = ::OpenFileMappingW( FILE_MAP_ALL_ACCESS, FALSE, w_share_name );

    if ( w_share_name != & w_tmp[0]) { std::free( w_share_name ); }

    if ( hMapFile != NULL ) {
        m_shm_ptr = ( IpcShmObj *) ::MapViewOfFile( hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
        if ( m_shm_ptr == NULL ) { ::CloseHandle( hMapFile ); hMapFile = NULL; }
    }

    //1) assign variable
    if ( hMapFile != NULL ) {
        m_shm = hMapFile;
    } else {
        m_shm_ptr = NULL; m_shm = NULL;
    }
    
    m_is_creator = false;
}

// =====================================================================
// DTOR
// =====================================================================
        IcSysShmPriv :: ~IcSysShmPriv ( )
{
    if ( m_shm_ptr != NULL ) {
        ::UnmapViewOfFile ( m_shm_ptr ); m_shm_ptr = NULL;
    }     
    if ( m_shm != NULL ) {
        ::CloseHandle( m_shm ); m_shm = NULL;
    } 
}


// /////////////////////////////////////////////////////////////////////
//  exports
// /////////////////////////////////////////////////////////////////////
// =====================================================================
//!  CTOR
// =====================================================================
        IcSysShm :: IcSysShm ( int size, const char *name )
{
    m_obj = nullptr;
    if ( name != nullptr ) {
        this->init ( size, name );
    }
}

        IcSysShm :: IcSysShm ( const char *name )
{
    m_obj = nullptr;
    if ( name != nullptr ) {
        this->init ( name );
    }
}

        IcSysShm :: IcSysShm (  )
{
    m_obj = nullptr;
}

// =====================================================================
//!  DTOR
// =====================================================================
        IcSysShm :: ~IcSysShm (  )
{
    this->deinit( );
}

// =====================================================================
//!  return if it is worked or not
// =====================================================================
bool    IcSysShm :: isWorked( ) const
{  return ( m_obj != nullptr ? T_PrivPtr( m_obj )->isWorked( ) : false ); }

// =====================================================================
//!  return the memory size if it is worked. the size maybe greater than \n
//   the user create size
// =====================================================================
int     IcSysShm :: size( ) const
{   return ( m_obj != nullptr ? T_PrivPtr( m_obj )->size( ) : 0 ); }

// =====================================================================
//!  return the memory pointer in this process
// =====================================================================
void*   IcSysShm :: data( )
{   return ( m_obj != nullptr ? T_PrivPtr( m_obj )->data( ) : nullptr ); }

// =====================================================================
//!  create the shared memory
/*!
  @param size  [ in ]  the required memory size 
  @param name  [ in ]  the shared name.
*/
// =====================================================================
bool    IcSysShm :: init ( int size, const char *name )
{
    this->deinit( );
    m_obj = qxpack_ic_new( IpcSharedMemPriv, size, name );
    if ( T_PrivPtr( m_obj )->isWorked() ) {
        return true;
    } else {
        this->deinit();
        return false;
    }
}

// =====================================================================
//!  open the shared memory
// =====================================================================
bool    IcSysShm :: init ( const char *name )
{
    this->deinit( );
    m_obj = qxpack_ic_new( IpcSharedMemPriv, name );
    if ( T_PrivPtr( m_obj )->isWorked() ) {
        return true;
    } else {
        this->deinit();
        return false;
    }
}

// =====================================================================
//!  deinit the share memory
// =====================================================================
void    IcSysShm :: deinit( )
{
    if ( m_obj != nullptr ) {
        qxpack_ic_delete( obj, IcSysShmPriv );
        m_obj = nullptr;
    }
}


}

#endif
