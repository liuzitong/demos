#ifndef QMLMSGBUSMULTIWINDEMO_MAIN_CXX
#define QMLMSGBUSMULTIWINDEMO_MAIN_CXX

#include "qmlmsgbusmultiwindemo_main.hxx"

// services list
#include "qmlmsgbusmultiwindemo/main/viewmodel/qmlmsgbusmultiwindemo_mm_mainvm.hxx"

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//  wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
Main :: Main ( QObject *pa ) : QxPack::IcModMgrBase( pa )
{

}

// ============================================================================
// dtor
// ============================================================================
Main :: ~Main ( )
{

}

// ============================================================================
// register types
// ============================================================================
void  Main :: registerTypes( QxPack::IcAppCtrlBase *app_base )
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
    // [HINT] we use a MmMainVm as main view's model ( view model )
    // register here, so QML can directly create it.
    obj_mgr->registerType<qmlMsgbusMultiwinDemo::MmMainVm>(
        QStringLiteral("qmlMsgbusMultiwinDemo::MmMainVm"), Q_NULLPTR
    );
}


}

#endif
