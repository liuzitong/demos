#ifndef QMLMSGBUSDEMO_MSGBUSSVC_HXX
#define QMLMSGBUSDEMO_MSGBUSSVC_HXX

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_API MsgBusSvc : public QxPack::IcMsgBus {
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
