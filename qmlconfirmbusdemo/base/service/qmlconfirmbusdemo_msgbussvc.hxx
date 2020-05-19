#ifndef QMLCONFIRMBUSDEMO_MSGBUSSVC_HXX
#define QMLCONFIRMBUSDEMO_MSGBUSSVC_HXX

#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCONFIRMBUSDEMO_API MsgBusSvc : public QxPack::IcMsgBus {
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
