#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <string>
#include <QThread>
#include <QMetaObject>
#include <QEventLoop>

#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/afw/qxpack_ic_actionexec.hxx"
#include "qxpack/indcom/afw/qxpack_ic_actstatelistener.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"


// ////////////////////////////////////////////////////////////////////////////
//
// test object
//
// ////////////////////////////////////////////////////////////////////////////
class  Test : public QObject {
    Q_OBJECT
private:
Q_SLOT  void initTestCase( ) {  }
Q_SLOT  void cleanupTestCase( );
Q_SLOT  void testListener( );
public :
    Test ( ) { }
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
// test no action thread, post call
// ============================================================================
void   Test :: testListener()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   QThread wkr_thread;
   wkr_thread.start();
   {
       enum ActionID { ActionID_TestSw = 1, ActionID_TestTrg };

       QxPack::IcActionExec  act_exec( & wkr_thread );
       act_exec.buildNode (
           ActionID_TestSw, QxPack::IcActionExec::ActionType_Switch,
           []( void*,  const QJsonObject &, const QxPack::IcVarData &,
               int curr_state, int target_state
           ) -> bool {
               qInfo() << "state change callback:" << curr_state << " to " << target_state;
               return true;
           }, Q_NULLPTR
       );

       QEventLoop  evt;

       QxPack::IcActStateListener  act_ls;
       act_ls.setupHandler(
            ActionID_TestSw,
            []( void*ctxt, int act_id, int c_st, int t_st ) {
                QEventLoop *evt_ptr = static_cast< QEventLoop*>( ctxt );
                qInfo() << "LS: action id:" << act_id << " state from " << c_st << " to " << t_st;
                QMetaObject::invokeMethod( evt_ptr, "quit", Qt::QueuedConnection );
            }, &evt
       );
       QObject::connect (
           & act_exec, SIGNAL(stateChanged(int,int,int)), & act_ls, SLOT(onStateChanged(int,int,int))
       );

       // do a later call
       QMetaObject::invokeMethod(
           & act_exec, "post", Qt::QueuedConnection,
           Q_ARG( int, ActionID_TestSw ), Q_ARG( QJsonObject, QJsonObject())
       );
       evt.exec( ); // while execute event, the post call will be execute
       act_exec.waitForAllDone( );
   }
   wkr_thread.quit();
   wkr_thread.wait();
   QxPack::IcAppDclPriv::barrier(16);

   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}


QTEST_MAIN( Test )
#include "main.moc"
