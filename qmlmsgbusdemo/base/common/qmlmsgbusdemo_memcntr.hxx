#ifndef QMLMSGBUSDEMO_MEMCNTR_HXX
#define QMLMSGBUSDEMO_MEMCNTR_HXX
 
#include "qmlmsgbusdemo_def.h"

#ifdef QMLMSGBUSDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLMSGBUSDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlmsgbusdemo_new() / qmlmsgbusdemo_delete() to manage the new / delete normal object
//
// 3) use qmlmsgbusdemo_new_qobj() / qmlmsgbusdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_API MemCntr {
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
#if defined( QMLMSGBUSDEMO_QT5_ENABLED )
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

#if defined( QMLMSGBUSDEMO_CFG_MEM_TRACE )

#define  qmlmsgbusdemo_new( t, ... )       ( qmlMsgbusDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlmsgbusdemo_delete( o, t )      do{ qmlMsgbusDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlmsgbusdemo_incr_new_cntr( t )  qmlMsgbusDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlmsgbusdemo_decr_new_cntr( t )  qmlMsgbusDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlmsgbusdemo_alloc( sz )         ( qmlMsgbusDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlMsgbusDemo::MemCntr::_assertNonNullPtr( qmlMsgbusDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlmsgbusdemo_realloc( p, sz )    ( qmlMsgbusDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlMsgbusDemo::MemCntr::_assertNonNullPtr( qmlMsgbusDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlmsgbusdemo_free( p )           do{ qmlMsgbusDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlMsgbusDemo::MemCntr::freeMemory( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlmsgbusdemo_new( t, ... )       ( qmlMsgbusDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlmsgbusdemo_delete( o, t )      do{ qmlMsgbusDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlmsgbusdemo_incr_new_cntr( t )  qmlMsgbusDemo::MemCntr::incrNewCntr(1)
#define  qmlmsgbusdemo_decr_new_cntr( t )  qmlMsgbusDemo::MemCntr::decrNewCntr(1)

#define  qmlmsgbusdemo_alloc( sz )         ( qmlMsgbusDemo::MemCntr::incrNewCntr(1), qmlMsgbusDemo::MemCntr::_assertNonNullPtr( qmlMsgbusDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlmsgbusdemo_realloc( p, sz )    ( qmlMsgbusDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlMsgbusDemo::MemCntr::_assertNonNullPtr( qmlMsgbusDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlmsgbusdemo_free( p )           do{ qmlMsgbusDemo::MemCntr::decrNewCntr(1); qmlMsgbusDemo::MemCntr::freeMemory( qmlMsgbusDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlmsgbusdemo_curr_new_cntr           ( qmlMsgbusDemo::MemCntr::currNewCntr())
#define  qmlmsgbusdemo_enable_mem_trace( sw )  qmlMsgbusDemo::MemCntr::enableMemTrace( sw )
#define  qmlmsgbusdemo_enable_mem_cntr( sw )   qmlMsgbusDemo::MemCntr::enableMemCntr( sw )



#ifdef QMLMSGBUSDEMO_QT5_ENABLED
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
 *     manual free it use qmlmsgbusdemo_delete_qobj( o )
 */
// ============================================================================
#define qmlmsgbusdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlMsgbusDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlmsgbusdemo_delete_qobj( o ) \
    do{ qmlMsgbusDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlmsgbusdemo_delete_qobj_later( o ) \
    do{ qmlMsgbusDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
