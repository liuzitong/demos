#ifndef QXPACK_IC_NAMEDQTHREAD_PRIV_CXX
#define QXPACK_IC_NAMEDQTHREAD_PRIV_CXX

#include "qxpack_ic_namedqthread_priv.hxx"
#include <qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/sys/qxpack_ic_sysevtqthread_priv.hxx>

#include <QMutex>
#include <QMap>
#include <QVector>
#include <QDebug>

namespace QxPack {

// /////////////////////////////////////////////////
// pair of the qthread
// /////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcNamedQThreadPriv_Pair {
private: QAtomicInt m_ref_cntr; QThread *m_thread;
public :
    // ==================================================
    // CTOR
    // ==================================================
    IcNamedQThreadPriv_Pair ( )
    {
      IcSysEvtQThreadPriv *sys_init = new IcSysEvtQThreadPriv;
      m_thread = new QThread;
      QObject::connect(
         m_thread, & QThread::destroyed, [ sys_init ]( QObject* ){ delete sys_init; }
      );
      if ( m_thread->thread() != sys_init->thread() ) {
          m_thread->moveToThread( sys_init->thread() );
          QMetaObject::invokeMethod ( m_thread, "start", Qt::BlockingQueuedConnection );
      } else {
          m_thread->start();
      }
      m_ref_cntr.store(1);
    }

    // ================================================
    // DTOR
    // ================================================
    ~IcNamedQThreadPriv_Pair( )
    {
        if ( QThread::currentThread() != m_thread->thread() ) {
            QObject::connect( m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()) );
            QMetaObject::invokeMethod( m_thread, "quit", Qt::QueuedConnection );
        } else {
            m_thread->quit();
            m_thread->wait();
            delete m_thread;
        }
    }
    inline int  currRefCntr( ) { return m_ref_cntr.loadAcquire(); }
    inline int  incrCntr( )    { return m_ref_cntr.fetchAndAddOrdered(1) + 1; }
    inline int  decrCntr( )    { return m_ref_cntr.fetchAndSubOrdered(1) - 1; }
    inline QThread* thread()  { return m_thread; }
};

// /////////////////////////////////////////////////
//
// pool of the QThread by name
//
// /////////////////////////////////////////////////
static QMutex  g_locker;
static QMap<QString,void*>    g_thread_pool;

// =======================================================
// try get an instance by name
// =======================================================
static QThread *   gThread_getInstance( const QString &nm )
{
    QThread *t = Q_NULLPTR;
    if ( nm.isEmpty()) { return t; } // empty name

    g_locker.lock();
    QMap<QString,void*>::const_iterator itr = g_thread_pool.find( nm );
    if ( itr != g_thread_pool.cend()) {
        IcNamedQThreadPriv_Pair *pair = T_PtrCast( IcNamedQThreadPriv_Pair*, itr.value());
        pair->incrCntr();
        t = pair->thread();
    } else {
        IcNamedQThreadPriv_Pair *pair = qxpack_ic_new( IcNamedQThreadPriv_Pair );
        g_thread_pool.insert( nm, pair );
        t = pair->thread();
    }
    g_locker.unlock();

    return t;
}

// =========================================================
// free the specified thread..
// =========================================================
static bool      gThread_freeInstance( const QString &nm )
{
    bool is_delete = false;
    if ( nm.isEmpty()) { return is_delete; }
    IcNamedQThreadPriv_Pair *pair = Q_NULLPTR;

    g_locker.lock();
    QMap<QString,void*>::iterator itr = g_thread_pool.find( nm );
    if ( itr != g_thread_pool.end()) {
        pair = T_PtrCast( IcNamedQThreadPriv_Pair *, itr.value());
        if ( pair->decrCntr() > 0 ) {  // need not to free
            pair = Q_NULLPTR;
        } else { // should be free it
            g_thread_pool.erase( itr );
        }
    }
    g_locker.unlock();

    if ( pair != Q_NULLPTR ) {
        qxpack_ic_delete( pair, IcNamedQThreadPriv_Pair );
        is_delete = true;
    }

    return is_delete;
}

// /////////////////////////////////////////////////////
//
// IcNamedQThreadPriv
//
// /////////////////////////////////////////////////////
// =====================================================
// CTOR
// =====================================================
    IcNamedQThreadPriv :: IcNamedQThreadPriv ( const QString &name )
{
    m_obj = Q_NULLPTR; m_name = name;
    if ( ! name.isEmpty() ) {
        m_obj = gThread_getInstance( name );
    }
}

// =====================================================
// DTOR
// =====================================================
    IcNamedQThreadPriv :: ~IcNamedQThreadPriv ( )
{
    if ( m_obj != Q_NULLPTR ) {
        gThread_freeInstance( m_name );
        m_obj = Q_NULLPTR;
    }
}

// =====================================================
// check if it is null
// =====================================================
bool  IcNamedQThreadPriv :: isNull() const { return ( m_obj == Q_NULLPTR ); }

// =====================================================
// return the name of this qthread
// =====================================================
QString  IcNamedQThreadPriv :: name() const { return m_name; }

// =====================================================
// return the QThread object
// =====================================================
QThread *  IcNamedQThreadPriv :: thread() const { return T_ObjCast( QThread*, m_obj ); }

// =====================================================
// [ static ] return the local dir file visit thread name
// =====================================================
QString    IcNamedQThreadPriv :: sysThreadName ( int idx )
{   
    if ( idx > 99 ) { idx = 99; }
    if ( idx <  0 ) { idx =  0; } 
    return QString("#IcNamedQThreadPriv::systhreadname_%1#").arg( idx, 2, 10, QChar('0'));
}

}

#endif
