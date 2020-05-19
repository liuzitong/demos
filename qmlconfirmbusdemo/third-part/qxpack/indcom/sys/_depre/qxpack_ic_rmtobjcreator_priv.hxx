#ifndef QXPACK_IC_RMTOBJCREATOR_PRIV_HXX
#define QXPACK_IC_RMTOBJCREATOR_PRIV_HXX

#include <QObject>
#include <QThread>
#include <QMetaObject>

namespace QxPack {


// ////////////////////////////////////////////////////
/*!
 * this object used to create object in a thread, and return it's pointer
 */
// ////////////////////////////////////////////////////
class  IcRmtObjCreator : public QObject {
    Q_OBJECT
public:
    typedef QObject* (* CreateFunc )( void* ctxt );

    //! create object in thread
    static QObject*  createObjInThread( QThread *t, CreateFunc f, void *ctxt )
    {
        if ( f == Q_NULLPTR ) { return Q_NULLPTR; }

        if ( t == Q_NULLPTR ) {
            return (* f )( ctxt );

        } else if ( QThread::currentThread() != t ) {
            IcRmtObjCreator *rmt_cr = new IcRmtObjCreator( f, ctxt );
            rmt_cr->moveToThread( t );
            QObject *r_obj = Q_NULLPTR;
            QMetaObject::invokeMethod (
                rmt_cr, "doCreate", Qt::BlockingQueuedConnection, Q_RETURN_ARG( QObject*, r_obj )
            );
            rmt_cr->deleteLater();
            return r_obj;

        } else {
            return (* f )( ctxt );
        }
    }

protected:
    IcRmtObjCreator( CreateFunc f, void *ctxt )
    { m_func = f; m_ctxt = ctxt; }
    virtual ~IcRmtObjCreator( ) { }

    Q_INVOKABLE  QObject* doCreate( )
    { return (* m_func )( m_ctxt ); }

private:
    CreateFunc  m_func; void *m_ctxt;
     Q_DISABLE_COPY( IcRmtObjCreator )
};



}
#endif
