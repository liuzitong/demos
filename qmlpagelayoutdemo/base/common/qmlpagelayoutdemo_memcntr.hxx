#ifndef QMLPAGELAYOUTDEMO_MEMCNTR_HXX
#define QMLPAGELAYOUTDEMO_MEMCNTR_HXX
 
#include "qmlpagelayoutdemo_def.h"

#ifdef QMLPAGELAYOUTDEMO_QT5_ENABLED
#include <QObject>
#endif


namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// memory counter
//
// Tips:
// 1) enable memory trace.
//    define the QMLPAGELAYOUTDEMO_CFG_MEM_TRACE
//    call  MemCntr::enableMemTrace( ) to enable or disable memory trace
//
// 2) use qmlpagelayoutdemo_new() / qmlpagelayoutdemo_delete() to manage the new / delete normal object
//
// 3) use qmlpagelayoutdemo_new_qobj() / qmlpagelayoutdemo_delete_qobj() to manage qobject
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API MemCntr {
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
#if defined( QMLPAGELAYOUTDEMO_QT5_ENABLED )
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

#if defined( QMLPAGELAYOUTDEMO_CFG_MEM_TRACE )

#define  qmlpagelayoutdemo_new( t, ... )       ( qmlPagelayoutDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ),  static_cast< t *>( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true )) )
#define  qmlpagelayoutdemo_delete( o, t )      do{ qmlPagelayoutDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t ); delete( static_cast< t *>( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( o, false )) ); }while(0)
#define  qmlpagelayoutdemo_incr_new_cntr( t )  qmlPagelayoutDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )
#define  qmlpagelayoutdemo_decr_new_cntr( t )  qmlPagelayoutDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__, #t )

#define  qmlpagelayoutdemo_alloc( sz )         ( qmlPagelayoutDemo::MemCntr::incrNewCntr(1,__FILE__,__FUNCTION__,__LINE__), qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( qmlPagelayoutDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlpagelayoutdemo_realloc( p, sz )    ( qmlPagelayoutDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 ),__FILE__,__FUNCTION__,__LINE__), qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( qmlPagelayoutDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlpagelayoutdemo_free( p )           do{ qmlPagelayoutDemo::MemCntr::decrNewCntr(1,__FILE__,__FUNCTION__,__LINE__); qmlPagelayoutDemo::MemCntr::freeMemory( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)


#else

#define  qmlpagelayoutdemo_new( t, ... )       ( qmlPagelayoutDemo::MemCntr::incrNewCntr(1 ), static_cast< t *>( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( new t ( __VA_ARGS__ ), true ) ))
#define  qmlpagelayoutdemo_delete( o, t )      do{ qmlPagelayoutDemo::MemCntr::decrNewCntr(1); delete( static_cast< t *>( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( o, false ) )); }while(0)
#define  qmlpagelayoutdemo_incr_new_cntr( t )  qmlPagelayoutDemo::MemCntr::incrNewCntr(1)
#define  qmlpagelayoutdemo_decr_new_cntr( t )  qmlPagelayoutDemo::MemCntr::decrNewCntr(1)

#define  qmlpagelayoutdemo_alloc( sz )         ( qmlPagelayoutDemo::MemCntr::incrNewCntr(1), qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( qmlPagelayoutDemo::MemCntr::allocMemory( sz ), true ))
#define  qmlpagelayoutdemo_realloc( p, sz )    ( qmlPagelayoutDemo::MemCntr::incrNewCntr(( p != nullptr ? 0 : 1 )), qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( qmlPagelayoutDemo::MemCntr::reallocMemory( p, sz ), true ))
#define  qmlpagelayoutdemo_free( p )           do{ qmlPagelayoutDemo::MemCntr::decrNewCntr(1); qmlPagelayoutDemo::MemCntr::freeMemory( qmlPagelayoutDemo::MemCntr::_assertNonNullPtr( p, false )); }while(0)

#endif

#define  qmlpagelayoutdemo_curr_new_cntr           ( qmlPagelayoutDemo::MemCntr::currNewCntr())
#define  qmlpagelayoutdemo_enable_mem_trace( sw )  qmlPagelayoutDemo::MemCntr::enableMemTrace( sw )
#define  qmlpagelayoutdemo_enable_mem_cntr( sw )   qmlPagelayoutDemo::MemCntr::enableMemCntr( sw )



#ifdef QMLPAGELAYOUTDEMO_QT5_ENABLED
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
 *     manual free it use qmlpagelayoutdemo_delete_qobj( o )
 */
// ============================================================================
#define qmlpagelayoutdemo_new_qobj( t, ... ) \
    ( \
     [=]( t* _t, const char *fn, const char *func, int ln, const char *type )-> t * { \
         return qobject_cast< t *>( qmlPagelayoutDemo::MemCntr::memCntOnQObj( _t, fn, func, ln, type )); \
     } \
    )( new t ( __VA_ARGS__ ), __FILE__, __FUNCTION__, __LINE__, #t )



#define qmlpagelayoutdemo_delete_qobj( o ) \
    do{ qmlPagelayoutDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), false ); } while(0)

#define qmlpagelayoutdemo_delete_qobj_later( o ) \
    do{ qmlPagelayoutDemo::MemCntr::memCntOnDelObj( qobject_cast<QObject*>(o), true ); } while(0)

#endif




#endif
