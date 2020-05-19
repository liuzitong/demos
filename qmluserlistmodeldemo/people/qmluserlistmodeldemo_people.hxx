#ifndef QMLUSERLISTMODELDEMO_PEOPLE_HXX
#define QMLUSERLISTMODELDEMO_PEOPLE_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_modmgrbase.hxx"
#include <QObject>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief manage the People information module
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API  People : public QxPack::IcModMgrBase {
    Q_OBJECT
public :
    //! ctor
    explicit People ( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~People ( ) Q_DECL_OVERRIDE;

    //! register types
    virtual void registerTypes( QxPack::IcAppCtrlBase* ) Q_DECL_OVERRIDE;

private:
    void *m_obj;
    Q_DISABLE_COPY( People )
};


}

#endif
