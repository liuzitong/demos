#ifndef QMLCONFIRMBUSDEMO_MEMCNTR_HXX
#define QMLCONFIRMBUSDEMO_MEMCNTR_HXX
 
#include "qmlconfirmbusdemo_def.h"

#ifdef QMLCONFIRMBUSDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLCONFIRMBUSDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlconfirmbusdemo_new() / qmlconfirmbusdemo_delete() to manage the new / delete normal object
//
// 3) use qmlconfirmbusdemo_new_qobj() / qmlconfirmbusdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCONFIRMBUSDEMO_API MemCntr {
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
#if defined( QMLCONFIRMBUSDEMO_QT5_ENABLED )
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

#if defined( QMLCONFIRMBUSDEMO_CFG_MEM_TRACE )

#define  qmlconfirmbusdemo_new( t, ... )       ( qmlconfirmbusdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlconfirmbusdemo_delete( o, t )      do{ qmlconfirmbusdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlconfirmbusdemo_incr_new_cntr( t )  qmlconfirmbusdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlconfirmbusdemo_decr_new_cntr( t )  qmlconfirmbusdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlconfirmbusdemo_alloc( sz )         ( qmlconfirmbusdemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( qmlconfirmbusdemo::MemCntr::allocMemory( sz ), true ))
#define  qmlconfirmbusdemo_realloc( p, sz )    ( qmlconfirmbusdemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( qmlconfirmbusdemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlconfirmbusdemo_free( p )           do{ qmlconfirmbusdemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlconfirmbusdemo::MemCntr::freeMemory( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlconfirmbusdemo_new( t, ... )       ( qmlconfirmbusdemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlconfirmbusdemo_delete( o, t )      do{ qmlconfirmbusdemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlconfirmbusdemo_incr_new_cntr( t )  qmlconfirmbusdemo::MemCntr::incrNewCntr(1)
#define  qmlconfirmbusdemo_decr_new_cntr( t )  qmlconfirmbusdemo::MemCntr::decrNewCntr(1)

#define  qmlconfirmbusdemo_alloc( sz )         ( qmlconfirmbusdemo::MemCntr::incrNewCntr(1), qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( qmlconfirmbusdemo::MemCntr::allocMemory( sz ), true ))
#define  qmlconfirmbusdemo_realloc( p, sz )    ( qmlconfirmbusdemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( qmlconfirmbusdemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlconfirmbusdemo_free( p )           do{ qmlconfirmbusdemo::MemCntr::decrNewCntr(1); qmlconfirmbusdemo::MemCntr::freeMemory( qmlconfirmbusdemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlconfirmbusdemo_curr_new_cntr           ( qmlconfirmbusdemo::MemCntr::currNewCntr())
#define  qmlconfirmbusdemo_enable_mem_trace( sw )  qmlconfirmbusdemo::MemCntr::enableMemTrace( sw )
#define  qmlconfirmbusdemo_enable_mem_cntr( sw )   qmlconfirmbusdemo::MemCntr::enableMemCntr( sw )



#ifdef QMLCONFIRMBUSDEMO_QT5_ENABLED
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
 *     manual free it use qmlconfirmbusdemo_delete_qobj( o )
 */
// ============================================================================
#define qmlconfirmbusdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlconfirmbusdemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlconfirmbusdemo_delete_qobj( o ) \
    do{ qmlconfirmbusdemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlconfirmbusdemo_delete_qobj_later( o ) \
    do{ qmlconfirmbusdemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
