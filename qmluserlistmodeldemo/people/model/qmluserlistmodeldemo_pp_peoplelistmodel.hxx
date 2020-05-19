#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLELISTMODEL_HXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLELISTMODEL_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include "qmluserlistmodeldemo/people/model/qmluserlistmodeldemo_pp_personinfo.hxx"

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief  the people list model, provide the people information.
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLUSERLISTMODELDEMO_API  PpPeopleListModel : public QAbstractListModel {
    Q_OBJECT
public:
    //! ctor
    explicit PpPeopleListModel( QObject *pa );

    //! dtor
    virtual ~PpPeopleListModel( ) Q_DECL_OVERRIDE;

    // ========================================================================
    // override functions
    // ========================================================================
    //! the total records number
    virtual int        rowCount( const QModelIndex &)  const Q_DECL_OVERRIDE;

    //! return the data object by index and role
    virtual QVariant   data    ( const QModelIndex &idx, int role ) const Q_DECL_OVERRIDE;

    //! return all role id ( and names ) supported by this model
    //! @note we used roles like "id","name","dob", so view can use name to access it.
    virtual QHash<int,QByteArray>  roleNames( ) const Q_DECL_OVERRIDE;

    //! directly set data object at spec. index
    virtual bool       setData ( const QModelIndex &, const QVariant &, int ) Q_DECL_OVERRIDE;

    //! return flags ( characteristics )
    virtual Qt::ItemFlags  flags ( const QModelIndex & ) const Q_DECL_OVERRIDE;

    // ========================================================================
    // user functions
    // ========================================================================
    void   clear ( );
    void   randomGen( );  // randomize generate person information.
    void   addPerson( const PpPersonInfo &pid ); // add a person data into list

private:
    friend class PpPeopleListModelPriv;
    void *m_obj;
    Q_DISABLE_COPY( PpPeopleListModel )
};


}

#endif
