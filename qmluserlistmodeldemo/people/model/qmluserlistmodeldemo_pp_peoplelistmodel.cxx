#ifndef QMLUSERLISTMODELDEMO_PP_PEOPLELISTMODEL_CXX
#define QMLUSERLISTMODELDEMO_PP_PEOPLELISTMODEL_CXX

#include "qmluserlistmodeldemo_pp_peoplelistmodel.hxx"
#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include <QList>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  qmluserlistmodeldemo_objcast( PpPeopleListModelPriv*, o )
class QMLUSERLISTMODELDEMO_HIDDEN  PpPeopleListModelPriv {
private:
    PpPeopleListModel   *m_parent;
    QList<PpPersonInfo>  m_data_list;
public :
    PpPeopleListModelPriv ( PpPeopleListModel *pa );
    ~PpPeopleListModelPriv( ) { }
    inline int  rowCount( ) { return m_data_list.size(); }
    QVariant    data( int idx, int role );
    void        randGen( );
    void        addPerson( const PpPersonInfo &);
    void        clear ( );
};

// ============================================================================
// ctor
// ============================================================================
PpPeopleListModelPriv :: PpPeopleListModelPriv ( PpPeopleListModel *pa )
{
    m_parent = pa;
}

// ============================================================================
// return the data of spec. index
// ============================================================================
QVariant  PpPeopleListModelPriv :: data ( int idx, int role )
{
    QVariant var;
    if ( idx >= 0  && idx < m_data_list.size() ) {
        switch( role ) { //[HINT] DisplayRole is the default role name ("display")
        case Qt::DisplayRole : var = QVariant( m_data_list.at( idx ).name()); break;
        case Qt::UserRole + 1: var = QVariant( m_data_list.at( idx ).id() );  break;
        case Qt::UserRole + 2: var = QVariant( m_data_list.at( idx ).name() ); break;
        case Qt::UserRole + 3: var = QVariant( m_data_list.at( idx ).dob() ); break;
        default: break;
        }
    }
    return var;
}

// ============================================================================
// rand. gen
// ============================================================================
void    PpPeopleListModelPriv :: randGen()
{
    // generate a new data set
    QxPack::IcLCG  lcg;
    QList<PpPersonInfo> tmp;
    for ( int i = 0; i < 10; i ++ ) {
        PpPersonInfo pi;
        QString nm = QString("%1%2%3").arg( QChar( ( lcg.value() % 26 ) + 'A' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ))
                         .arg( QChar( ( lcg.value() % 26 ) + 'a' ));
        QString id = QString("PID_%1").arg( lcg.value() % 100 );

        pi.setName( nm );
        pi.setId  ( id );
        pi.setDob ( QDate::currentDate() );
        tmp.append( pi );
    }

    // [HINT] replace current list, because all old context will be drop,
    // here we use beginResetModel() to tell view who want to visit this model
    m_parent->beginResetModel();
    m_data_list = tmp;
    m_parent->endResetModel();
}


// ============================================================================
// clear all member
// ============================================================================
void    PpPeopleListModelPriv :: clear ( )
{
    // [HINT] clear all context need to call beginResetModel() and endResetModel()
    // the signal is automatically emit in beginResetModel() and endResetModel()
    m_parent->beginResetModel();
    m_data_list = QList<PpPersonInfo>();
    m_parent->endResetModel();
}

// ============================================================================
// add new person into list
// ============================================================================
void    PpPeopleListModelPriv :: addPerson(const PpPersonInfo &pi )
{
    // [HINT] the new person we put it to last position
#if 0  // append mode
    int b_pos = m_data_list.size(); // the last position. ( current not used )
    int e_pos = b_pos; // we only use 1 element, so the range [b_pos,e_pos]
                       // are we used.
    m_parent->beginInsertRows( QModelIndex(), b_pos, e_pos );
    m_data_list.append( pi );
    m_parent->endInsertRows();
#else  // pre-append mode
    m_parent->beginInsertRows( QModelIndex(), 0, 0 );
    m_data_list.push_front( pi );
    m_parent->endInsertRows();
#endif
}


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
PpPeopleListModel :: PpPeopleListModel( QObject *pa ) : QAbstractListModel( pa )
{
    m_obj = qmluserlistmodeldemo_new( PpPeopleListModelPriv, this );
}

// ============================================================================
// dtor
// ============================================================================
PpPeopleListModel :: ~PpPeopleListModel( )
{
    if ( m_obj != nullptr ) {
        qmluserlistmodeldemo_delete( m_obj, PpPeopleListModelPriv );
    }
}

// ============================================================================
// return the total count of records
// ============================================================================
int  PpPeopleListModel :: rowCount(const QModelIndex &) const
{ return T_PrivPtr( m_obj )->rowCount(); }

// ============================================================================
// return the role flags
// ============================================================================
Qt::ItemFlags  PpPeopleListModel :: flags ( const QModelIndex &idx ) const
{  return ( idx.isValid() ? QAbstractItemModel::flags( idx ) | Qt::ItemIsUserCheckable : Qt::NoItemFlags );}

// ============================================================================
// return the role names used by model
// ============================================================================
QHash<int,QByteArray>  PpPeopleListModel :: roleNames() const
{
    QHash<int,QByteArray> roles;
    roles.insert( Qt::DisplayRole,  "display" );
    roles.insert( Qt::UserRole + 1, "id" );
    roles.insert( Qt::UserRole + 2, "name");
    roles.insert( Qt::UserRole + 3, "dob");
    return roles;
}

// ============================================================================
// return the data by index and role
// ============================================================================
QVariant   PpPeopleListModel :: data ( const QModelIndex &idx, int role ) const
{  return T_PrivPtr( m_obj )->data( idx.row(), role );  }

// ============================================================================
// set the data, now allowed.
// ============================================================================
bool       PpPeopleListModel :: setData ( const QModelIndex &, const QVariant &, int )
{ return false; }

// ============================================================================
// clear all context
// ============================================================================
void       PpPeopleListModel :: clear ( )
{ T_PrivPtr( m_obj )->clear(); }

// ============================================================================
// random genrate information
// ============================================================================
void       PpPeopleListModel :: randomGen( )
{ T_PrivPtr( m_obj )->randGen(); }

// ============================================================================
// add a new person
// ============================================================================
void       PpPeopleListModel :: addPerson( const PpPersonInfo &pid )
{ T_PrivPtr( m_obj )->addPerson( pid );  }




}

#endif
