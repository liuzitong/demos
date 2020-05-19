#include <QCoreApplication>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QMetaObject>
#include "../../qxpack_ic_jsonipcsrv.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/ipc/qxpack_ic_jsonrpc2.hxx>

// ///////////////////////////////////////
// local functions
// ///////////////////////////////////////
class  Handler : public QObject {
    Q_OBJECT
public:
    Handler ( QxPack::IcJsonIpcHandler *hde )
    {
        m_hde = hde;
        QObject::connect( hde, SIGNAL(newRequest( const QxPack::IcJsonRpc2Request &)),
                          this, SLOT(onNewRequest(const QxPack::IcJsonRpc2Request & )) );
        QObject::connect( hde, SIGNAL(ipcClosed()), this, SLOT(onIpcClosed()));
    }
    virtual ~Handler( )
    {
        if ( m_hde != Q_NULLPTR ) {
            QObject::disconnect( m_hde, 0, this, 0 );
            m_hde->deleteLater(); m_hde = Q_NULLPTR;
        }
    }
    Q_SLOT void sendReply( )
    {
        if ( m_hde != Q_NULLPTR ) {
            QxPack::IcJsonRpc2Result rsl;
            rsl.setId( m_req.id());
            m_hde->postResult( rsl );
        }
    }

    Q_SLOT void onIpcClosed( ) { m_hde = Q_NULLPTR; }

    Q_SLOT void onNewRequest( const QxPack::IcJsonRpc2Request &req )
    {
        qInfo() << "new request.";
        qInfo() << "id:" << req.id();
        qInfo() << "method:" << req.method();
        m_req = req;
        QMetaObject::invokeMethod( this, "sendReply", Qt::QueuedConnection );
    }

private:
    QxPack::IcJsonIpcHandler *m_hde;
    QxPack::IcJsonRpc2Request  m_req;
};

// //////////////////////////////////////
//
// main entry
//
// //////////////////////////////////////
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int ret = 0;

    {
        QxPack::IcJsonIpcSrv  ipc_srv("MyIPC");
        QList<Handler*> list;

        QObject::connect( & ipc_srv, & QxPack::IcJsonIpcSrv::newConnection,
                          [ &list]( QxPack::IcJsonIpcHandler *hde )
                          {
                              qInfo() << hde;
                              Handler *h = new Handler( hde );
                              list.append( h );
                          }
                         );
        qInfo() << "start the server:" << ipc_srv.start();
        ret = a.exec();

        while( ! list.isEmpty())
        {
            Handler *hde = list.takeFirst();
            if ( hde != Q_NULLPTR ) { delete hde; }
        }
    }
    qInfo() << "current new cntr:" << qxpack_ic_curr_new_cntr;
    qInfo() << "program end.";

    return ret;
}

#include "jsonipcsrv-test.moc"
