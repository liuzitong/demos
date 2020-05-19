#ifndef QMLUSERLISTMODELDEMO_MEMCNTR_HXX
#define QMLUSERLISTMODELDEMO_MEMCNTR_HXX
 
#include "qmluserlistmodeldemo_def.h"

#ifdef QMLUSERLISTMODELDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLUSERLISTMODELDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmluserlistmodeldemo_new() / qmluserlistmodeldemo_delete() to manage the new / delete normal object
//
// 3) use qmluserlistmodeldemo_new_qobj() / qmluserlistmodeldemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API MemCntr {
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
#if defined( QMLUSERLISTMODELDEMO_QT5_ENABLED )
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

#if defined( QMLUSERLISTMODELDEMO_CFG_MEM_TRACE )

#define  qmluserlistmodeldemo_new( t, ... )       ( qmlUserListmodelDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmluserlistmodeldemo_delete( o, t )      do{ qmlUserListmodelDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmluserlistmodeldemo_incr_new_cntr( t )  qmlUserListmodelDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmluserlistmodeldemo_decr_new_cntr( t )  qmlUserListmodelDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmluserlistmodeldemo_alloc( sz )         ( qmlUserListmodelDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( qmlUserListmodelDemo::MemCntr::allocMemory( sz ), true ))
#define  qmluserlistmodeldemo_realloc( p, sz )    ( qmlUserListmodelDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( qmlUserListmodelDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmluserlistmodeldemo_free( p )           do{ qmlUserListmodelDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlUserListmodelDemo::MemCntr::freeMemory( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmluserlistmodeldemo_new( t, ... )       ( qmlUserListmodelDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmluserlistmodeldemo_delete( o, t )      do{ qmlUserListmodelDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmluserlistmodeldemo_incr_new_cntr( t )  qmlUserListmodelDemo::MemCntr::incrNewCntr(1)
#define  qmluserlistmodeldemo_decr_new_cntr( t )  qmlUserListmodelDemo::MemCntr::decrNewCntr(1)

#define  qmluserlistmodeldemo_alloc( sz )         ( qmlUserListmodelDemo::MemCntr::incrNewCntr(1), qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( qmlUserListmodelDemo::MemCntr::allocMemory( sz ), true ))
#define  qmluserlistmodeldemo_realloc( p, sz )    ( qmlUserListmodelDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( qmlUserListmodelDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmluserlistmodeldemo_free( p )           do{ qmlUserListmodelDemo::MemCntr::decrNewCntr(1); qmlUserListmodelDemo::MemCntr::freeMemory( qmlUserListmodelDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmluserlistmodeldemo_curr_new_cntr           ( qmlUserListmodelDemo::MemCntr::currNewCntr())
#define  qmluserlistmodeldemo_enable_mem_trace( sw )  qmlUserListmodelDemo::MemCntr::enableMemTrace( sw )
#define  qmluserlistmodeldemo_enable_mem_cntr( sw )   qmlUserListmodelDemo::MemCntr::enableMemCntr( sw )



#ifdef QMLUSERLISTMODELDEMO_QT5_ENABLED
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
 *     manual free it use qmluserlistmodeldemo_delete_qobj( o )
 */
// ============================================================================
#define qmluserlistmodeldemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlUserListmodelDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmluserlistmodeldemo_delete_qobj( o ) \
    do{ qmlUserListmodelDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmluserlistmodeldemo_delete_qobj_later( o ) \
    do{ qmlUserListmodelDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
