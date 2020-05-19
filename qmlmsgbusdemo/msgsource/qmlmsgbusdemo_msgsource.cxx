#ifndef QMLMSGBUSDEMO_MSGSOURCE_CXX
#define QMLMSGBUSDEMO_MSGSOURCE_CXX

#include "qmlmsgbusdemo_msgsource.hxx"
#include <QCoreApplication>

// services list
#include "qmlmsgbusdemo/msgsource/service/qmlmsgbusdemo_ms_msggensvc.hxx"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
//  wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MsgSource :: MsgSource ( QObject *pa ) : QxPack::IcModMgrBase( pa )
{
}

// ============================================================================
// dtor
// ============================================================================
MsgSource :: ~MsgSource ( )
{
}

// ============================================================================
// register types
// ============================================================================
static MsMsgGenSvc *g_msg_gen_svc = Q_NULLPTR;

void  MsgSource :: registerTypes( QxPack::IcAppCtrlBase *app_base )
{
    // ------------------------------------------------------------------------
    // check param.
    // ------------------------------------------------------------------------
    if ( app_base == Q_NULLPTR ) { return; }
    QxPack::IcObjMgr *obj_mgr = app_base->objMgr();
    if ( obj_mgr  == Q_NULLPTR ) { return; }

    // ------------------------------------------------------------------------
    // register all public types of this module
    // ------------------------------------------------------------------------
    // [HINT]
    // NOTE: this is just for test! DO NOT use below code as real scenario.
    // here we directly start the message source service...
    g_msg_gen_svc = new MsMsgGenSvc(QVariantList());
    QObject::connect(
        QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
        []() {
            if ( g_msg_gen_svc != Q_NULLPTR ) {
                g_msg_gen_svc->deleteLater(); g_msg_gen_svc = Q_NULLPTR;
            }
        }
    );

}


}

#endif
