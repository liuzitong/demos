#ifndef CORE_APPCTRL_HXX
#define CORE_APPCTRL_HXX

#include "core/base/common/core_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include <QObject>

namespace core {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief  the application controller
 */
// ////////////////////////////////////////////////////////////////////////////
class CORE_API  AppCtrl : public QxPack::IcAppCtrlBase {
    Q_OBJECT
public:
    //! ctor
    explicit AppCtrl( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~AppCtrl() Q_DECL_OVERRIDE;

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

