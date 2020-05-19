#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <cstring>
#include <memory>
#include <map>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/gemotry/qxpack_ic_point.hxx>

// /////////////////////////////////////////////////
// test object
// /////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void  initTestCase ( ) { }
Q_SLOT  void  cleanupTestCase ( );

Q_SLOT  void  create_NullObject ( );
Q_SLOT  void  create_SpecPoint  ( );
Q_SLOT  void  create_ByAnother  ( );
Q_SLOT  void  create_ByAssign ( );

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

// =================================================
// create null object
// =================================================
void   Test :: create_NullObject()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    bool is_pt_null = false, is_pt_f_null = false;
    {
        QxPack::IcPoint  pt;
        QxPack::IcPointF pt_f;
        is_pt_null   = pt.isNull();
        is_pt_f_null = pt_f.isNull();
    }
    int curr_cntr = QxPack::IcMemCntr::currNewCntr();
    QVERIFY2( mem_cntr == curr_cntr, "memory leak!");
    QVERIFY2( is_pt_null,   "IcPoint is not null."  );
    QVERIFY2( is_pt_f_null, "IcPointF is not null." );
}

// =================================================
// create spec. point
// =================================================
void   Test :: create_SpecPoint()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    int    x  = 10, y = 10;
    float  xf = 10, yf = 10;
    bool   is_pt_ok, is_pt_f_ok;
    {
        QxPack::IcPoint  pt  ( x, y );
        QxPack::IcPointF pt_f( xf, yf );
        is_pt_ok   = ( pt.x() == x && pt.y() == y );
        is_pt_f_ok = ( QxPack::fuzzyCompare( pt_f.x(), xf )  &&  QxPack::fuzzyCompare( pt_f.y(), yf ));
    }
    int curr_cntr = QxPack::IcMemCntr::currNewCntr();
    QVERIFY2( mem_cntr == curr_cntr, "memory leak!");
    QVERIFY2( is_pt_ok, "IcPoint  do not point to (10,10)." );
    QVERIFY2( is_pt_f_ok, "IcPointF do not point to (10.0,10.0)." );
}

// ===================================================
//  create object by another
// ===================================================
void    Test :: create_ByAnother()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    bool is_pt_same, is_pt_d_same, is_pt_f_same;
    {
        QxPack::IcPoint  pt1  ( 10, 10 );  QxPack::IcPoint  pt2  ( pt1 );
        QxPack::IcPointF pt_f1( 10, 10 );  QxPack::IcPointF pt_f2( pt_f1 );
        is_pt_same   = ( pt1.x() == pt2.x() && pt1.y() == pt2.y());
        is_pt_f_same = (
            QxPack::fuzzyCompare( pt_f1.x(), pt_f2.x()) &&
            QxPack::fuzzyCompare( pt_f1.y(), pt_f2.y())
        );
    }
    int curr_cntr = QxPack::IcMemCntr::currNewCntr();
    QVERIFY2( mem_cntr == curr_cntr, "memory leak!");
    QVERIFY2( is_pt_same,   "IcPoint create by Another is not same." );
    QVERIFY2( is_pt_f_same, "IcPointF create by Another is not same." );
}

// ===================================================
//  create object by assign
// ===================================================
void    Test :: create_ByAssign()
{
    int mem_cntr = QxPack::IcMemCntr::currNewCntr();
    bool is_pt_same, is_pt_d_same, is_pt_f_same;
    {
        QxPack::IcPoint  pt1( 10, 10 );    QxPack::IcPoint  pt2 = pt1;
        QxPack::IcPointF pt_f1( 10, 10 );  QxPack::IcPointF pt_f2 = pt_f1;
        is_pt_same   = ( pt1.x() == pt2.x() && pt1.y() == pt2.y());
        is_pt_f_same = (
            QxPack::fuzzyCompare( pt_f1.x(), pt_f2.x()) &&
            QxPack::fuzzyCompare( pt_f1.y(), pt_f2.y())
        );
    }
    int curr_cntr = QxPack::IcMemCntr::currNewCntr();
    QVERIFY2( mem_cntr == curr_cntr, "memory leak!");
    QVERIFY2( is_pt_same,   "IcPoint create by assign is not same." );
    QVERIFY2( is_pt_f_same, "IcPointF create by assign is not same." );
}





QTEST_MAIN( Test )
#include "main.moc"
