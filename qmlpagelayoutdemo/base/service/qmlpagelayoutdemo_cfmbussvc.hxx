#ifndef QMLPAGELAYOUTDEMO_CFMBUSSVC_HXX
#define QMLPAGELAYOUTDEMO_CFMBUSSVC_HXX

#include "qmlpagelayoutdemo/base/common/qmlpagelayoutdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_confirmbus.hxx"

namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  confirm bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API CfmBusSvc : public QxPack::IcConfirmBus {
    Q_OBJECT
private:
    explicit CfmBusSvc( QObject *pa = Q_NULLPTR );
    virtual ~CfmBusSvc( );
public :
    static CfmBusSvc*  getInstance ( );
    static void        freeInstance( );
};

}

#endif
