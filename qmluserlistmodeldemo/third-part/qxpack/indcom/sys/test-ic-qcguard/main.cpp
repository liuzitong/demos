#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <string>
#include <QThread>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/sys/qxpack_ic_qcguard.hxx>
#include <qxpack/indcom/sys/qxpack_ic_vardata.hxx>
#include <QMetaObject>
#include <QVariant>

// /////////////////////////////////////////////////
//
// thread object
//
// /////////////////////////////////////////////////
class Worker : public QObject {
    Q_OBJECT
public:
    Worker( ) { QxPack::IcQcGuard(); }
    virtual ~Worker( ) { }
    Q_INVOKABLE void wake( QxPack::IcQcGuard &g, int sl_tm ) {
        QThread::msleep(sl_tm);
        if ( g.beginWork()) {
            qInfo() << "Wake up! And do some thing.";
            QThread::msleep(1000);
            qInfo() << "Done!";
            g.endWork();
        } else {
            qInfo() << "Wake up! but the caller dropped call.";
        }
    }
};

// /////////////////////////////////////////////////
//
// test object
//
// /////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase( ) {  }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void testPostCall( );
Q_SLOT  void testPostOverTime( );
public :
    Test ( ) {
        QVariant var_a; var_a = QVariant::fromValue( QxPack::IcVarData());
        QVariant var_b = var_a;
        QVariant var_c( var_a );

    }
    ~Test( ) { }
};

// =================================================
// final cleanup
// =================================================
void  Test :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" <<
            QxPack::IcMemCntr::currNewCntr();
}

// ========================================
// test the normal post call
// ========================================
void   Test :: testPostCall()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   {
       QThread wkr_thread;
       Worker *wkr = new Worker;
       wkr->moveToThread( & wkr_thread );
       QObject::connect( & wkr_thread, SIGNAL(finished()), wkr, SLOT(deleteLater()) );
       wkr_thread.start();
       {
           QxPack::IcQcGuard guard(true);
           QMetaObject::invokeMethod (
               wkr, "wake", Qt::QueuedConnection,
               Q_ARG( QxPack::IcQcGuard&, guard ),
               Q_ARG( int, 1000 )
           );
           guard.waitForDone(3000);
       }
       wkr_thread.quit();
       wkr_thread.wait();
   }
   QxPack::IcQcGuard::cleanupCache();
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}

// ========================================
// test over time
// ========================================
void   Test :: testPostOverTime()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   {
       QThread wkr_thread;
       Worker *wkr = new Worker;
       wkr->moveToThread( & wkr_thread );
       QObject::connect( & wkr_thread, SIGNAL(finished()), wkr, SLOT(deleteLater()) );
       wkr_thread.start();
       {
           QxPack::IcQcGuard guard(true);
           QMetaObject::invokeMethod (
               wkr, "wake", Qt::QueuedConnection,
               Q_ARG( QxPack::IcQcGuard&, guard ),
               Q_ARG( int, 3000 )
           );
           guard.waitForDone(1000);
       }
       wkr_thread.quit();
       wkr_thread.wait();
   }
   QxPack::IcQcGuard::cleanupCache();
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}




QTEST_MAIN( Test )
#include "main.moc"
