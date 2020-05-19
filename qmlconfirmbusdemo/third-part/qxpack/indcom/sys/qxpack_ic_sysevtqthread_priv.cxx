#ifndef QXPACK_IC_SYSEVTQTHREAD_PRIV_CXX
#define QXPACK_IC_SYSEVTQTHREAD_PRIV_CXX

#include "qxpack_ic_sysevtqthread_priv.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx"

#include <QCoreApplication>
#include <QMetaObject>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QSemaphore>
#include <QObject>
#include <QEventLoop>
#include <QMap>
#include <QAtomicPointer>

namespace QxPack {

// ////////////////////////////////////////////////////////
//
// IcSysEvtQThreadPriv_Keeper
//
// ////////////////////////////////////////////////////////
static QAtomicPointer<QThread>  g_thread_ptr( Q_NULLPTR );
static QMutex      g_locker;
static QAtomicInt  g_ref_cntr(0);
static QThread *   getInstance ( );
static bool        freeInstance( );

static QMutex      g_depre_map_locker;
static QMap<void*,void*> g_depre_map;

static IcDyncInit  g_dync_init( nullptr,
    [](){ // dync.deInit stage
        // NOTE: do not use locker on dync.deInit stage! maybe dead locked!
        // delete all objects already in g_depre_list.
        QMap<void*,void*>::const_iterator c_itr = g_depre_map.cbegin();
        while ( c_itr != g_depre_map.cend()) {
            QThread *t = T_ObjCast( QThread*, c_itr.value() ); ++ c_itr;
            if ( t != Q_NULLPTR ) {
                t->disconnect();
                t->quit();
                t->wait();
                delete t;
            }
        }
        g_depre_map = QMap<void*,void*>();
    }
);


// =======================================================
// get an instance
// =======================================================
static QThread *  getInstance( )
{
    g_locker.lock();
    QThread *t = g_thread_ptr.loadAcquire();
    if ( t != Q_NULLPTR ) {
        g_ref_cntr.fetchAndAddOrdered(1);
    } else {
        t = new QThread;
        if ( QThread::currentThread() != QCoreApplication::instance()->thread() ) {        
            t->moveToThread( QCoreApplication::instance()->thread());
            QMetaObject::invokeMethod( t, "start", Qt::BlockingQueuedConnection );
        } else {
            t->start( );
        }
        g_thread_ptr.store( t );
        g_ref_cntr.store(1);
    }

    g_locker.unlock();

    return t;
}

// =======================================================
// free an instance
// =======================================================
static bool       freeInstance( )
{
    bool is_del = false;

    g_locker.lock();
    QThread *t = g_thread_ptr.loadAcquire();

    if ( t != Q_NULLPTR ) {
        if ( g_ref_cntr.fetchAndSubOrdered(1) - 1 == 0 ) {
            // NOTE: t->thread() is which t living in.
            if ( t->thread() != QThread::currentThread()) {
                QThread *tmp = t;
                QObject::connect(
                    tmp, &QThread::finished,
                    [tmp]() { tmp->deleteLater(); }
                );
                QObject::connect(
                    tmp, &QThread::destroyed,
                    [tmp]( ){ // remove item in the map.
                        g_depre_map_locker.lock();
                        g_depre_map.take( tmp );
                        g_depre_map_locker.unlock();
                    }
                );
                g_depre_map_locker.lock();
                g_depre_map.insert( tmp, tmp );
                g_depre_map_locker.unlock();
                QMetaObject::invokeMethod( tmp, "quit", Qt::QueuedConnection );
                t = Q_NULLPTR;
                g_thread_ptr.store( t );
            } else {
                t->quit();
                t->wait();
                delete t;
                t = Q_NULLPTR;
                g_thread_ptr.store( t );
            }
            is_del = true;
        }
    }

    g_locker.unlock();

    return is_del;
}

// ///////////////////////////////////////////////////////
//
// IcSysEvtQThreadPriv
//
// ///////////////////////////////////////////////////////
// =======================================================
// CTOR
// =======================================================
   IcSysEvtQThreadPriv :: IcSysEvtQThreadPriv ( )
{
    m_obj = getInstance( );
}

// =======================================================
// DTOR
// =======================================================
   IcSysEvtQThreadPriv :: ~IcSysEvtQThreadPriv( )
{
    if ( m_obj != Q_NULLPTR ) {
        freeInstance();
        m_obj = Q_NULLPTR;
    }
}

// =======================================================
// return current thread
// =======================================================
QThread*  IcSysEvtQThreadPriv :: thread( ) const
{ return T_ObjCast( QThread*, m_obj ); }

// =======================================================
// check if thread alive [ static ]
// =======================================================
bool      IcSysEvtQThreadPriv :: isAlive()
{
    bool is_alive = false;

    g_locker.lock();
    QThread *t = g_thread_ptr.loadAcquire();
    if (  t == Q_NULLPTR ) {
        g_depre_map_locker.lock();
        //if ( g_depre_map.size() > 0 ) { is_alive = true; }
        if ( ! g_depre_map.empty() ) { is_alive = true; }
        g_depre_map_locker.unlock();
    } else {
        is_alive = true;
    }
    g_locker.unlock();

    return is_alive;
}

}

//#include "qxpack_ic_sysevtqthread_priv.moc"
#endif
