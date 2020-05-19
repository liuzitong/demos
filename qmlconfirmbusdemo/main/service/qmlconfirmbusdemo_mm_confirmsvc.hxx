#ifndef QMLCONFIRMBUSDEMO_MC_CONFIRMSVC_HXX
#define QMLCONFIRMBUSDEMO_MC_CONFIRMSVC_HXX

#include "qmlconfirmbusdemo/base/common/qmlconfirmbusdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_svcbase.hxx"
#include <QVariantList>
#include <QString>

namespace qmlconfirmbusdemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief this service used to demo. the confirm bus
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLCONFIRMBUSDEMO_API  MmConfirmSvc : public QxPack::IcSvcBase {
    Q_OBJECT
public:
    //! ctor
    //! @note after created, the service is starting
    Q_INVOKABLE explicit MmConfirmSvc( const QVariantList & );

    //! dtor
    virtual ~MmConfirmSvc( ) Q_DECL_OVERRIDE;

    //! return the group name of demo0
    QString  confirmGroupNameDemo0( ) const;

    //! do demo.
    void  doDemo0( );

private:
    void *m_obj;
    Q_DISABLE_COPY( MmConfirmSvc )
};

}

#endif
