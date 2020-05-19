#ifndef QMLBASE_MEMCNTR_HXX
#define QMLBASE_MEMCNTR_HXX
 
#include "qmlbase_def.h"

#ifdef QMLBASE_QT5_ENABLED
#include <QObject>
#endif


namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLBASE_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlbase_new() / qmlbase_delete() to manage the new / delete normal object
//
// 3) use qmlbase_new_qobj() / qmlbase_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API MemCntr {
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
#if defined( QMLBASE_QT5_ENABLED )
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

#if defined( QMLBASE_CFG_MEM_TRACE )

#define  qmlbase_new( t, ... )       ( qmlbase::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlbase::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlbase_delete( o, t )      do{ qmlbase::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlbase::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlbase_incr_new_cntr( t )  qmlbase::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlbase_decr_new_cntr( t )  qmlbase::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlbase_alloc( sz )         ( qmlbase::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlbase::MemCntr::_assertNonNullPtr( qmlbase::MemCntr::allocMemory( sz ), true ))
#define  qmlbase_realloc( p, sz )    ( qmlbase::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlbase::MemCntr::_assertNonNullPtr( qmlbase::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlbase_free( p )           do{ qmlbase::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlbase::MemCntr::freeMemory( qmlbase::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlbase_new( t, ... )       ( qmlbase::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlbase::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlbase_delete( o, t )      do{ qmlbase::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlbase::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlbase_incr_new_cntr( t )  qmlbase::MemCntr::incrNewCntr(1)
#define  qmlbase_decr_new_cntr( t )  qmlbase::MemCntr::decrNewCntr(1)

#define  qmlbase_alloc( sz )         ( qmlbase::MemCntr::incrNewCntr(1), qmlbase::MemCntr::_assertNonNullPtr( qmlbase::MemCntr::allocMemory( sz ), true ))
#define  qmlbase_realloc( p, sz )    ( qmlbase::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlbase::MemCntr::_assertNonNullPtr( qmlbase::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlbase_free( p )           do{ qmlbase::MemCntr::decrNewCntr(1); qmlbase::MemCntr::freeMemory( qmlbase::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlbase_curr_new_cntr           ( qmlbase::MemCntr::currNewCntr())
#define  qmlbase_enable_mem_trace( sw )  qmlbase::MemCntr::enableMemTrace( sw )
#define  qmlbase_enable_mem_cntr( sw )   qmlbase::MemCntr::enableMemCntr( sw )



#ifdef QMLBASE_QT5_ENABLED
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
 *     manual free it use qmlbase_delete_qobj( o )
 */
// ============================================================================
#define qmlbase_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlbase::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlbase_delete_qobj( o ) \
    do{ qmlbase::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlbase_delete_qobj_later( o ) \
    do{ qmlbase::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
