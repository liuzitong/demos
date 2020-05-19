#ifndef QXPACK_IC_TIMEDOBJCACHE_CXX
#define QXPACK_IC_tiMEDOBJCACHE_CXX

#include "qxpack_ic_timedobjcache.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include <QTimer>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcTimedObjCachePriv*, o )
class QXPACK_IC_HIDDEN  IcTimedObjCachePriv {
private:
    IcObjectCache  m_obj_cache_ts;  // the Object Cache is thread-safe by design
    QTimer         m_cls_tmr;
    int            m_rt_ms;
public :
    explicit IcTimedObjCachePriv( int rt_ms, IcObjectCache::Callback cb, void *cb_obj, int max_num );
    ~IcTimedObjCachePriv( );
    inline IcObjectCache &  objCache_ts( ) { return m_obj_cache_ts; }
};

// ============================================================================
// ctor
// ============================================================================
IcTimedObjCachePriv :: IcTimedObjCachePriv (
    int rt_ms, IcObjectCache::Callback cb, void *cb_obj, int max_num
) : m_obj_cache_ts( cb, cb_obj, max_num )
{
    m_rt_ms = ( rt_ms < 16 ? 16 : rt_ms );
    m_cls_tmr.setInterval  ( m_rt_ms );
    m_cls_tmr.setSingleShot( false );

    QObject::connect (
        & m_cls_tmr, & QTimer::timeout,
        [this]( ) {
            m_obj_cache_ts.freeOldest( m_rt_ms );
        }
    );

    m_cls_tmr.start();
}

// ============================================================================
// dtor
// ============================================================================
IcTimedObjCachePriv :: ~IcTimedObjCachePriv ( )
{
    QObject::disconnect( & m_cls_tmr, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR );
    m_cls_tmr.stop();
}




// ////////////////////////////////////////////////////////////////////////////
//  wrapper API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcTimedObjCache :: IcTimedObjCache (
    IcObjectCache::Callback cb, void *cb_obj, int max_num, int rt_ms, QObject *pa
 ) : QObject( pa )
{
    m_obj = qxpack_ic_new( IcTimedObjCachePriv, rt_ms, cb, cb_obj, max_num );
}

// ============================================================================
// dtor
// ============================================================================
IcTimedObjCache :: ~IcTimedObjCache ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcTimedObjCachePriv );
    }
}

// ============================================================================
// allocate a object
// ============================================================================
void *  IcTimedObjCache :: allocate ( )
{
    return T_PrivPtr( m_obj )->objCache_ts().allocate();
}

// ============================================================================
// recycle the object
// ============================================================================
void    IcTimedObjCache :: recycle ( void *obj )
{
    T_PrivPtr( m_obj )->objCache_ts().recycle( obj );
}

}

#endif
