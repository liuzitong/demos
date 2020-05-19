#ifndef QXPACK_QCGUARD_CXX
#define QXPACK_QCGUARD_CXX

#include "qxpack_ic_qcguard.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_objectcache.hxx"
#include "qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx"

#include <QMutex>
#include <QSemaphore>
#include <QObject>
#include <QAtomicInt>
#include <QAtomicPointer>

namespace QxPack {

// //////////////////////////////////////////////////////////
// register in meta system
// //////////////////////////////////////////////////////////
static bool  g_is_reg = false;
static void  gRegInMeta( )
{
    if ( ! g_is_reg ) {
        qRegisterMetaType<QxPack::IcQcGuard>("QxPack::IcQcGuard&");
        qRegisterMetaType<QxPack::IcQcGuard>("QxPack::IcQcGuard");
        g_is_reg = true;
    }
}


// ///////////////////////////////////////////////////////////
//
//              QcGuard inner item
// NOTE: because the object maybe delete atexit(), so here do
// not use any signal or slot, and only use the basic objects.
//
// //////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcQcGuard_Item *, o )
class QXPACK_IC_HIDDEN IcQcGuard_Item {
private:
    enum Stage{ Stage_WaitForHandle, Stage_Working, Stage_Done, Stage_Error, Stage_Dropped };
    QSemaphore  m_acc_sem;
    QMutex      m_locker;
    Stage       m_stage;
    QAtomicInt  m_ref_cntr;
protected:
    bool    waitByTime ( QSemaphore &sem, int ms );
    inline void resetSem ( QSemaphore &sem ) { while( sem.tryAcquire(1)){} }
public:
    IcQcGuard_Item ( ) { m_ref_cntr.store(0); m_stage = Stage_WaitForHandle; }
    ~IcQcGuard_Item( ) { }
    inline int  addRef ( )    { return m_ref_cntr.fetchAndAddOrdered( 1 ) + 1; }
    inline int  releaseRef( ) { return m_ref_cntr.fetchAndSubOrdered( 1 ) - 1; }

    void  reset ( ); // call it before use the guard
    bool  waitForDone( int acc_ms );
    bool  beginWork  ( );
    void  endWork    ( bool is_done = true );
};

// ============================================================
// reset all variables
// ============================================================
void   IcQcGuard_Item :: reset( )
{
    this->resetSem( m_acc_sem );
    m_stage = Stage_WaitForHandle;
}

// ============================================================
// wait by time
// ============================================================
bool   IcQcGuard_Item :: waitByTime ( QSemaphore &sem, int ms )
{
    bool ret = true;
    if ( ms < 0 ) {
        sem.acquire(1);
    } else if ( ms == 0 ) {
        ret = sem.tryAcquire(1);
    } else {
        ret = sem.tryAcquire(1,ms);
    }
    return ret;
}

// ============================================================
// wait for operation down
// ============================================================
bool  IcQcGuard_Item :: waitForDone ( int acc_ms )
{
    bool ret = this->waitByTime( m_acc_sem, acc_ms );
    if ( ret ) {
        m_locker.lock();
        if ( m_stage != Stage_Error ) { ret = false; } // some error..
        m_locker.unlock();
    } else {
        m_locker.lock();
        switch( m_stage ) {
        case Stage_WaitForHandle : m_stage = Stage_Dropped; break;
        case Stage_Working       : break; // impossible, because m_locker is locked while working.
        case Stage_Done          : ret = true;  break;
        case Stage_Error         : ret = false; break;
        default: m_stage = Stage_Dropped; break;
        }
        m_locker.unlock();
    }

    return ret;
}

// ============================================================
// begin work ( called by target function )
// ============================================================
bool   IcQcGuard_Item :: beginWork ( )
{
    bool ret = false;

    m_locker.lock();
    if ( m_stage == Stage_Dropped ) {
        m_locker.unlock(); // the caller is dropped this call.
    } else {
        m_stage = Stage_Working; // start working.
        m_acc_sem.release(1);
        ret = true;
    }

    return ret;
}

// ==========================================================
// end of work ( called by target function )
// ==========================================================
void   IcQcGuard_Item :: endWork( bool is_done )
{
    m_stage = ( is_done ? Stage_Done : Stage_Error );
    m_locker.unlock();
}

// /////////////////////////////////////////////////////////
//
// GcGuard_Item cache manager
//
// /////////////////////////////////////////////////////////
static QMutex         g_locker;
static QAtomicPointer<IcObjectCache> g_obj_cache( Q_NULLPTR );

// ========================================================
// should be called in dync.deinit stage in C++
// ========================================================
static void gCleanUp ( )
{
    IcObjectCache *cache = g_obj_cache.loadAcquire();
    if ( cache != Q_NULLPTR ) {
        qxpack_ic_delete( cache, IcObjectCache );
        g_obj_cache.store(Q_NULLPTR);
    }
}
static IcDyncInit  g_dync_init( nullptr, & gCleanUp );

// ========================================================
// handle the allocate and free operation
// ========================================================
static void  gObjCacheCallback( void *, void **obj_pptr, IcObjectCache::HandlerType h_type )
{
    Q_ASSERT( obj_pptr != Q_NULLPTR );
    if ( h_type == IcObjectCache::HandlerType_Alloc ) {
        *obj_pptr = qxpack_ic_new( IcQcGuard_Item );
    } else {
        if ( *obj_pptr != nullptr ) {
            qxpack_ic_delete( *obj_pptr, IcQcGuard_Item );
            *obj_pptr = nullptr;
        }
    }
}

// ========================================================
// return the object cache instance
// ========================================================
static IcObjectCache &  globalInstance( )
{
    g_locker.lock();
    IcObjectCache *cache = g_obj_cache.loadAcquire();
    if ( cache == Q_NULLPTR ) {
        cache = qxpack_ic_new(
            IcObjectCache, & gObjCacheCallback, Q_NULLPTR, 64
        );
        g_obj_cache.store( cache );
    }
    g_locker.unlock();

    return *cache;
}



// //////////////////////////////////////////////////////////
//
//   IcQcGuard_Item Cache Manager
//
// //////////////////////////////////////////////////////////
// ============================================================
// clean up global cache
// ============================================================
void   IcQcGuard ::cleanupCache() {
    g_locker.lock();
    gCleanUp( );
    g_locker.unlock();
}

// ============================================================
// CTOR
// ============================================================
    IcQcGuard :: IcQcGuard ( )
{
    gRegInMeta();
    m_obj = Q_NULLPTR;
}

// ============================================================
// CTOR
// ============================================================
    IcQcGuard :: IcQcGuard ( bool cr )
{
    gRegInMeta();
    m_obj = Q_NULLPTR;
    if ( cr ) {
        m_obj = globalInstance().allocate();
        T_PrivPtr( m_obj )->addRef();
    }
}

// ============================================================
// DTOR
// ============================================================
    IcQcGuard :: ~IcQcGuard (  )
{
    if ( m_obj != Q_NULLPTR ) {
        if ( T_PrivPtr( m_obj )->releaseRef() == 0 ) {
            globalInstance().recycle( m_obj );
            m_obj = Q_NULLPTR;
        }   
    }
}

// ============================================================
// CTOR ( Copy )
// ============================================================
    IcQcGuard :: IcQcGuard ( const IcQcGuard &other )
{
    m_obj = other.m_obj;
    if ( m_obj != Q_NULLPTR ) { T_PrivPtr( m_obj )->addRef(); }
}

// ============================================================
// operator = 
// ============================================================
IcQcGuard &  IcQcGuard :: operator = ( const IcQcGuard &other )
{
    if ( m_obj != Q_NULLPTR ) { 
        if ( T_PrivPtr( m_obj )->releaseRef( ) == 0 ) { 
            globalInstance().recycle( m_obj );
        }
        m_obj = Q_NULLPTR;
    }
    m_obj = other.m_obj;
    if ( m_obj != Q_NULLPTR ) { T_PrivPtr( m_obj )->addRef( ); }

    return *this;
}

// ============================================================
// method wrap
// ============================================================
bool  IcQcGuard :: waitForDone( int accept_mill )
{ return ( m_obj != Q_NULLPTR ? T_PrivPtr( m_obj )->waitForDone( accept_mill ) : false ); }

bool  IcQcGuard :: beginWork( ) { return ( m_obj != Q_NULLPTR ? T_PrivPtr( m_obj )->beginWork() : false ); }
void  IcQcGuard :: endWork  ( bool is_done )   { if ( m_obj != Q_NULLPTR ) { T_PrivPtr( m_obj )->endWork( is_done ); } }


}

#endif
