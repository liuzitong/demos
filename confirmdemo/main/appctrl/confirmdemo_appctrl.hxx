#ifndef CONFIRMDEMO_APPCTRL_HXX
#define CONFIRMDEMO_APPCTRL_HXX

#include "confirmdemo/base/common/confirmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include <QObject>

namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief  the application controller
 */
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API  AppCtrl : public QxPack::IcAppCtrlBase {
    Q_OBJECT
    Q_PROPERTY( QObject*  objMgr READ  objMgrObj CONSTANT )
    Q_PROPERTY( QObject*  msgBus READ  msgBusObj CONSTANT )
public:
    //! ctor
    explicit AppCtrl( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~AppCtrl() Q_DECL_OVERRIDE;

    //! objMgrObj
    QObject*  objMgrObj( ) const;

    //! msgBusObj
    QObject*  msgBusObj( ) const;

protected:
    // override the name
    virtual QString   instanceName() const Q_DECL_OVERRIDE;

    // override the the initialization process
    virtual void      preInit( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcAppSettingsBase* createSingleton_AppSettings( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcMsgBus*  createSingleton_MsgBus( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcObjMgr*  createSingleton_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      registerTypes_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      postInit( ) Q_DECL_OVERRIDE;

    // override the deinitialization process
    virtual void      preDeinit() Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_MsgBus( ) Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_AppSettings( ) Q_DECL_OVERRIDE;
    virtual void      postDeinit() Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( AppCtrl )
};

}

#endif 

