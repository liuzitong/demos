#include <QCoreApplication>
#include <QDebug>
#include <QMessageLogContext>
#include <QThread>

#include "core/main/appctrl/core_appctrl.hxx"
#include "core/base/common/core_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include "qxpack/indcom/sys/qxpack_ic_eventloopbarrier.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"

#include <cstdio>

// below is for linux
#include <signal.h>

#if !defined(WIN32)
#include <unistd.h>
#endif

// ============================================================================
// handle the SIGTERM
// ============================================================================
static void  gSigTerm_Handler( int sig )
{
    if ( sig == SIGTERM ) {
        QCoreApplication::quit();
    }
}

// ============================================================================
// print out current memory counter
// ============================================================================
static void  gPrintMemCntr( const char *stage )
{
    qInfo("%s, current new counter = %d", stage, core_curr_new_cntr );
    qInfo("%s, current qxpack ic counter=%d", stage, qxpack_ic_curr_new_cntr );
}

// ============================================================================
// the message handler
// ============================================================================
static void gMsgHandler( QtMsgType type, const QMessageLogContext &ctxt, const QString &msg )
{
    QByteArray  loc_msg = msg.toLocal8Bit();
    switch ( type ) {
    case QtDebugMsg : {
        std::fprintf (
          stderr, "[Debug]@%x %s ( ln:%u, %s )\n", reinterpret_cast<intptr_t>(QThread::currentThread()),
          loc_msg.constData(), ctxt.line, ctxt.function
        );
    }   break;
    case QtInfoMsg  : {
        std::fprintf(
          stderr, "[Info ]@%x %s ( ln:%u, %s )\n", reinterpret_cast<intptr_t>(QThread::currentThread()),
          loc_msg.constData(), ctxt.line, ctxt.function
          );
    }   break;
    case QtWarningMsg : {
        std::fprintf(
          stderr, "[Warn ]@%x %s ( ln:%u, %s )\n", reinterpret_cast<intptr_t>(QThread::currentThread()),
          loc_msg.constData(), ctxt.line, ctxt.function
        );
    }   break;
    case QtCriticalMsg: {
        char tmp[26]; QxPack::tailPath( ctxt.file, tmp, sizeof(tmp), 1 );
        std::fprintf(
          stderr, "[Criti]@%x %s ( %s:%u, %s )\n", reinterpret_cast<intptr_t>(QThread::currentThread()),
          loc_msg.constData(), tmp, ctxt.line, ctxt.function
          );
    }   break;
    case QtFatalMsg: {
        char tmp[26]; QxPack::tailPath( ctxt.file, tmp, sizeof(tmp), 1 );
        std::fprintf(
          stderr, "[Fatal]@%x %s ( %s:%u, %s )\n", reinterpret_cast<intptr_t>(QThread::currentThread()),
          loc_msg.constData(), tmp, ctxt.line, ctxt.function
        );
    }   abort();
    }
}

// ============================================================================
// main entry
// ============================================================================
int  main ( int argc, char *argv[] )
{
    int ret = 0;

    // handle the terminate signal
    signal( SIGTERM, & gSigTerm_Handler );
    qInstallMessageHandler( & gMsgHandler );

    // start the application
    gPrintMemCntr("pre-stage");
    {

        QCoreApplication a(argc, argv);
        core::AppCtrl *app_ctrl = new core::AppCtrl;
        app_ctrl->doInit();

        // --------------------------------------------------------------------
        //  event loop
        //  add main GUI or other cmd view here
        // --------------------------------------------------------------------

        // --------------------------------------------------------------------
        gPrintMemCntr("enter eventloop stage");
        ret = a.exec();
        QxPack::IcAppDclPriv::barrier( 32 );

        app_ctrl->doDeinit();
        delete app_ctrl;
        QxPack::IcAppDclPriv::barrier( 32 );
    }
    gPrintMemCntr("post-stage");

    return ret;
}
