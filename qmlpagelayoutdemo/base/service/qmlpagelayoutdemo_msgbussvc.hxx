#ifndef QMLPAGELAYOUTDEMO_MSGBUSSVC_HXX
#define QMLPAGELAYOUTDEMO_MSGBUSSVC_HXX

#include "qmlpagelayoutdemo/base/common/qmlpagelayoutdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlPagelayoutDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLPAGELAYOUTDEMO_API MsgBusSvc : public QxPack::IcMsgBus {
    Q_OBJECT
private:
    explicit MsgBusSvc( QObject *pa = Q_NULLPTR );
    virtual ~MsgBusSvc( );
public :
    static MsgBusSvc*  getInstance ( );
    static void        freeInstance( );
};

}

#endif
