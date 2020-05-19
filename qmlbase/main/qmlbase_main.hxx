#ifndef QMLBASE_MAIN_HXX
#define QMLBASE_MAIN_HXX

#include "qmlbase/base/common/qmlbase_def.h"
#include "qxpack/indcom/afw/qxpack_ic_modmgrbase.hxx"
#include <QObject>

namespace qmlbase {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main module
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLBASE_API  Main : public QxPack::IcModMgrBase {
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
