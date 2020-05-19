#ifndef QMLUSERLISTMODELDEMO_PERSONINFO_HXX
#define QMLUSERLISTMODELDEMO_PERSONINFO_HXX

#include "qmluserlistmodeldemo/base/common/qmluserlistmodeldemo_def.h"
#include <QObject>
#include <QString>
#include <QDate>

namespace qmlUserListmodelDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief a copy on write information object
 */
// ////////////////////////////////////////////////////////////////////////////
// [HINT] use PImpl ( Copy On Write ) can reduce memory
class QMLUSERLISTMODELDEMO_API  PpPersonInfo {
public:
    //! ctor
    PpPersonInfo ( );

    //! ctor ( copy )
    PpPersonInfo ( const PpPersonInfo & );

    //! operator =
    PpPersonInfo &  operator = ( const PpPersonInfo & );

    //! dtor
    virtual ~PpPersonInfo( );

    // ========================================================================
    // property access
    // ========================================================================
    QString  name( ) const;
    QString  id  ( ) const;
    QDate    dob ( ) const; // dob means Date Of Birth ( born date )

    void  setName ( const QString & );
    void  setId   ( const QString & );
    void  setDob  ( const QDate & );

private:
    void *m_obj;
};

}

#endif
