#ifndef QMLCFMBUSMULTICFMDEMO_APPSETTINGSSVC_HXX
#define QMLCFMBUSMULTICFMDEMO_APPSETTINGSSVC_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
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
