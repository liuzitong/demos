#ifndef QMLMSGBUSDEMO_MSGSOURCE_HXX
#define QMLMSGBUSDEMO_NSGSOURCE_HXX

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_modmgrbase.hxx"
#include <QObject>

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the message source module
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_API MsgSource : public QxPack::IcModMgrBase {
    Q_OBJECT
public :
    //! ctor
    explicit MsgSource ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~MsgSource( ) Q_DECL_OVERRIDE;

    //! register types
    virtual void registerTypes( QxPack::IcAppCtrlBase* ) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( MsgSource )
};


}

#endif
