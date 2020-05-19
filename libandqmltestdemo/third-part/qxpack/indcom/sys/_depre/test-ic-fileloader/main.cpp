#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <string>
#include <QThread>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <QMetaObject>


// /////////////////////////////////////////////////
//
// test object
//
// /////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase( ) { }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void testPostCall( );
public :
    Test ( ) { }
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
   }
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}



QTEST_MAIN( Test )
#include "main.moc"
