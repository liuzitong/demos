#ifndef QXPACK_IC_UI_REG_CXX
#define QXPACK_IC_UI_REG_CXX

#include "qxpack_ic_ui_reg.hxx"

#include <QList>
#include <QMutex>
#include <qxpack/indcom/ui/qxpack_ic_ui_api.hxx>
#include <qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx>
#include <qxpack/indcom/ui/qxpack_ic_ui_cfg.hxx>

#if !defined( QXPACK_IC_UI_CFG_NO_QML  )
#  include <QQmlEngine>
#endif 

namespace QxPack {

// ///////////////////////////////////////////////////////
// static functions
// ///////////////////////////////////////////////////////
static void QxPack_IcUiReg_Init( )
{ QxPack::IcUiReg(); }
Q_COREAPP_STARTUP_FUNCTION( QxPack_IcUiReg_Init )

// ///////////////////////////////////////////////////////
//  Register
// ///////////////////////////////////////////////////////
// =======================================================
// CTOR
// =======================================================
    IcUiReg :: IcUiReg ( )
{
    m_obj = Q_NULLPTR;
    static bool is_reg = false;
    if ( ! is_reg ) {
        is_reg = true;
#if !defined( QXPACK_IC_UI_CFG_NO_QML )
        qmlRegisterSingletonType<QxPack::IcUiApi>( "qxpack.indcom.ui.api", 1, 0, "QxIcUiApi", & QxPack::IcUiApi::createInstance );
#endif

    }
}

// =======================================================
// DTOR
// =======================================================
    IcUiReg :: ~IcUiReg ( )
{ }

}

#endif
