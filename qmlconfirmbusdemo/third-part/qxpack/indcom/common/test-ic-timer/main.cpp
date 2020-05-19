#include <QCoreApplication>

#include <qxpack/indcom/common/qxpack_ic_bytearray.hxx>
#include <qxpack/indcom/common/qxpack_ic_logging.hxx>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/common/qxpack_ic_timer.hxx>

#include <QString>
#include <QDebug>
#include <cstdlib>
#include <QSemaphore>
#include <QObject>
#include <QtTest>

// /////////////////////////////////////////////////
//
// test object
//
// /////////////////////////////////////////////////
class  TestTimer : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase( ) { }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void testTimer( );
Q_SLOT  void testCall( );
public :
    TestTimer ( );
};

// =================================================
// CTOR
// =================================================
    TestTimer :: TestTimer ( ) {  }

// =================================================
// final cleanup
// =================================================
void  TestTimer :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" << QxPack::IcMemCntr::currNewCntr();
}

// ========================================
// test the timer
// ========================================
void   TestTimer :: testTimer()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    int t1_cntr = 0;
    int t2_cntr = 0;

    {
        QSemaphore sem;
        qInfo() << "wait about 10s.";

        QxPack::IcTimer t1( 45, -1, [](void*cntr_ptr){
                            int *c = T_PtrCast( int *, cntr_ptr );
                            *c = *c + 1;
                        }, &t1_cntr );

        QxPack::IcTimer t2( 45, 25, [](void*cntr_ptr){
                            int *c = T_PtrCast( int *, cntr_ptr );
                            *c = *c + 1;
                        }, &t2_cntr );

        QxPack::IcTimer t3( 10000, 1, [](void*sem_ptr){
                          qInfo() << "Timer T3 post quit().";
                          T_ObjCast( QSemaphore*, sem_ptr )->release(1);
                        }, &sem );

        QxPack::IcTimer *t4_ptr = nullptr;
        QxPack::IcTimer t4( 1000, 5, [](void*ctxt) {
                             qInfo() << "Timer T4 raised. directly call stop()";
                             if ( ctxt != nullptr ) {
                                 ( * T_ObjCast( QxPack::IcTimer **,ctxt ))->stop();
                             } else {
                                 qFatal("Timer T4 ctxt is null!");
                             }
                           }, &t4_ptr
                        );

        t1.start();
        t2.start();
        t3.start();
        t4_ptr = &t4;
        t4.start();
        sem.acquire(1); // wait..
    }
    QVERIFY2( t2_cntr == 25, QString("T2 timer counter is not 25, is %1").arg( t2_cntr ).toUtf8());
    QVERIFY2( QxPack::IcMemCntr::currNewCntr() == mem_cntr, "IcTimer::testTimer() exist memory leak!" );
}

// =========================================================
//  test the call queue
// =========================================================
void   TestTimer :: testCall()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    {
        QSemaphore w_sem;
        QxPack::IcCaller  caller(
            []( void *ctxt, void *param, QxPack::IcCaller::HandlerType ) {
               // always delete param, ignore the type
               qInfo() << "caller: call with param = " << * T_PtrCast( int *, param );
               if ( * T_PtrCast( int *, param ) == -1 ) {
                   T_ObjCast( QSemaphore *, ctxt )->release();
               }
               qxpack_ic_delete( param, int );
            }, & w_sem
        );

        int *p = qxpack_ic_new( int ); *p = 0;
        caller.post( p );
        p = qxpack_ic_new( int ); *p = 1;
        caller.post( p );
        p = qxpack_ic_new( int ); *p = 2;
        caller.post( p );
        p = qxpack_ic_new( int ); *p = 3;
        caller.post( p );

        p = qxpack_ic_new( int ); *p = -1;
        caller.post( p, 1000);
        w_sem.acquire();
    }
    QVERIFY2( QxPack::IcMemCntr::currNewCntr() == mem_cntr, "IcTimer::testCall() exist memory leak!" );
}



QTEST_MAIN( TestTimer )
#include "main.moc"
