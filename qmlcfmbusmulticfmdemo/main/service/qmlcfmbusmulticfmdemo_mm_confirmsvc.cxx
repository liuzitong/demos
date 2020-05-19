#ifndef QMLCFMBUSMULTICFMDEMO_MM_CONFIRMSVC_CXX
#define QMLCFMBUSMULTICFMDEMO_MM_CONFIRMSVC_CXX

#include "qmlcfmbusmulticfmdemo_mm_confirmsvc.hxx"
#include <QTimer>
#include <QString>
#include <QDebug>

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_guns.hxx"
#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_memcntr.hxx"

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"
#include "qxpack/indcom/sys/qxpack_ic_rmtobjcreator_priv.hxx"

#define DEMO0_CFM_GROUPNAME   QStringLiteral("Confirm::Demo0")


namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
// ////////////////////////////////////////////////////////////////////////////
class   BgWorker : public QObject {
    Q_OBJECT
public :
    BgWorker( ) {
        QObject::connect( QThread::currentThread(), SIGNAL(finished()), this, SLOT(deleteLater()));
    }
    virtual ~BgWorker() Q_DECL_OVERRIDE { }

    Q_SLOT void  doDemo( )
    {
        qInfo() << QThread::currentThread() << "start demo1, doing some thing...";

        QxPack::IcConfirmBusPkg cfm_pkg;
        cfm_pkg.setGroupName( DEMO0_CFM_GROUPNAME );
        cfm_pkg.setMessage( QStringLiteral("This is the demo1, answere yes or no in json result.\n e.g {\"result\":\"yes\"}"));

        QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl )->cfmBus()->reqConfirm( cfm_pkg );

        if ( cfm_pkg.isGotResult()) {
            QJsonObject jo_rsl = cfm_pkg.result();
            if ( jo_rsl.value("result").toString().toLower() == QStringLiteral("yes")) {
                qInfo() << QThread::currentThread() <<  "user answered: YES!";
            } else {
                qInfo() << QThread::currentThread() <<  "user answered: NO!";
            }
        } else {
            qInfo() << QThread::currentThread() << "no result, break !";
        }
    }

};

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( MmConfirmSvcPriv*, o )
class QMLCFMBUSMULTICFMDEMO_HIDDEN  MmConfirmSvcPriv : public QObject {
    Q_OBJECT
private:
    QxPack::IcConfirmBus *m_cfm_bus;

    QThread  m_bg_thr; QObject *m_bg_wkr;
public :
    explicit MmConfirmSvcPriv( );
    virtual ~MmConfirmSvcPriv( ) Q_DECL_OVERRIDE;
    void  doDemo0( );
    inline void  doDemo1( ) {
        QMetaObject::invokeMethod( m_bg_wkr, "doDemo", Qt::QueuedConnection );
    }
};

// ============================================================================
// ctor
// ============================================================================
MmConfirmSvcPriv :: MmConfirmSvcPriv ( )
{
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    m_cfm_bus = app_ctrl->cfmBus(); // [HINT] just get the confirm bus
    m_bg_thr.start();
    m_bg_wkr = QxPack::IcRmtObjCreator::createObjInThread(
        & m_bg_thr, [](void*)->QObject* { return new BgWorker; }
        , Q_NULLPTR, false
    );
}

// ============================================================================
// dtor
// ============================================================================
MmConfirmSvcPriv :: ~MmConfirmSvcPriv( )
{
    m_bg_thr.quit();
    m_bg_thr.wait();
}

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

    // c) get result do bussiness
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
    m_obj = qmlcfmbusmulticfmdemo_new( MmConfirmSvcPriv );
}

// ============================================================================
// dtor
// ============================================================================
MmConfirmSvc :: ~MmConfirmSvc ( )
{
    qmlcfmbusmulticfmdemo_delete( m_obj, MmConfirmSvcPriv );
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

// ============================================================================
// do the demo1
// ============================================================================
void     MmConfirmSvc :: doDemoInBg()
{ T_PrivPtr(m_obj)->doDemo1(); }

}
#include "qmlcfmbusmulticfmdemo_mm_confirmsvc.moc"
#endif
