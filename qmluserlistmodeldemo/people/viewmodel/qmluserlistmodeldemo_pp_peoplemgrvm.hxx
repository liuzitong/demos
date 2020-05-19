#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLEMGRVM_HXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLEMGRVM_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include <QObject>
#include <QVariantList>
#include <QJsonObject>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main view model
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_HIDDEN  PpPeopleMgrVm : public QObject {
    Q_OBJECT

    // [HINT] view can access the peoplelist by property
    Q_PROPERTY( QObject* peopleList  READ  peopleListObj CONSTANT )
public:
    //! ctor
    Q_INVOKABLE explicit PpPeopleMgrVm( const QVariantList & );

    //! dtor
    virtual ~PpPeopleMgrVm( ) Q_DECL_OVERRIDE;

    //! property
    QObject*  peopleListObj( ) const;

    //! method supported
    Q_INVOKABLE void  addPerson ( const QJsonObject & );
    Q_INVOKABLE void  clearPeople  ( );
    Q_INVOKABLE void  randGenPeople( );

private:
    void *m_obj;
    Q_DISABLE_COPY( PpPeopleMgrVm )
};

}

#endif
