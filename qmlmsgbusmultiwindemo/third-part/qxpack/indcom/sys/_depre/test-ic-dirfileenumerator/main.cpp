#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <QThread>
#include <QMap>
#include <QImage>

#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/sys/qxpack_ic_dirfileenumerator.hxx>
#include <qxpack/indcom/sys/qxpack_ic_ncstring.hxx>
#include <qxpack/indcom/sys/qxpack_ic_apptot_priv.hxx>


// //////////////////////////////////////////////////
// static functions
// //////////////////////////////////////////////////
// ==================================================
// generate the temp file name only
// ==================================================
static QMap<QxPack::IcNcString,int>  gGenTmpName( )
{
   QMap<QxPack::IcNcString,int> file_map;
   file_map.insert( "1.jpg", true );
   file_map.insert( "2.jpg", true );
   file_map.insert( "3.jpg", true );
   return file_map;
}

// ==================================================
// generate the temp file at specified path
// ==================================================
static void gGenWriteImage( const QString &dir, const QMap<QxPack::IcNcString,int> &file_map )
{
   QImage test_im( 400, 400, QImage::Format_RGBA8888 ); test_im.fill( 0x00ff00 );
   QMap<QxPack::IcNcString,int>::const_iterator c_itr = file_map.cbegin();
   while ( c_itr != file_map.cend()) {
       test_im.save( dir + "/" + c_itr.key().strRef(), "jpeg");
       c_itr ++;
   }
}

// ==================================================
// delete all temp file that in map
// ==================================================
static void   gDelTempImage( const QString &dir, QMap<QxPack::IcNcString,int> &file_map )
{
   QMap<QxPack::IcNcString,int>::iterator itr = file_map.begin();
   while ( itr != file_map.end()) {
       QFile::remove( dir + "/" + itr.key().strRef() );
       itr ++;
   }
}

// /////////////////////////////////////////////////
//  loader
// /////////////////////////////////////////////////


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
Q_SLOT  void testEnumFiles( );

public :
    Test ( ) { }
    ~Test( ) { }
};

// =================================================
// final cleanup
// =================================================
void  Test :: cleanupTestCase()
{
    qInfo() << "current MemCntr:" << QxPack::IcMemCntr::currNewCntr();
}

// ========================================
// test first scan the file.
// ========================================
void   Test :: testEnumFiles()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
//    QxPack::IcMemCntr::enableMemTrace( true );
   {
       QMap<QxPack::IcNcString,int> file_map = gGenTmpName();
       gGenWriteImage( QCoreApplication::applicationDirPath(), file_map );
       static int testFirstScan_file_cntr = file_map.count();

       QxPack::IcDirFileEnumerator *file_enum =
           new QxPack::IcDirFileEnumerator (
               QStringList() << "*.jpg",
               QCoreApplication::applicationDirPath(), true
           );

       QObject::connect(
           file_enum, & QxPack::IcDirFileEnumerator::dataFound,
           [file_enum]( const QVector<QString> &data_list ) {
               static int add_cntr = 0;
               int cnt = data_list.count();
               for ( int i = 0; i < cnt; i ++ ) {
                   const QString &fn = data_list.at(i);
                   qInfo() << "data found:" << fn;
               }
               add_cntr += cnt;
               if ( add_cntr >= testFirstScan_file_cntr ) {
                   file_enum->deleteLater();
                   QCoreApplication::instance()->quit();
               }
           }
       );
       QObject::connect(
           file_enum, & QxPack::IcDirFileEnumerator::finished,
           [file_enum]() {
               file_enum->deleteLater();
               QCoreApplication::instance()->quit();
           }

       );

       // create wait timer, and enter in main loop
       QxPack::IcAppTotPriv::setTot(10000);
       QObject::connect(
           QCoreApplication::instance()->thread(), &QThread::started,
           [](){ qInfo() << "started"; }
       );
       QCoreApplication::instance()->exec();

       // remove all temp image
       gDelTempImage( QCoreApplication::applicationDirPath(), file_map );
   }

//   QxPack::IcMemCntr::saveTraceInfoToFile("mem_cntr.txt" );
//   QxPack::IcMemCntr::enableMemTrace(false);
   qInfo() << "current counter is:" << QxPack::IcMemCntr::currNewCntr();
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}



QTEST_MAIN( Test )
#include "main.moc"
