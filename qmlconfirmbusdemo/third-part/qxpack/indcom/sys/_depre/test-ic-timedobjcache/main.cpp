#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <string>
#include <QThread>
#include <QMetaObject>
#include <QVariant>
#include <QDateTime>

#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/sys/qxpack_ic_timedobjcache.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"
#include "qxpack/indcom/sys/qxpack_ic_apptot_priv.hxx"

// ////////////////////////////////////////////////////////////////////////////
// test object
// ////////////////////////////////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase ( ) {  }
Q_SLOT  void cleanupTestCase ( );
Q_SLOT  void testObjCleanup ( );
public :
    Test ( ) {
    }
    ~Test( ) { }
};

// ============================================================================
// final cleanup
// ============================================================================
void  Test :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" << QxPack::IcMemCntr::currNewCntr();
}

// ============================================================================
// test the object cleanup
// ============================================================================
void   Test :: testObjCleanup()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   {
       struct Context { int m_cnt; };
       Context  ctxt;
       ctxt.m_cnt = 0;

       // ---------------------------------------------------------------------
       // create a timered object
       // ---------------------------------------------------------------------
       QxPack::IcTimedObjCache  toc(
           []( void *ct, void **obj_ptr, QxPack::IcObjectCache::HandlerType h_type ) {
               Context *ctxt = reinterpret_cast< Context*>( ct );

               if ( h_type == QxPack::IcObjectCache::HandlerType_Alloc ) {
                   *obj_ptr = qxpack_ic_new( QVariant );
                   qInfo() << QString("alloc object %1 at %2")
                              .arg( intptr_t( *obj_ptr )).arg( QDateTime::currentDateTime().toString());
                   ctxt->m_cnt ++;
               } else {
                   if ( *obj_ptr != nullptr ) {
                       qxpack_ic_delete( *obj_ptr, QVariant );
                       qInfo() << QString("free object %1 at %2")
                                  .arg( intptr_t( *obj_ptr) ).arg( QDateTime::currentDateTime().toString());
                       if ( -- ctxt->m_cnt == 0 ) { QCoreApplication::quit(); }
                   }
               }
           }, & ctxt, 8, 3000  // wait 3ms to do cleanup
       );

       // allocate a series data
       void *dp[16];
       for ( int x = 0; x < sizeof( dp ) / sizeof(dp[0]); x ++ ) {
           dp[x] = toc.allocate();
       }

       // the recycle the object
       for ( int x = 0; x < sizeof( dp ) / sizeof(dp[0]); x ++ ) {
           toc.recycle( dp[x] );
       }

       QxPack::IcAppTotPriv tot(
           30000, -1,
           []( void*, QxPack::IcAppTotPriv&, bool ) {
               qInfo() << "time out!";
               QCoreApplication::instance()->quit();
           }, Q_NULLPTR
       );
       QCoreApplication::instance()->exec();
   }
   QxPack::IcAppDclPriv::barrier();
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}


QTEST_MAIN( Test )
#include "main.moc"
