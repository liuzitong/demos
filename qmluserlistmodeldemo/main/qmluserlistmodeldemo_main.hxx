#ifndef QMLUSERLISTMODELDEMO_MAIN_HXX
#define QMLUSERLISTMODELDEMO_MAIN_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_modmgrbase.hxx"
#include <QObject>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main module
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API  Main : public QxPack::IcModMgrBase {
    Q_OBJECT
public :
    //! ctor
    explicit Main ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~Main( ) Q_DECL_OVERRIDE;

    //! register types
    virtual void registerTypes( QxPack::IcAppCtrlBase* ) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( Main )
};


}

#endif
