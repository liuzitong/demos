#include <QCoreApplication>
#include <QDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_objectcache.hxx"

// ////////////////////////////////////////////////////////////////////////////
// test object
// ////////////////////////////////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase( ) {  }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void callbackTest( );
public :
    Test ( ) { }
    ~Test( ) { }
};

// ============================================================================
// final cleanup
// ============================================================================
void  Test :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" <<  QxPack::IcMemCntr::currNewCntr();
}

// ============================================================================
// test the timer
// ============================================================================
void   Test :: callbackTest()
{
    int curr_mem_cntr = QxPack::IcMemCntr::currNewCntr();
    {
        QxPack::IcObjectCache  obj_cache(
           [](void*,void**pptr, QxPack::IcObjectCache::HandlerType h_type ) {
               if ( h_type == QxPack::IcObjectCache::HandlerType_Alloc ) {
                   *pptr = qxpack_ic_new( int );
               } else {
                   if ( *pptr != nullptr ) { qxpack_ic_delete( *pptr, int ); }
               }
           }, nullptr
        );

        void *ptr0 = obj_cache.allocate(); qInfo("allocated obj addr: 0x%zx", size_t( ptr0 ));
        void *ptr1 = obj_cache.allocate(); qInfo("allocated obj addr: 0x%zx", size_t( ptr1 ));
        void *ptr2 = obj_cache.allocate(); qInfo("allocated obj addr: 0x%zx", size_t( ptr2 ));
        void *ptr3 = obj_cache.allocate(); qInfo("allocated obj addr: 0x%zx", size_t( ptr3 ));

        obj_cache.recycle( ptr0 );
        obj_cache.recycle( ptr1 );
        obj_cache.recycle( ptr2 );
        obj_cache.recycle( ptr3 );

        obj_cache.freeOldest(0);
    }
    int curr_mem_cntr2 = QxPack::IcMemCntr::currNewCntr();

    QVERIFY2( curr_mem_cntr == curr_mem_cntr2, "memory counter is not equal." );
}

QTEST_MAIN( Test )
#include "main.moc"
