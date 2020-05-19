#ifndef QMLMSGBUSDEMO_MM_MAINVM_CXX
#define QMLMSGBUSDEMO_MM_MAINVM_CXX

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_guns.hxx"
#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_memcntr.hxx"
#include "qmlmsgbusdemo_mm_mainvm.hxx"

#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( MmMainVmPriv*, o )
class QMLMSGBUSDEMO_HIDDEN  MmMainVmPriv : public QObject {
    Q_OBJECT
private:
    QString  m_animal_msg;
protected:
    // [HINT] the msg bus spec. named SLOT.
    Q_SLOT void  IcMsgBus_onMsg ( const QxPack::IcMsgBusPkg &pkg );
public :
    explicit MmMainVmPriv( );
    virtual ~MmMainVmPriv( ) Q_DECL_OVERRIDE;
    inline QString  animalMsg( ) const { return m_animal_msg; }
    Q_SIGNAL void  animalMsgChanged( );
};

// ============================================================================
//  ctor
// ============================================================================
MmMainVmPriv :: MmMainVmPriv ( )
{
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    QxPack::IcMsgBus *msg_bus = app_ctrl->msgBus();

    // Hint: here we listen the message group
    msg_bus->add( this, "MsgGroup.animal" );
}

// ============================================================================
//  dtor
// ============================================================================
MmMainVmPriv :: ~MmMainVmPriv ( )
{
    QxPack::IcAppCtrlBase *app_ctrl = QxPack::IcAppCtrlBase::instance( GUNS_AppCtrl );
    QxPack::IcMsgBus *msg_bus = app_ctrl->msgBus();

    // Hint: remember to remove from msg-group
    msg_bus->rmv( this, "MsgGroup.animal");
}

// ============================================================================
// slot: handle the message group
// ============================================================================
void  MmMainVmPriv :: IcMsgBus_onMsg( const QxPack::IcMsgBusPkg &pkg )
{
    if ( pkg.groupName() != "MsgGroup.animal" ) { return; }

    // [HINT] now we get the spec. message "MsgGroup.animal", we simply store
    // it and emit the signal
    m_animal_msg = pkg.message();
    emit this->animalMsgChanged();
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
MmMainVm :: MmMainVm ( const QVariantList & )
{
    m_obj = qmlmsgbusdemo_new( MmMainVmPriv );
    QObject::connect (
        T_PrivPtr( m_obj ), SIGNAL(animalMsgChanged()), this, SIGNAL(animalMsgChanged()),
        Qt::QueuedConnection // [TIP] use queued connection to make call stack is shallow
    );
}

// ============================================================================
// dtor
// ============================================================================
MmMainVm :: ~MmMainVm ( )
{
    QObject::disconnect( );
    qmlmsgbusdemo_delete( m_obj, MmMainVmPriv );
}

// ============================================================================
// return the current animal message
// ============================================================================
QString  MmMainVm :: animalMsg( ) const
{ return T_PrivPtr( m_obj )->animalMsg(); }



}
#include "qmlmsgbusdemo_mm_mainvm.moc"
#endif
