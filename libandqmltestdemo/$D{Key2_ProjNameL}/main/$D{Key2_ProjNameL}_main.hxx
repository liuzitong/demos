#ifndef $D{Key2_ProjNameU}_MAIN_HXX
#define $D{Key2_ProjNameU}_MAIN_HXX

#include "$D{Key2_ProjNameL}/base/common/$D{Key2_ProjNameL}_def.h"
#include "qxpack/indcom/afw/qxpack_ic_modmgrbase.hxx"
#include <QObject>

namespace $D{Key2_ProjNameN} {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main module
 */
// ////////////////////////////////////////////////////////////////////////////
class $D{Key2_ProjNameU}_API  Main : public QxPack::IcModMgrBase {
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
