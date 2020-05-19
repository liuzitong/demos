#ifndef QMLCFMBUSMULTICFMDEMO_MC_CONFIRMSVC_HXX
#define QMLCFMBUSMULTICFMDEMO_MC_CONFIRMSVC_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_svcbase.hxx"
#include <QVariantList>
#include <QString>

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief this service used to demo. the confirm bus
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_API  MmConfirmSvc : public QxPack::IcSvcBase {
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

    //! do demo in bgthread
    void  doDemoInBg( );

private:
    void *m_obj;
    Q_DISABLE_COPY( MmConfirmSvc )
};

}

#endif
