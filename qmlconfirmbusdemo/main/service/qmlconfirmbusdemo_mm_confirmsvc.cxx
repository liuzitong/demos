#ifndef QMLCONFIRMBUSDEMO_MM_CONFIRMSVC_CXX
#define QMLCONFIRMBUSDEMO_MM_CONFIRMSVC_CXX

#include "qmlconfirmbusdemo_mm_confirmsvc.hxx"
#include <QTimer>
#include <QString>
#include <QDebug>

#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_guns.hxx"
#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_memcntr.hxx"

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

#define DEMO0_CFM_GROUPNAME   QStringLiteral("Confirm::Demo0")


namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( MmConfirmSvcPriv*, o )
class QMLCONFIRMBUSDEMO_HIDDEN  MmConfirmSvcPriv : public QObject {
    Q_OBJECT
private:
    QxPack::IcConfirmBus *m_cfm_bus;
public :
    explicit MmConfirmSvcPriv( );
    ~MmConfirmSvcPriv( ) Q_DECL_OVERRIDE;
    void  doDemo0( );
};

// ============================================================================
// ctor
// ============================================================================
MmConfirmSvcPriv :: MmConfirmSvcPriv ( )
{
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    m_cfm_bus = app_ctrl->cfmBus(); // [HINT] just get the confirm bus
}

// ============================================================================
// dtor
// ============================================================================
MmConfirmSvcPriv :: ~MmConfirmSvcPriv( ) = default;

// ============================================================================
// do the demo0
// ============================================================================
void  MmConfirmSvcPriv :: doDemo0()
{
    qInfo() << "start demo0, doing some thing...";

    // [HINT] prepare confirm package
    // a) just setup a confirm package
    QxPack::IcConfirmBusPkg cfm_pkg;
    cfm_pkg.setGroupName( DEMO0_CFM_GROUPNAME );
    cfm_pkg.setMessage( QStringLiteral("This is the demo0, answere yes or no in json result.\n e.g {\"result\":\"yes\"}"));

    // b) set package to confirm bus, and wait it return ( this will be blocking )
    m_cfm_bus->reqConfirm( cfm_pkg );

    // c)after execute qml get result do bussiness
    if ( cfm_pkg.isGotResult()) {
        QJsonObject jo_rsl = cfm_pkg.result();
        if ( jo_rsl.value("result").toString().toLower() == QStringLiteral("yes")) {
            qInfo() << "user answered: YES!";
        } else {
            qInfo() << "user answered: NO!";
        }
    } else {
        qInfo() << "no result, break !";
    }
}

// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MmConfirmSvc :: MmConfirmSvc ( const QVariantList & )
{
    m_obj = qmlconfirmbusdemo_new( MmConfirmSvcPriv );
}

// ============================================================================
// dtor
// ============================================================================
MmConfirmSvc :: ~MmConfirmSvc ( )
{
    qmlconfirmbusdemo_delete( m_obj, MmConfirmSvcPriv );
}

// ============================================================================
// return the confirm group name of Demo0
// ============================================================================
QString  MmConfirmSvc :: confirmGroupNameDemo0() const
{ return DEMO0_CFM_GROUPNAME; }

// ============================================================================
// do the demo0
// ============================================================================
void     MmConfirmSvc :: doDemo0()
{ T_PrivPtr( m_obj )->doDemo0(); }

}
#include "qmlconfirmbusdemo_mm_confirmsvc.moc"
#endif
