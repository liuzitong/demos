#ifndef QMLPAGELAYOUTDEMO_APPSETTINGSSVC_HXX
#define QMLPAGELAYOUTDEMO_APPSETTINGSSVC_HXX

#include "qmlpagelayoutdemo/base/common/qmlpagelayoutdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appsettingsbase.hxx"

namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  application settings manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API AppSettingsSvc : public QxPack::IcAppSettingsBase {
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
