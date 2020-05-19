#ifndef CONFIRMDEMO_APPSETTINGSSVC_HXX
#define CONFIRMDEMO_APPSETTINGSSVC_HXX

#include "confirmdemo/base/common/confirmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace confirmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class CONFIRMDEMO_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
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
