#ifndef QMLICQUICKIMAGEITEMDEMO_MAIN_CXX
#define QMLICQUICKIMAGEITEMDEMO_MAIN_CXX

#include "qmlicquickimageitemdemo_main.hxx"

// services list
#include "qmlicquickimageitemdemo/main/viewmodel/qmlicquickimageitemdemo_mm_mainvm.hxx"

namespace qmlIcquickimageitemDemo {

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
    obj_mgr->registerType<qmlIcquickimageitemDemo::MmMainVm>(
        QStringLiteral("qmlIcquickimageitemDemo::MmMainVm"), Q_NULLPTR
    );
}


}

#endif
