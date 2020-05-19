#ifndef QMLMSGBUSDEMO_APPSETTINGSSVC_HXX
#define QMLMSGBUSDEMO_APPSETTINGSSVC_HXX

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
    Q_OBJECT
private:
    explicit AppSettingsSvc( const QString &settings_file_path, QObject *pa = Q_NULLPTR );
    virtual ~AppSettingsSvc( ) Q_DECL_OVERRIDE;
public :
    static AppSettingsSvc*  getInstance ( );
    static void             freeInstance( );
};

}

#endif
