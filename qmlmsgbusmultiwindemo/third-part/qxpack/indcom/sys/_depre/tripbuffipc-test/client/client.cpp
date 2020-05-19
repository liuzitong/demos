#include <QCoreApplication>
#include <qxpack/indcom/sys/qxpack_ic_tripbuffipc.hxx>
#include <QByteArray>
#include <QString>
#include <qDebug>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        QxPack::IcTripBuffIpc trip("ShmArea");
        qDebug() << "data generator created:" << trip.size( );
        while ( true ) {       
            static int cntr = 0;
            if ( ! trip.isDirty()) {
                QString data_str = QString("cntr = %1").arg( cntr ++ );
                QByteArray data_ba = data_str.toUtf8(); data_ba = data_ba.append((char)0);
                trip.write( data_ba.constData(), data_ba.size() );
            }
            QThread::msleep(1000);
        }
    }

    return a.exec();
}
