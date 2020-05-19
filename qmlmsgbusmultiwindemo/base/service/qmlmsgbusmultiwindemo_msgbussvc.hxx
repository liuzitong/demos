#ifndef QMLMSGBUSMULTIWINDEMO_MSGBUSSVC_HXX
#define QMLMSGBUSMULTIWINDEMO_MSGBUSSVC_HXX

#include "qmlmsgbusmultiwindemo/base/common/qmlmsgbusmultiwindemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API MsgBusSvc : public QxPack::IcMsgBus {
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
