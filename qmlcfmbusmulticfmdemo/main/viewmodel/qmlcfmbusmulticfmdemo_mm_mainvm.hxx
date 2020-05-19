#ifndef QMLCFMBUSMULTICFMDEMO_MM_MAINVM_HXX
#define QMLCFMBUSMULTICFMDEMO_MM_MAINVM_HXX

#include "qmlcfmbusmulticfmdemo/base/common/qmlcfmbusmulticfmdemo_def.h"
#include <QObject>
#include <QVariantList>
#include <QJsonObject>

namespace qmlCfmBusMulticfmDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main view model
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLCFMBUSMULTICFMDEMO_HIDDEN  MmMainVm : public QObject {
    Q_OBJECT

public:
    //! ctor
    Q_INVOKABLE explicit MmMainVm( const QVariantList & );

    //! dtor
    virtual ~MmMainVm( ) Q_DECL_OVERRIDE;

    Q_INVOKABLE void  doDemo0( );

    Q_INVOKABLE void  doDemoInBg();

    //! notice view , a confirm package arrived.
    //! [HINT] the view must be directly connect to this signal
    Q_SIGNAL void  demo0_reqConfirm( QObject* cfm_pkg );

private:
    friend class MmMainVmPriv; // [HINT] use this to make private object visite signal and slot
    void *m_obj;
    Q_DISABLE_COPY( MmMainVm )
};

}

#endif
