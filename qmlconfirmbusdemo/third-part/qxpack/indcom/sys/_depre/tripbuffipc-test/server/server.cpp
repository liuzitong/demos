#include <QCoreApplication>
#include <QThread>
#include <qxpack/indcom/sys/qxpack_ic_tripbuffipc.hxx>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if defined( _WIN32 )
    QFile file( QCoreApplication::applicationDirPath() + "/shm_client.exe");
    if ( ! file.exists()) { file.setFileName( QCoreApplication::applicationDirPath() + "/../client/shm_client.exe"); }
    if ( ! file.exists()) { qInfo("cound not found shm_client.exe"); return -1; }
#else
# error "this platform is not supported"
#endif
    {
        QProcess proc;
        proc.start( file.fileName() );
        proc.waitForStarted();

        QxPack::IcTripBuffIpc trip( 512, "ShmArea" );
        qInfo() << "data recv. created:" << trip.size();
        while ( true ) {
            const char *dp = ( const char *) trip.read();
            if ( dp != Q_NULLPTR ) {
                qDebug() << "recv OK:" << QString( dp );
            }
            QThread::msleep(1000);
        }

        proc.terminate();
        proc.waitForFinished();
        proc.kill();
    }

    return a.exec();
}
