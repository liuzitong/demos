#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLEMGRSVC_HXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLEMGRSVC_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include "qxpack/indcom/afw/qxpack_ic_svcbase.hxx"
#include <QJsonObject>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief people manager service
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API  PpPeopleMgrSvc : QxPack::IcSvcBase {
    Q_OBJECT
public:
    //! ctor
    Q_INVOKABLE explicit PpPeopleMgrSvc( const QVariantList & );

    //! dtor
    ~PpPeopleMgrSvc( ) Q_DECL_OVERRIDE;

    Q_INVOKABLE QObject*  peopleListObj( );

    Q_INVOKABLE void  addPerson     ( const QJsonObject &jo );
    Q_INVOKABLE void  clearPeople   ( );
    Q_INVOKABLE void  randGenPeople ( );

private:
    void *m_obj;
    Q_DISABLE_COPY( PpPeopleMgrSvc )
};



}

#endif
