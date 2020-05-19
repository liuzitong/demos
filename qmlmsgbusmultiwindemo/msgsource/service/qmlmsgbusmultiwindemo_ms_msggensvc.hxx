#ifndef QMLMSGBUSMULTIWINDEMO_MS_MSGGENSVC_HXX
#define QMLMSGBUSMULTIWINDEMO_MS_MSGGENSVC_HXX

#include "qmlmsgbusmultiwindemo/base/common/qmlmsgbusmultiwindemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_svcbase.hxx"
#include <QVariantList>

namespace qmlMsgbusMultiwinDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief this service used to generate some messages
 * @note  the global message group: "MsgGroup.animal" will show some text message text
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSMULTIWINDEMO_API  MsMsgGenSvc : public QxPack::IcSvcBase {
    Q_OBJECT
public:
    //! ctor
    //! @note after created, the service is starting
    Q_INVOKABLE explicit MsMsgGenSvc( const QVariantList & );

    //! dtor
    virtual ~MsMsgGenSvc( ) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( MsMsgGenSvc )
};

}

#endif
