#include <QCoreApplication>
#include <qDebug>
#include <cstdlib>
#include <QObject>
#include <QtTest>
#include <string>
#include <QThread>
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/afw/qxpack_ic_actionexec.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"
#include <QMetaObject>

// ////////////////////////////////////////////////////////////////////////////
// enum type
// ////////////////////////////////////////////////////////////////////////////
class EnumObject : public QObject {
    Q_OBJECT
public:
    enum ActionID { ActionID_TestSw = 0, ActionID_TestSw2 = 1, ActionID_TestTrg };
    Q_ENUM( ActionID )
};

// ////////////////////////////////////////////////////////////////////////////
// handler back
// ////////////////////////////////////////////////////////////////////////////
class  CallbackHandler : public QObject {
    Q_OBJECT
public :
    CallbackHandler(  ) {  }

    Q_INVOKABLE bool  onSw_ActionID_TestSw2 ( const QJsonObject &, const QxPack::IcVarData &, int c_st, int t_st )
    {
        qInfo() << "CallbackHandler::onSw_ActionID_TestSw2() :" << c_st << ">" << t_st;
        return true;
    }
};


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
Q_SLOT  void testCrThreadAndPost( );
Q_SLOT  void testCrThreadAndSend( );
Q_SLOT  void testNoThreadAndPost( );
Q_SLOT  void testNoThreadAndSend( );
        void doTestRoutine( bool is_post = true, QThread *t = Q_NULLPTR );
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
// do the test routine
// ============================================================================
void  Test :: doTestRoutine( bool is_post, QThread *t )
{
   EnumObject eo;
   QxPack::IcActionExec  act_exec ( t );
   act_exec.buildNode (
       EnumObject::ActionID::ActionID_TestSw, QxPack::IcActionExec::ActionType_Switch,
       []( void*,  const QJsonObject &, const QxPack::IcVarData&, int curr_state, int target_state )
       -> bool {
           qInfo() << "state change callback:" << curr_state << " to " << target_state;
           return true;
       }, Q_NULLPTR
   );

   CallbackHandler  cb_obj;
   QMetaEnum  act_enum = QxPack::IcActionExec::findActionIDEnum( &eo );
   act_exec.autoBuildNodes( & cb_obj, act_enum );

   if ( is_post ) {
       act_exec.post( EnumObject::ActionID::ActionID_TestSw, QJsonObject() );
       act_exec.post( EnumObject::ActionID::ActionID_TestSw2, QJsonObject() );
   } else {
       act_exec.send( EnumObject::ActionID::ActionID_TestSw, QJsonObject() );
       act_exec.send( EnumObject::ActionID::ActionID_TestSw2, QJsonObject() );
    }
    act_exec.waitForAllDone( true );

}

// ============================================================================
// test no action thread, post call
// ============================================================================
void   Test :: testNoThreadAndPost()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   this->doTestRoutine( true, Q_NULLPTR );
   QxPack::IcAppDclPriv::barrier(16);
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}

// ============================================================================
// test no action thread, send call
// ============================================================================
void   Test :: testNoThreadAndSend()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   this->doTestRoutine( false, Q_NULLPTR );
   QxPack::IcAppDclPriv::barrier(16);
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}

// ============================================================================
// test the normal post call
// ============================================================================
void   Test :: testCrThreadAndPost ()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();

//   QxPack::IcMemCntr::enableMemTrace(true);

   QThread wkr_thread;
   wkr_thread.start();
   this->doTestRoutine( true, & wkr_thread );
   wkr_thread.quit();
   wkr_thread.wait();
   QxPack::IcAppDclPriv::barrier(16);

//   QxPack::IcMemCntr::saveTraceInfoToFile("z:/t.txt");
//   QxPack::IcMemCntr::enableMemTrace(false);

   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}

// ============================================================================
// test workthread,  send call
// ============================================================================
void   Test :: testCrThreadAndSend ()
{
   int mem_cntr = QxPack::IcMemCntr::currNewCntr();
   QThread wkr_thread;
   wkr_thread.start();
   this->doTestRoutine( false, & wkr_thread );
   wkr_thread.quit();
   wkr_thread.wait();
   QxPack::IcAppDclPriv::barrier(16);
   QVERIFY2( mem_cntr == QxPack::IcMemCntr::currNewCntr(), "current memory counter is not equal" );
}




QTEST_MAIN( Test )
#include "main.moc"
