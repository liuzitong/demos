#ifndef QMLCONFIRMBUSDEMO_MM_MAINVM_CXX
#define QMLCONFIRMBUSDEMO_MM_MAINVM_CXX

#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_guns.hxx"
#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_memcntr.hxx"
#include "qmlconfirmbusdemo_mm_mainvm.hxx"
#include "qmlconfirmbusdemo/main/service/qmlconfirmbusdemo_mm_confirmsvc.hxx"

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

#include <QMetaObject>

namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmlconfirmbusdemo_objcast( MmMainVmPriv*, o )
class QMLCONFIRMBUSDEMO_HIDDEN  MmMainVmPriv : public QObject {
    Q_OBJECT
private:
    MmMainVm *m_parent;
    MmConfirmSvc  m_cfm_svc;
protected:
    // [HINT] the confirm bus spec. named SLOT.
    Q_SLOT void  IcConfirmBus_onReqConfirm ( QxPack::IcConfirmBusPkg & );
public :
    explicit MmMainVmPriv( MmMainVm *pa );
    virtual ~MmMainVmPriv( ) Q_DECL_OVERRIDE;
    inline void doDemo0() { m_cfm_svc.doDemo0(); }
};

// ============================================================================
//  ctor
// ============================================================================
MmMainVmPriv :: MmMainVmPriv ( MmMainVm *pa ) : m_cfm_svc( QVariantList())
{
    m_parent = pa;

    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    QxPack::IcConfirmBus *cfm_bus = app_ctrl->cfmBus();

    // [HINT] here we listen the confirm group
    cfm_bus->add( this, m_cfm_svc.confirmGroupNameDemo0() );
}

// ============================================================================
//  dtor
// ============================================================================
MmMainVmPriv :: ~MmMainVmPriv ( )
{
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    QxPack::IcConfirmBus  *cfm_bus = app_ctrl->cfmBus();

    // [HINT] remember to remove from confirm-bus
    cfm_bus->rmv( this, m_cfm_svc.confirmGroupNameDemo0() );
}

// ============================================================================
// slot: handle the confirm group
// ============================================================================
void  MmMainVmPriv :: IcConfirmBus_onReqConfirm( QxPack::IcConfirmBusPkg &pkg )
{
    QxPack::IcConfirmBusPkgQObj  cfm_pkg_obj;
    cfm_pkg_obj.setContent( pkg );
    if ( pkg.groupName() == m_cfm_svc.confirmGroupNameDemo0()) {
        emit m_parent->demo0_reqConfirm( &cfm_pkg_obj );
    }
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MmMainVm :: MmMainVm ( const QVariantList & )
{
    m_obj = qmlconfirmbusdemo_new( MmMainVmPriv, this );
}

// ============================================================================
// dtor
// ============================================================================
MmMainVm :: ~MmMainVm ( )
{
    qmlconfirmbusdemo_delete( m_obj, MmMainVmPriv );
}

// ============================================================================
// do the demo0
// ============================================================================
void   MmMainVm :: doDemo0()
{ T_PrivPtr( m_obj )->doDemo0(); }


}
#include "qmlconfirmbusdemo_mm_mainvm.moc"
#endif