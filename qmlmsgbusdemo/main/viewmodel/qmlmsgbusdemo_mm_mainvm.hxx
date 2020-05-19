#ifndef QMLMSGBUSDEMO_MM_MAINVM_HXX
#define QMLMSGBUSDEMO_MM_MAINVM_HXX

#include "qmlmsgbusdemo/base/common/qmlmsgbusdemo_def.h"
#include <QObject>
#include <QVariantList>

namespace qmlMsgbusDemo {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the main view model
 */
// ////////////////////////////////////////////////////////////////////////////
class QMLMSGBUSDEMO_HIDDEN  MmMainVm : public QObject {
    Q_OBJECT

    // Hint: here we use a string to trace the named msg-group ("MsgGroup.animal") context
    Q_PROPERTY( QString  animalMsg READ  animalMsg  NOTIFY  animalMsgChanged )
public:
    //! ctor
    Q_INVOKABLE explicit MmMainVm( const QVariantList & );

    //! dtor
    virtual ~MmMainVm( ) Q_DECL_OVERRIDE;

    //! property
    QString  animalMsg( ) const;

    //! signals to hint animal message
    Q_SIGNAL void animalMsgChanged( );

private:
    void *m_obj;
    Q_DISABLE_COPY( MmMainVm )
};

}

#endif
