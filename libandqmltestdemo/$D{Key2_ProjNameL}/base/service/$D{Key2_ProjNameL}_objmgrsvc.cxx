#ifndef $D{Key2_ProjNameU}_MODMGRSVC_CXX
#define $D{Key2_ProjNameU}_MODMGRSVC_CXX

#include "$D{Key2_ProjNameL}_objmgrsvc.hxx"
#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_memcntr.hxx"
#include <QMutex>
#include <QAtomicPointer>
#include <QAtomicInt>
#include "qxpack/indcom/sys/qxpack_ic_rmtobjdeletor_priv.hxx"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
// msgbussvc wrapper
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
ObjMgrSvc :: ObjMgrSvc ( QObject * )
{ }

// ============================================================================
// dtor
// ============================================================================
ObjMgrSvc :: ~ObjMgrSvc ( ) { }


// ////////////////////////////////////////////////////////////////////////////
// static factory
// ////////////////////////////////////////////////////////////////////////////

static QAtomicPointer<ObjMgrSvc>  g_ptr( Q_NULLPTR );
static QMutex       g_locker;
static QAtomicInt   g_ref_cntr(0);

// ============================================================================
// get new instance
// ============================================================================
ObjMgrSvc *  ObjMgrSvc::getInstance( )
{
    ObjMgrSvc *ptr = Q_NULLPTR;

    g_locker.lock();
    ptr = g_ptr.loadAcquire();
    if ( ptr == Q_NULLPTR ) {
        g_ptr.store( ( ptr = new ObjMgrSvc( Q_NULLPTR ) ));
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
void  ObjMgrSvc::freeInstance()
{
    ObjMgrSvc *ptr = Q_NULLPTR;

    g_locker.lock();
    ptr = g_ptr.loadAcquire();
    if ( ptr != Q_NULLPTR ) {
        if ( g_ref_cntr.fetchAndSubOrdered(1) - 1 == 0 ) {
            QxPack::IcRmtObjDeletor::deleteObjInThread(
                ptr->thread(),
                [](void*, QObject* obj ) {
                    ObjMgrSvc *svc = qobject_cast<ObjMgrSvc*>( obj );
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
