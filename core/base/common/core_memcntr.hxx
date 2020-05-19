#ifndef CORE_MEMCNTR_HXX
#define CORE_MEMCNTR_HXX
 
#include "core_def.h"

#ifdef CORE_QT5_ENABLED
#include <QObject>
#endif


namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the CORE_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use core_new() / core_delete() to manage the new / delete normal object
//
// 3) use core_new_qobj() / core_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API MemCntr {
public:
    static int    currNewCntr( );
    static void*  _assertNonNullPtr( void* p, bool is_alloc ); // nw: 20180807 added
    static int    incrNewCntr( int cntr, const char *file = nullptr, const char *func = nullptr, int ln = 0, const char *type = nullptr );
    static int    decrNewCntr( int cntr, const char *file = nullptr, const char *func = nullptr, int ln = 0, const char *type = nullptr );
    static void   saveTraceInfoToFile( const char *fn );
    static void   enableMemTrace( bool sw );

    // nw: 2018/11 added
    static void*  allocMemory  ( size_t size );
    static void*  reallocMemory( void *old, size_t size );
    static void   freeMemory ( void *p );

    // nw: 2019/05 added
    static void   enableMemCntr( bool sw ); // default is true

    // nw: 2019/05/23 added for QT5
#if defined( CORE_QT5_ENABLED )
    static QObject*  memCntOnQObj  ( QObject*, const char *file, const char *func, int ln, const char *type );
    static void      memCntOnDelObj( QObject *o, bool is_later = false );
#endif

protected:
    MemCntr ( );
    virtual ~MemCntr( );
private:
    void *m_obj;
};

}

#if defined( CORE_CFG_MEM_TRACE )

#define  core_new( t, ... )       ( core::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( core::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  core_delete( o, t )      do{ core::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( core::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  core_incr_new_cntr( t )  core::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  core_decr_new_cntr( t )  core::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  core_alloc( sz )         ( core::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), core::MemCntr::_assertNonNullPtr( core::MemCntr::allocMemory( sz ), true ))
#define  core_realloc( p, sz )    ( core::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), core::MemCntr::_assertNonNullPtr( core::MemCntr::reallocMemory( p, sz ), true ))
#define  core_free( p )           do{ core::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); core::MemCntr::freeMemory( core::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  core_new( t, ... )       ( core::MemCntr::incrNewCntr(1 ), static_cast< t *>( core::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  core_delete( o, t )      do{ core::MemCntr::decrNewCntr(1); delete( static_cast< t *>( core::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  core_incr_new_cntr( t )  core::MemCntr::incrNewCntr(1)
#define  core_decr_new_cntr( t )  core::MemCntr::decrNewCntr(1)

#define  core_alloc( sz )         ( core::MemCntr::incrNewCntr(1), core::MemCntr::_assertNonNullPtr( core::MemCntr::allocMemory( sz ), true ))
#define  core_realloc( p, sz )    ( core::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), core::MemCntr::_assertNonNullPtr( core::MemCntr::reallocMemory( p, sz ), true ))
#define  core_free( p )           do{ core::MemCntr::decrNewCntr(1); core::MemCntr::freeMemory( core::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  core_curr_new_cntr           ( core::MemCntr::currNewCntr())
#define  core_enable_mem_trace( sw )  core::MemCntr::enableMemTrace( sw )
#define  core_enable_mem_cntr( sw )   core::MemCntr::enableMemCntr( sw )



#ifdef CORE_QT5_ENABLED
// ////////////////////////////////////////////////////////////////////////////
//
//             added for QT5
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
/*!
 * @brief memory counted new QObject
 * @details user can use this macro to do count on classes inherit QObject
 * @warning This mechanism used QObject::destroyed signal, do not do disconnect like \n
 *     obj::disconnect() or QObject::disconnect( obj, 0,0,0 ) \n
 *     that will discard signals. If possible, use obj::blockSignals(true) to instead
 *     block emit signals.
 * @note  For third library object, maybe inner discard all signals, so user can \n
 *     manual free it use core_delete_qobj( o )
 */
// ============================================================================
#define core_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( core::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define core_delete_qobj( o ) \
    do{ core::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define core_delete_qobj_later( o ) \
    do{ core::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
