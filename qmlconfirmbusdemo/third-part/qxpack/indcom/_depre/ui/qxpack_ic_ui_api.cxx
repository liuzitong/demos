#ifndef QXPACK_IC_UI_API_CXX
#define QXPACK_IC_UI_API_CXX

#include "qxpack_ic_ui_api.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <QMetaObject>
#include <QEventLoop>
#include <QStack>

namespace QxPack {

// //////////////////////////////////////////////////////
//
// private object
//
// //////////////////////////////////////////////////////
#define T_PrivPtr( o )  (( IcUiApiPriv *) o )
class QXPACK_IC_HIDDEN  IcUiApiPriv {
private:

#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    QQmlEngine *m_qml_eng;
    QJSEngine  *m_js_eng;
#endif
    QStack<void*> m_evt_loop_stack;
public :

#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    IcUiApiPriv ( QQmlEngine *, QJSEngine *);
#else
    IcUiApiPriv ( );
#endif
    virtual ~IcUiApiPriv( );
    inline QStack<void*> &  evtLoopStack() { return m_evt_loop_stack; }
};

// ======================================================
// CTOR
// ======================================================
#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    IcUiApiPriv :: IcUiApiPriv (QQmlEngine *qml, QJSEngine *js)
{
    m_qml_eng = qml; m_js_eng = js;
    if ( qml != Q_NULLPTR ) {
        qml->addImportPath( QString("qrc:/") );
    }
}
#else
    IcUiApiPriv :: IcUiApiPriv ( )
{
}
#endif

// ======================================================
// DTOR
// ======================================================
    IcUiApiPriv :: ~IcUiApiPriv ( )
{

}



// ///////////////////////////////////////////////////////
//
// main api
//
// ///////////////////////////////////////////////////////
// ======================================================
// CTOR
// ======================================================
#if !defined( QXPACK_IC_UI_CFG_NO_QML )
    IcUiApi :: IcUiApi ( QQmlEngine *q, QJSEngine *j )
{
    m_obj = qxpack_ic_new( IcUiApiPriv, q, j );
}
#else
    IcUiApi :: IcUiApi ( )
{
}
#endif

// ======================================================
// DTOR
// ======================================================
    IcUiApi :: ~IcUiApi ( )
{
    if ( m_obj != Q_NULLPTR ) {
        qxpack_ic_delete( m_obj, IcUiApiPriv );
    }
}

// ======================================================
// [ static ] create an instance for Engine
// NOTE: this object will owned by QmlEngine
// ======================================================
QObject *    IcUiApi::createInstance( QQmlEngine *q, QJSEngine *j )
{
    return qobject_cast<QObject*>( new IcUiApi( q, j ));
}

// ======================================================
// return an api version
// ======================================================
QString     IcUiApi :: apiVersion() const
{
    return QStringLiteral("1.0");
}

// ======================================================
// enter a new eventloop
// ======================================================
void  IcUiApi  :: enterEventLoop ( )
{
    QEventLoop *evt = new QEventLoop();
    T_PrivPtr( m_obj )->evtLoopStack().push( evt );
    evt->exec();
    evt->deleteLater();
}

// ======================================================
// leave a new eventloop
// ======================================================
void  IcUiApi  :: leaveEventLoop()
{
    if ( T_PrivPtr( m_obj )->evtLoopStack().isEmpty()) { return; }
    QEventLoop *evt = ( QEventLoop *) T_PrivPtr( m_obj )->evtLoopStack().pop();
    evt->quit();
}

// ======================================================
// post the meta call
// ======================================================
bool        IcUiApi :: postMetaCall(
    QObject *obj, const QString &method,
    const QVariant & var1, const QVariant & var2,
    const QVariant & var3, const QVariant & var4
)
{
    char method_name[64]; bool ret_call = false;
    if ( obj != Q_NULLPTR && ! method.isEmpty()) {
        const QChar *chr = method.constData( );
        int len = method.size(); len = ( len < 63 ? len : 62 );
        for ( int x = 0; x < len; x ++ ) { method_name[ x ] = chr[ x ].toLatin1(); }
        method_name[ len ] = 0;

        if ( var1.isNull()) { // no parameter
            ret_call = QMetaObject::invokeMethod( obj, & method_name[0], Qt::QueuedConnection );
        } else if ( var2.isNull() ) { // exist one parameter
            ret_call = QMetaObject::invokeMethod( obj, & method_name[0], Qt::QueuedConnection, Q_ARG( QVariant, var1 ));
        } else if ( var3.isNull() ) {
            ret_call = QMetaObject::invokeMethod( obj, & method_name[0], Qt::QueuedConnection, Q_ARG( QVariant, var1 ), Q_ARG( QVariant, var2 ));
        } else if ( var4.isNull() ) {
            ret_call = QMetaObject::invokeMethod( obj, & method_name[0], Qt::QueuedConnection, Q_ARG( QVariant, var1 ), Q_ARG( QVariant, var2 ), Q_ARG( QVariant, var3 ));
        } else {
            ret_call = QMetaObject::invokeMethod(
                obj, & method_name[0], Qt::QueuedConnection,
                Q_ARG( QVariant, var1 ), Q_ARG( QVariant, var2 ),
                Q_ARG( QVariant, var3 ), Q_ARG( QVariant, var4 )
            );
        }
    }
    return ret_call;
}



}

#endif
