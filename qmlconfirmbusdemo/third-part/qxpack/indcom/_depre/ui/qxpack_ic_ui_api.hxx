#ifndef QXPACK_IC_UI_API_HXX
#define QXPACK_IC_UI_API_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/ui/qxpack_ic_ui_cfg.hxx>
#include <QObject>
#include <QString>
#include <QVariant>

#if !defined( QXPACK_IC_UI_CFG_NO_QML )
#  include <QQmlEngine>
#  include <QJSEngine>
#endif

namespace QxPack {

// /////////////////////////////////////////////////////
/*!
 * @brief this class support a set of APIs in UI(QML)
 */
// /////////////////////////////////////////////////////
class QXPACK_IC_API  IcUiApi : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString apiVersion READ apiVersion CONSTANT )
public:
    //! CTOR
#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    IcUiApi( QQmlEngine *, QJSEngine* );
#else
    IcUiApi ( );
#endif
    //! DTOR
    virtual ~IcUiApi( );

    //! create the instance of IcUiApi
#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    static QObject *  createInstance( QQmlEngine*, QJSEngine* );
#endif
    //! return the version of this API set
    QString  apiVersion() const;

    //! enter a new QEventLoop
    Q_INVOKABLE void  enterEventLoop( );

    //! leave a event loop
    Q_INVOKABLE void  leaveEventLoop( );

    //! post the queued method call
    Q_INVOKABLE  bool  postMetaCall(
        QObject* obj, const QString & method,
        const QVariant & = QVariant(), const QVariant & = QVariant(),
        const QVariant & = QVariant(), const QVariant & = QVariant()
    );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcUiApi )
};


}

#endif
