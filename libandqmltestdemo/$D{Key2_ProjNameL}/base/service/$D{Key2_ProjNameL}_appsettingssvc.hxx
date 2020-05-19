#ifndef $D{Key2_ProjNameU}_APPSETTINGSSVC_HXX
#define $D{Key2_ProjNameU}_APPSETTINGSSVC_HXX

#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
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
