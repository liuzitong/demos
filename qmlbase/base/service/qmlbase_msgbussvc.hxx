#ifndef QMLBASE_MSGBUSSVC_HXX
#define QMLBASE_MSGBUSSVC_HXX

#include "qmlbase/base/common/qmlbase_def.h"
#include "qxpack/indcom/afw/qxpack_ic_msgbus.hxx"

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
//
//  message bus manager
//
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API MsgBusSvc : public QxPack::IcMsgBus {
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
