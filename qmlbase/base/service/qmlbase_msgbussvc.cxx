#ifndef QMLBASE_MSGBUSSVC_CXX
#define QMLBASE_MSGBUSSVC_CXX

#include "qmlbase_msgbussvc.hxx"
#include "qmlbase/base/common/qmlbase_memcntr.hxx"
#include <QMutex>
#include <QAtomicPointer>
#include <QAtomicInt>

#include "qxpack/indcom/sys/qxpack_ic_rmtobjdeletor_priv.hxx"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
// msgbussvc wrapper
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MsgBusSvc :: MsgBusSvc ( QObject *pa ) : QxPack::IcMsgBus( pa ){ }

// ============================================================================
// dtor
// ============================================================================
MsgBusSvc :: ~MsgBusSvc ( ) { }


// ////////////////////////////////////////////////////////////////////////////
// static factory
// ////////////////////////////////////////////////////////////////////////////

static QAtomicPointer<MsgBusSvc>  g_ptr( Q_NULLPTR );
static QMutex       g_locker;
static QAtomicInt   g_ref_cntr(0);

// ============================================================================
// get new instance
// ============================================================================
MsgBusSvc *  MsgBusSvc::getInstance( )
{
    MsgBusSvc *ptr = Q_NULLPTR;

    g_locker.lock();
    ptr = g_ptr.loadAcquire();
    if ( ptr == Q_NULLPTR ) {
        g_ptr.store( ( ptr = new MsgBusSvc( Q_NULLPTR ) ));
        g_ref_cntr.store(1);
    } else {
        g_ref_cntr.fetchAndAddOrdered(1);
    }
    g_locker.unlock();
    return ptr;
}

// ============================================================================
// free the instance
// ============================================================================
void  MsgBusSvc::freeInstance()
{
    MsgBusSvc *ptr = Q_NULLPTR;

    g_locker.lock();
    ptr = g_ptr.loadAcquire();
    if ( ptr != Q_NULLPTR ) {
        if ( g_ref_cntr.fetchAndSubOrdered(1) - 1 == 0 ) {     
            QxPack::IcRmtObjDeletor::deleteObjInThread(
                ptr->thread(),
                [](void*, QObject* obj ) {
                    MsgBusSvc *svc = qobject_cast<MsgBusSvc*>( obj );
                    delete svc;
                }, Q_NULLPTR, ptr, false
            );
            g_ptr.store( Q_NULLPTR );
        }
    }
    g_locker.unlock();
}



}

#endif
