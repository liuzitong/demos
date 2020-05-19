#ifndef LIBANDQMLTESTDEMO_MEMCNTR_HXX
#define LIBANDQMLTESTDEMO_MEMCNTR_HXX
 
#include "libandqmltestdemo_def.h"

#ifdef LIBANDQMLTESTDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace libandqmltestdemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the LIBANDQMLTESTDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use libandqmltestdemo_new() / libandqmltestdemo_delete() to manage the new / delete normal object
//
// 3) use libandqmltestdemo_new_qobj() / libandqmltestdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class LIBANDQMLTESTDEMO_API MemCntr {
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
#if defined( LIBANDQMLTESTDEMO_QT5_ENABLED )
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

#if defined( LIBANDQMLTESTDEMO_CFG_MEM_TRACE )

#define  libandqmltestdemo_new( t, ... )       ( libandqmltestdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( libandqmltestdemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  libandqmltestdemo_delete( o, t )      do{ libandqmltestdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( libandqmltestdemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  libandqmltestdemo_incr_new_cntr( t )  libandqmltestdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  libandqmltestdemo_decr_new_cntr( t )  libandqmltestdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  libandqmltestdemo_alloc( sz )         ( libandqmltestdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), libandqmltestdemo::MemCntr::_assertNonNullPtr( libandqmltestdemo::MemCntr::allocMemory( sz ), true ))
#define  libandqmltestdemo_realloc( p, sz )    ( libandqmltestdemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), libandqmltestdemo::MemCntr::_assertNonNullPtr( libandqmltestdemo::MemCntr::reallocMemory( p, sz ), true ))
#define  libandqmltestdemo_free( p )           do{ libandqmltestdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); libandqmltestdemo::MemCntr::freeMemory( libandqmltestdemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  libandqmltestdemo_new( t, ... )       ( libandqmltestdemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( libandqmltestdemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  libandqmltestdemo_delete( o, t )      do{ libandqmltestdemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( libandqmltestdemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  libandqmltestdemo_incr_new_cntr( t )  libandqmltestdemo::MemCntr::incrNewCntr(1)
#define  libandqmltestdemo_decr_new_cntr( t )  libandqmltestdemo::MemCntr::decrNewCntr(1)

#define  libandqmltestdemo_alloc( sz )         ( libandqmltestdemo::MemCntr::incrNewCntr(1), libandqmltestdemo::MemCntr::_assertNonNullPtr( libandqmltestdemo::MemCntr::allocMemory( sz ), true ))
#define  libandqmltestdemo_realloc( p, sz )    ( libandqmltestdemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), libandqmltestdemo::MemCntr::_assertNonNullPtr( libandqmltestdemo::MemCntr::reallocMemory( p, sz ), true ))
#define  libandqmltestdemo_free( p )           do{ libandqmltestdemo::MemCntr::decrNewCntr(1); libandqmltestdemo::MemCntr::freeMemory( libandqmltestdemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  libandqmltestdemo_curr_new_cntr           ( libandqmltestdemo::MemCntr::currNewCntr())
#define  libandqmltestdemo_enable_mem_trace( sw )  libandqmltestdemo::MemCntr::enableMemTrace( sw )
#define  libandqmltestdemo_enable_mem_cntr( sw )   libandqmltestdemo::MemCntr::enableMemCntr( sw )



#ifdef LIBANDQMLTESTDEMO_QT5_ENABLED
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
 *     manual free it use libandqmltestdemo_delete_qobj( o )
 */
// ============================================================================
#define libandqmltestdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( libandqmltestdemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define libandqmltestdemo_delete_qobj( o ) \
    do{ libandqmltestdemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define libandqmltestdemo_delete_qobj_later( o ) \
    do{ libandqmltestdemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
