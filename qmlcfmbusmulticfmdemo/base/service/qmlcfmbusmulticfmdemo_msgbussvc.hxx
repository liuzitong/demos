#ifndef QMLCFMBUSMULTICFMDEMO_MSGBUSSVC_HXX
#define QMLCFMBUSMULTICFMDEMO_MSGBUSSVC_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API MsgBusSvc : public QxPack::IcMsgBus {
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
