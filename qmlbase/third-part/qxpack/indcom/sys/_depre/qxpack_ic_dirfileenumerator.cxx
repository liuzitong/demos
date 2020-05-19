#ifndef QXPACK_IC_DIRFILEENUMERATOR_CXX
#define QXPACK_IC_DIRFILEENUMERATOR_CXX

#include "qxpack_ic_dirfileenumerator.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/sys/qxpack_ic_ncstring.hxx>
#include <qxpack/indcom/sys/qxpack_ic_workercreatortemp_priv.hpp>
#include <qxpack/indcom/sys/qxpack_ic_sysevtqthread_priv.hxx>
#include <qxpack/indcom/sys/qxpack_ic_jotaskworker.hxx>
#include <QDirIterator>
#include <QString>

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif


namespace QxPack {

// ///////////////////////////////////////////////////////////////
// register in meta system
// ///////////////////////////////////////////////////////////////
static bool  g_is_reg = false;
static void  gRegInQt ( )
{
    if ( ! g_is_reg ) {
        qRegisterMetaType<QVector<QString>>("QVector<QString>" );
        qRegisterMetaType<QVector<QString>>("QVector<QString>&");
        g_is_reg = true;
    }
}

// //////////////////////////////////////////////////////
//  private worker
// //////////////////////////////////////////////////////
#define T_PrivWorker( o )  T_ObjCast( IcDirFileEnumerator_Worker *, o )
class QXPACK_IC_HIDDEN IcDirFileEnumerator_Worker : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY( IcDirFileEnumerator_Worker )
private:
    IcJoTaskWorker  m_jo_worker;
    QStringList     m_file_pt;
    QString  m_dir;
    bool     m_enum_sub_dir;
protected:
    void  doEnum ( );
public :
    IcDirFileEnumerator_Worker ( const QStringList &pt, const QString &dir, bool sub_dir );
    virtual ~IcDirFileEnumerator_Worker( );

    Q_SIGNAL void  dataFound ( const QVector<QString> &data_list );
    Q_SIGNAL void  finished  ( );
};

// ======================================================
// ctor
// ======================================================
    IcDirFileEnumerator_Worker :: IcDirFileEnumerator_Worker (
        const QStringList &pt, const QString &dir, bool sub_dir
    )
{
    qxpack_ic_incr_new_cntr(1);

    // make a new task item
    m_file_pt = pt;
    m_dir     = dir;
    m_enum_sub_dir = sub_dir;

    // post task to do
    m_jo_worker.post(
        []( void *ctxt, const QJsonObject &, const IcVarData &, IcJoTaskWorker::HandlerType ht ) {
            if ( ht == IcJoTaskWorker::HandlerType_Drop ) { return; }
            IcDirFileEnumerator_Worker *t_this = T_ObjCast( IcDirFileEnumerator_Worker *, ctxt );
            if ( t_this == Q_NULLPTR ) { return; }
            t_this->doEnum( );
            emit t_this->finished();
        },this, QJsonObject()
    );
}

// =======================================================
// dtor
// =======================================================
    IcDirFileEnumerator_Worker :: ~IcDirFileEnumerator_Worker ( )
{
    m_jo_worker.shutdown();
    m_jo_worker.waitForAllDone(); // if the worker is busy, here need to wait it done
    qxpack_ic_decr_new_cntr(1);
}

// =======================================================
// enumeric directory
// =======================================================
void  IcDirFileEnumerator_Worker :: doEnum( )
{
    QDirIterator itr (
        m_dir, m_file_pt, QDir::Files,
        ( m_enum_sub_dir ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags ) | QDirIterator::FollowSymlinks
    );

    #define VECT_SIZE 32

    QVector<QString> fn_vect;
    fn_vect.reserve(VECT_SIZE);

    // here we enumerate all files that match the pattern
    // if we collect VECT_SIZE number files, emit them.
    while ( itr.hasNext()) {
        fn_vect.push_back( itr.next() );
        if ( fn_vect.size() >= VECT_SIZE ) {
            emit this->dataFound( fn_vect );
            fn_vect = QVector<QString>();
            fn_vect.reserve(VECT_SIZE);
        }
    }
    if ( ! fn_vect.isEmpty() ) { emit this->dataFound( fn_vect ); }
}

// //////////////////////////////////////////////////////
// worker creator ( live in sys_evt_thread )
// //////////////////////////////////////////////////////
#define T_WorkerCreator( o )  T_ObjCast( IcDirFileEnumerator_Worker_Creator *, o )
class QXPACK_IC_HIDDEN IcDirFileEnumerator_Worker_Creator :
    public QObject, public IcWorkerCreatorTemp< IcDirFileEnumerator_Worker > {
    Q_OBJECT
    Q_DISABLE_COPY( IcDirFileEnumerator_Worker_Creator )
private:
    IcSysEvtQThreadPriv m_sys_evt_thread;
public :
    // ctor
    IcDirFileEnumerator_Worker_Creator ( )
    { this->moveToThread( m_sys_evt_thread.thread()); }

    // dtor
    virtual ~IcDirFileEnumerator_Worker_Creator( ){ }

protected:
    // create worker in creator
    Q_INVOKABLE void createWorker ( const QStringList &fn_pt, const QString &dir, bool use_sub_dir )
    {
        this->deleteWorker();
        m_wkr = new IcDirFileEnumerator_Worker( fn_pt, dir, use_sub_dir );
    }

public :
    // try create a new creator
    static IcDirFileEnumerator_Worker_Creator*  newCreator( const QStringList &fn_pt, const QString &dir, bool use_sub_dir )
    {
        IcDirFileEnumerator_Worker_Creator *wkr = new IcDirFileEnumerator_Worker_Creator( );
        if ( wkr->thread() != QThread::currentThread()) {
            QMetaObject::invokeMethod(
                wkr, "createWorker", Qt::BlockingQueuedConnection,
                Q_ARG( const QStringList &, fn_pt ), Q_ARG( const QString &, dir ),
                Q_ARG( bool, use_sub_dir )
            );
        } else {
            wkr->createWorker( fn_pt, dir, use_sub_dir );
        }
        return wkr;
    }

    // delete the creator
    static void  deleteCreator( IcDirFileEnumerator_Worker_Creator *wkr_cr )
    {
        wkr_cr->deleteWorker();
        wkr_cr->deleteLater();
    }
};


// //////////////////////////////////////////////////////
// wrapper API
// //////////////////////////////////////////////////////
// ======================================================
// ctor
// ======================================================
    IcDirFileEnumerator :: IcDirFileEnumerator (
        const QStringList &pattern, const QString &dir, bool enum_sub_dir,
        QObject *pa
    ) : QObject( pa )
{
    gRegInQt();

    IcDirFileEnumerator_Worker_Creator *wkr_cr =
        IcDirFileEnumerator_Worker_Creator::newCreator( pattern, dir, enum_sub_dir );

    QObject::connect(
        wkr_cr->worker_di(), SIGNAL(dataFound( const QVector<QString>&)),
        this, SIGNAL(dataFound( const QVector<QString>&))
    );
    QObject::connect(
        wkr_cr->worker_di(), SIGNAL(finished()),
        this, SIGNAL(finished())
    );
    m_obj = wkr_cr;
}

// ======================================================
// dtor
// ======================================================
    IcDirFileEnumerator :: ~IcDirFileEnumerator()
{
    if ( m_obj != Q_NULLPTR ) {
       QObject::disconnect( T_WorkerCreator( m_obj )->worker_di(), nullptr, nullptr, nullptr );
       IcDirFileEnumerator_Worker_Creator::deleteCreator( T_WorkerCreator( m_obj ) );
    }
}


}

#include "qxpack_ic_dirfileenumerator.moc"

#ifdef __GNUC__ // in GCC 5, close below warnings
#pragma GCC diagnostic pop
#endif

#endif
