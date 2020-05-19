#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QMetaObject>
#include <qxpack/indcom/ipc/qxpack_ic_jsonipccli.hxx>
#include <qxpack/indcom/ipc/qxpack_ic_memcntr.hxx>
#include "../../qxpack_ic_jsonrpc2.hxx"


int main(int argc, char *argv[])
{

    int ret = 0;
    {
        QCoreApplication a(argc, argv);
        QxPack::IcJsonIpcCli  cli( "MyIPC");
        QObject::connect( &cli, & QxPack::IcJsonIpcCli::newResult,
                          [ ]( const QxPack::IcJsonRpc2Result &rsl ) {
                             qInfo() << "new result ";
                          }
                         );
        QObject::connect( &cli, & QxPack::IcJsonIpcCli::newError,
                          []( const QxPack::IcJsonRpc2Error &err ) {
                             qInfo() << "new error";
                          }
                          );
        cli.waitForConnected( 3000 );

        if ( cli.isWorked()) {
            QxPack::IcJsonRpc2Request req;
            req.setId(1992);
            req.setMethod("show_me");
            if ( cli.postRequest( req )) {
               qInfo() << "post request OK";
            } else {
               qInfo() << "post request error";
            }
            ret = a.exec();
        } else {
            ret = -1;
        }
    }

    return ret;
}
//#include "jsonipccli-test.moc"
