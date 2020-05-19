#ifndef CORE_APPSETTINGSSVC_HXX
#define CORE_APPSETTINGSSVC_HXX

#include "core/base/common/core_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace core {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class CORE_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
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
