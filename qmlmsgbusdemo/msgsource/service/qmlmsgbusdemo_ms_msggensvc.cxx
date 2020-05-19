#ifndef QMLMSGBUSDEMO_MS_MSGGENSVC_CXX
#define QMLMSGBUSDEMO_MS_MSGGENSVC_CXX

#include "qmlmsgbusdemo_ms_msggensvc.hxx"
#include <QTimer>
#include <QString>
#include <QDebug>

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_guns.hxx"
#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_memcntr.hxx"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
// message array
// ////////////////////////////////////////////////////////////////////////////
// below message will emit one by one
static const char*  g_msg[] = {
    "The brown fox jump over the lazy dog.",
    "The dog waked.",
    "The dog try to sniff out the fox's trail, but found nothing.",
    "The dog sleep again."
};

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( MsMsgGenSvcPriv*, o )
class QMLMSGBUSDEMO_HIDDEN  MsMsgGenSvcPriv : public QObject {
    Q_OBJECT
private:
    QTimer m_tmr;
    int    m_curr_idx;
protected:
    Q_SLOT void tmr_onTimeout();
public :
    explicit MsMsgGenSvcPriv( );
    virtual ~MsMsgGenSvcPriv( ) Q_DECL_OVERRIDE;
};

// ============================================================================
// ctor
// ============================================================================
MsMsgGenSvcPriv :: MsMsgGenSvcPriv ( )
{
    m_curr_idx = 0;
    m_tmr.setInterval(1000);
    m_tmr.setSingleShot(false);
    QObject::connect( & m_tmr, SIGNAL(timeout()), this, SLOT(tmr_onTimeout()));
    m_tmr.start();
}

// ============================================================================
// dtor
// ============================================================================
MsMsgGenSvcPriv :: ~MsMsgGenSvcPriv( )
{
    QObject::disconnect( &m_tmr, Q_NULLPTR, this, Q_NULLPTR );
    m_tmr.stop();
}

// ============================================================================
// slot: handle the timer event
// ============================================================================
void  MsMsgGenSvcPriv :: tmr_onTimeout()
{
    // [HINT] the application controller can get by name ( name is defined in
    // the user application. normally user should added a base/common/projname_guns.h
    // the GUNS_AppCtrl is there
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    QxPack::IcMsgBus *msg_bus = app_ctrl->msgBus();

    // [HINT] the message package is a copy on write class.
    QxPack::IcMsgBusPkg  pkg;
    pkg.setGroupName( "MsgGroup.animal" );  // Hint: must setup the msg group name
    pkg.setMessage  ( g_msg[ m_curr_idx ++ ] );
    if ( m_curr_idx >= sizeof( g_msg ) / sizeof( const char*) ) { m_curr_idx = 0; }

    // [HINT] use this method to post the message.
    // after posted, the 'pkg' is attached by MsgBus inner, here can destruct it.
    msg_bus->post( pkg );
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MsMsgGenSvc :: MsMsgGenSvc ( const QVariantList & )
{
    m_obj = qmlmsgbusdemo_new( MsMsgGenSvcPriv );
}

// ============================================================================
// dtor
// ============================================================================
MsMsgGenSvc :: ~MsMsgGenSvc ( )
{
    qmlmsgbusdemo_delete( m_obj, MsMsgGenSvcPriv );
}


}
#include "qmlmsgbusdemo_ms_msggensvc.moc"
#endif
