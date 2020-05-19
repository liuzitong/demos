#ifndef QMLBASE_APPCTRL_CXX
#define QMLBASE_APPCTRL_CXX

#include "qmlbase_appctrl.hxx"
#include "qmlbase/base/service/qmlbase_appsettingssvc.hxx"
#include "qmlbase/base/service/qmlbase_msgbussvc.hxx"
#include "qmlbase/base/service/qmlbase_objmgrsvc.hxx"
#include "qmlbase/base/common/qmlbase_guns.hxx"
#include "qmlbase/base/common/qmlbase_memcntr.hxx"

#include <QCoreApplication>

// modules manager
#include "qmlbase/main/qmlbase_main.hxx"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//    private objects
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmlbase_objcast( AppCtrlPriv*, o )
class QMLBASE_HIDDEN AppCtrlPriv {
private:
    AppCtrl   *m_parent;
    QxPack::IcMsgBus *m_msg_bus_svc;
    QxPack::IcObjMgr *m_obj_mgr_svc;
    QxPack::IcAppSettingsBase *m_app_set_svc;

    // add other sub modules here
    Main      m_main_mod;
public :
    AppCtrlPriv ( AppCtrl *pa );
    ~AppCtrlPriv( );
    inline QxPack::IcMsgBus*  msgBusSvc( ) const { return m_msg_bus_svc; }
    inline QxPack::IcObjMgr*  objMgrSvc( ) const { return m_obj_mgr_svc; }
    inline QxPack::IcAppSettingsBase*  appSetSvc( ) const { return m_app_set_svc; }
    void  registerTypes( );
};

// ============================================================================
// ctor
// ============================================================================
AppCtrlPriv :: AppCtrlPriv ( AppCtrl *pa )
{
    m_parent = pa;
    m_app_set_svc = AppSettingsSvc::getInstance();
    m_msg_bus_svc = MsgBusSvc::getInstance();
    m_obj_mgr_svc = ObjMgrSvc::getInstance();
}

// ============================================================================
// dtor
// ============================================================================
AppCtrlPriv :: ~AppCtrlPriv ( )
{
    // release base service
    ObjMgrSvc::freeInstance();
    MsgBusSvc::freeInstance();
    AppSettingsSvc::freeInstance();
}

// ============================================================================
// register types in ObjMgr
// ============================================================================
void  AppCtrlPriv :: registerTypes()
{
    m_main_mod.registerTypes    ( m_parent );
}


// ////////////////////////////////////////////////////////////////////////////
//  wrapper API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
AppCtrl :: AppCtrl ( QObject *pa ) : QxPack::IcAppCtrlBase( pa )
{
    m_obj = qmlbase_new( AppCtrlPriv, this );
}

// ============================================================================
// dtor
// ============================================================================
AppCtrl :: ~AppCtrl ( )
{
    qmlbase_delete( m_obj, AppCtrlPriv );
}

// ============================================================================
// return the objmgr
// ============================================================================
QObject*  AppCtrl :: objMgrObj() const
{ return objMgr(); }

// ============================================================================
// return the msgBus
// ============================================================================
QObject*  AppCtrl :: msgBusObj() const
{ return msgBus(); }

// ============================================================================
// instance name
// ============================================================================
QString  AppCtrl :: instanceName() const
{
    return GUNS_AppCtrl;
}

// ============================================================================
//  previously initialization process
// ============================================================================
void    AppCtrl :: preInit( )
{ }

// ============================================================================
// create a global singleton message bus object
// ============================================================================
QxPack::IcMsgBus*  AppCtrl :: createSingleton_MsgBus( )
{
    return T_PrivPtr( m_obj )->msgBusSvc();
}

// ============================================================================
// create the object manager
// ============================================================================
QxPack::IcObjMgr*   AppCtrl :: createSingleton_ObjMgr( )
{
    return T_PrivPtr( m_obj )->objMgrSvc();
}

// ============================================================================
// create the appsettings manager
// ============================================================================
QxPack::IcAppSettingsBase*  AppCtrl :: createSingleton_AppSettings( )
{
    return T_PrivPtr( m_obj )->appSetSvc();
}

// ============================================================================
// register types in object manager
// ============================================================================
void       AppCtrl :: registerTypes_ObjMgr( )
{
    T_PrivPtr( m_obj )->registerTypes();
}

// ============================================================================
// post stage of initialization
// ============================================================================
void       AppCtrl :: postInit( )
{

}

// ============================================================================
// override the deinitialization process
// ============================================================================
void       AppCtrl :: preDeinit()
{

}

// ============================================================================
// release the refe. of object manager
// ============================================================================
void      AppCtrl :: releaseSingleton_ObjMgr ( )
{

}

// ============================================================================
// release the refe. of message bus
// ============================================================================
void      AppCtrl :: releaseSingleton_MsgBus( )
{

}

// ============================================================================
// release the refe. of application settings
// ============================================================================
void      AppCtrl :: releaseSingleton_AppSettings()
{

}

// ============================================================================
// post stage of deinitialization
// ============================================================================
void      AppCtrl :: postDeinit( )
{

}





}

#endif

