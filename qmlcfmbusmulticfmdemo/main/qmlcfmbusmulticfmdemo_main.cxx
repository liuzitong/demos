#ifndef QMLCFMBUSMULTICFMDEMO_MAIN_CXX
#define QMLCFMBUSMULTICFMDEMO_MAIN_CXX

#include "qmlcfmbusmulticfmdemo_main.hxx"

// services list
#include "qmlcfmbusmulticfmdemo/main/viewmodel/qmlcfmbusmulticfmdemo_mm_mainvm.hxx"

namespace qmlCfmBusMulticfmDemo {

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
    obj_mgr->registerType<qmlCfmBusMulticfmDemo::MmMainVm>(
        QStringLiteral("qmlCfmBusMulticfmDemo::MmMainVm"), Q_NULLPTR
    );
}


}

#endif
