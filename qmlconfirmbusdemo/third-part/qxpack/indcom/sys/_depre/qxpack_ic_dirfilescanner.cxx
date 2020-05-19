#ifndef QXPACK_IC_DIRFILESCANNER_CXX
#define QXPACK_IC_DIRFILESCANNER_CXX

#include "qxpack_ic_dirfilescanner.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <qxpack/indcom/sys/qxpack_ic_filemon.hxx>
#include <qxpack/indcom/sys/qxpack_ic_ncstring.hxx>
#include <qxpack/indcom/sys/qxpack_ic_jotaskworker.hxx>
#include <qxpack/indcom/sys/qxpack_ic_vardata.hxx>
#include <qxpack/indcom/sys/qxpack_ic_namedqthread_priv.hxx>
#include <qxpack/indcom/sys/qxpack_ic_workercreatortemp_priv.hpp>
#include <qxpack/indcom/sys/qxpack_ic_sysevtqthread_priv.hxx>
#include <QMap>
#include <QAtomicInt>
#include <QJsonObject>
#include <QReadWriteLock>
#include <QVector>
#include <QMetaObject>

namespace QxPack {

// ///////////////////////////////////////////////////////////////
// register in meta system
// ///////////////////////////////////////////////////////////////
static bool  g_is_reg = false;
static void  gRegInQt ( )
{
    if ( ! g_is_reg ) {
        qRegisterMetaType<QVector<QxPack::IcVarData>>("QVector<QxPack::IcVarData>" );
        qRegisterMetaType<QVector<QxPack::IcVarData>>("QVector<QxPack::IcVarData>&");
        g_is_reg = true;
    }
}

// //////////////////////////////////////////////////////////////
// private worker
// //////////////////////////////////////////////////////////////
#define T_PrivWorker( o )  (( IcDirFileScanner_Worker *) o )
class QXPACK_IC_HIDDEN IcDirFileScanner_Worker : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY( IcDirFileScanner_Worker )
private:
    QMap<IcNcString,IcVarData> m_var_data_map;
    IcJoTaskWorker  m_jo_worker;
    IcFileMon       m_file_mon;
    IcDirFileHandlerBase *m_ldr;
    IcDirFileHandlerBase::Factory  m_ldr_fact;
    int             m_work_path_tag; 
protected:
    Q_SLOT void  fileMon_onFileRemoved  ( const QStringList & );
    Q_SLOT void  fileMon_onFileModified ( const QStringList & );
    Q_SLOT void  fileMon_onFileAdded    ( const QStringList & );
    Q_SLOT void  onDataLoadedPreProc    ( QxPack::IcVarData & );

    Q_SIGNAL void  dataLoadedPreProc( QxPack::IcVarData & );

    IcVarData    newVarData    ( const QString &fn );
    IcVarData    deteteVarData ( const QString &fn );
    IcVarData    findVarData   ( const QString &fn );

    static void  joTaskHandler ( void *ctxt, const QJsonObject &jo, const IcVarData&, IcJoTaskWorker::HandlerType );

public :
    IcDirFileScanner_Worker ( const QStringList &pattern, IcDirFileHandlerBase::Factory );
    virtual ~IcDirFileScanner_Worker( );

    inline QString  workPath  ( ) const { return m_file_mon.workPath(); }
    inline IcJoTaskWorker &  joTaskWorker( ) { return m_jo_worker; }

    Q_SLOT  void  postToWorker ( const QxPack::IcVarData & );
    Q_SLOT  void  setWorkPath  ( const QString &wk );

    Q_SIGNAL void  dataLoaded ( const QxPack::IcVarData & );
    Q_SIGNAL void  dataLoadFailed( const QxPack::IcVarData & );
    Q_SIGNAL void  dataAdded  ( const QVector<QxPack::IcVarData> & );
    Q_SIGNAL void  dataRemoved( const QVector<QxPack::IcVarData> & );
    Q_SIGNAL void  workPathChanged( const QString & );
};

// ===================================================
// ctor
// ===================================================
    IcDirFileScanner_Worker :: IcDirFileScanner_Worker ( 
        const QStringList &pattern, IcDirFileHandlerBase::Factory ldr_fact
    )
    : m_file_mon( pattern )
{
    m_work_path_tag = m_file_mon.setWorkPath(QString());
    m_ldr = Q_NULLPTR;
    m_ldr_fact = ldr_fact;
    if ( ldr_fact != Q_NULLPTR ) {
        if ( ! (* ldr_fact )( QStringLiteral("createLoader"), & m_ldr )) {
            m_ldr = Q_NULLPTR;
        }
    }

    QObject::connect( this, SIGNAL(dataLoadedPreProc( QxPack::IcVarData& )),  this, SLOT(onDataLoadedPreProc( QxPack::IcVarData&)));
    QObject::connect( & m_file_mon, SIGNAL(fileFirstFound(const QStringList &)), this, SLOT(fileMon_onFileAdded( const QStringList &)));
    QObject::connect( & m_file_mon, SIGNAL(fileAdded  ( const QStringList &)),  this, SLOT(fileMon_onFileAdded  ( const QStringList &)));
    QObject::connect( & m_file_mon, SIGNAL(fileRemoved( const QStringList &)),  this, SLOT(fileMon_onFileRemoved( const QStringList &)));
    QObject::connect( & m_file_mon, SIGNAL(fileModified( const QStringList &)), this, SLOT(fileMon_onFileModified( const QStringList &)));

    qxpack_ic_incr_new_cntr(1);
}

// ===================================================
// dtor
// ===================================================
    IcDirFileScanner_Worker :: ~IcDirFileScanner_Worker ( )
{
    // disable signals and slots
    QObject::disconnect( this, nullptr, nullptr, nullptr );
    QObject::disconnect( & m_file_mon, nullptr, nullptr, nullptr );

    // disable file mon
    m_file_mon.setWorkPath( QString() );

    // drop all jobs, and wait for finished them
    m_jo_worker.shutdown();
    m_jo_worker.waitForAllDone();

    // free the loader
    if ( m_ldr_fact != Q_NULLPTR ) {
        if ( m_ldr != Q_NULLPTR ) {
            (* m_ldr_fact )( QStringLiteral("deleteLoader"), & m_ldr );
        }
    }

    qxpack_ic_decr_new_cntr(1);
}

// ==================================================
// set work path
// ==================================================
void      IcDirFileScanner_Worker :: setWorkPath ( const QString &wk )
{
    m_work_path_tag = m_file_mon.setWorkPath( wk );
    m_var_data_map = QMap<IcNcString,IcVarData>();
    emit this->workPathChanged( wk );
}

// ==================================================
// gen the new var data
// ==================================================
IcVarData  IcDirFileScanner_Worker :: newVarData( const QString &fn )
{
    IcVarData var;  // var auto-generate the id() -- depends on memory manager.
    var.setTag ( m_work_path_tag );
    var.setName( fn );
    m_var_data_map.insert ( var.name(), var ); // save it in map.
    return var;
}

// ==================================================
// drop the specified var data by name
// ==================================================
IcVarData  IcDirFileScanner_Worker :: deteteVarData ( const QString &fn )
{
    return ( ! fn.isEmpty() ? m_var_data_map.take( IcNcString(fn) ) : IcVarData());
}

// ===================================================
// find specified var data by name
// ===================================================
IcVarData  IcDirFileScanner_Worker :: findVarData ( const QString &fn )
{
    if ( fn.isEmpty()) { return IcVarData(); }
    QMap<IcNcString,IcVarData>::const_iterator itr = m_var_data_map.constFind( IcNcString(fn));
    return ( itr != m_var_data_map.constEnd() ? itr.value() : IcVarData() );
}

// =========================================================
// SLOT: the data is loaded, here process it and emit the parent
// =========================================================
void    IcDirFileScanner_Worker :: onDataLoadedPreProc( QxPack::IcVarData &var_data )
{
    // discard if the var data is not current tag
    if ( var_data.tag() != m_work_path_tag ) { return; }

    // replace it if existed.
    QMap<IcNcString,IcVarData>::iterator itr = m_var_data_map.find( var_data.name() );
    if ( itr != m_var_data_map.end()) {
        IcVarData &var = itr.value();
        var = var_data; // overwrite it
    }

    // emit again.
    emit this->dataLoaded( var_data );
}

// =========================================================
// post data to worker
// =========================================================
void  IcDirFileScanner_Worker :: postToWorker( const QxPack::IcVarData &org )
{
    m_jo_worker.post( & joTaskHandler, this, QJsonObject(), org );
}

// =========================================================
// the jo task handler [ static ]
// =========================================================
void  IcDirFileScanner_Worker :: joTaskHandler( void *ctxt, const QJsonObject &, const IcVarData &ext, IcJoTaskWorker::HandlerType h_type )
{
    // ---------------------------------------------------
    // Be carefull: below code is running in other thread
    // ---------------------------------------------------
    if ( ext.isNull() ) { return; }
    IcDirFileScanner_Worker *t_this = static_cast< IcDirFileScanner_Worker *>( ctxt );

    if ( t_this != Q_NULLPTR  &&  h_type == IcJoTaskWorker::HandlerType_Routine  &&  t_this->m_ldr != Q_NULLPTR ) {

        IcVarData load_var = t_this->m_ldr->handle(
            ext.name().section( QChar('.'), -1, -1 ),
            QString("%1/%2").arg( t_this->workPath()).arg( ext.name())
        );
        if ( ! load_var.isNull()) {
            load_var.setId  ( ext.id() ); // overwrite the id
            load_var.setTag ( ext.tag());
            load_var.setName( ext.name());
            emit t_this->dataLoadedPreProc( load_var );
        } else {
            emit t_this->dataLoadFailed( ext );
        }
    } else {
        // HandlerType is Drop or m_ldr is null.
    }
}

// ===================================================
// SLOT: handle the first found files
// ===================================================
void   IcDirFileScanner_Worker :: fileMon_onFileAdded( const QStringList &fn_list )
{
    QVector<IcVarData>  var_list;
    QStringList::const_iterator c_itr = fn_list.cbegin();
    while ( c_itr != fn_list.cend()) {
        var_list.append( this->newVarData( *c_itr ++ ));
    }
    if ( var_list.size() > 0 ) { emit this->dataAdded( var_list ); }
}

// ===================================================
// SLOT: handle the file removed
// ===================================================
void  IcDirFileScanner_Worker :: fileMon_onFileRemoved( const QStringList &fn_list )
{
    QVector<IcVarData> var_list;
    QStringList::const_iterator c_itr = fn_list.cbegin();
    while ( c_itr != fn_list.cend()) {
        IcVarData var = this->deteteVarData( *c_itr ++ );
        if ( ! var.isNull()) { var_list.append( var ); }
        var = IcVarData();
    }
    if ( var_list.size() > 0 ) { emit this->dataRemoved( var_list ); }
}

// ===================================================
// SLOT: handle the file modified
// ===================================================
void  IcDirFileScanner_Worker :: fileMon_onFileModified( const QStringList &fn_list )
{
    QStringList::const_iterator c_itr = fn_list.cbegin();
    while ( c_itr != fn_list.cend()) {
        IcVarData var = this->findVarData( *c_itr ++ );
        if ( var.isNull()) { continue; }
        this->postToWorker( var );
        var = IcVarData();
    }
}

// ///////////////////////////////////////////////////////////
//
// worker creator ( live in sys_evt_thread )
//
// ///////////////////////////////////////////////////////////
#define T_WorkerCreator( o )  T_ObjCast( IcDirFileScanner_Worker_Creator *, o )
class QXPACK_IC_HIDDEN IcDirFileScanner_Worker_Creator : public QObject, public IcWorkerCreatorTemp< IcDirFileScanner_Worker > {
    Q_OBJECT
    Q_DISABLE_COPY( IcDirFileScanner_Worker_Creator )
private: IcSysEvtQThreadPriv m_sys_evt_thread;
public :
    IcDirFileScanner_Worker_Creator ( )
    { this->moveToThread( m_sys_evt_thread.thread()); }

    virtual ~IcDirFileScanner_Worker_Creator( ) { }

    Q_INVOKABLE void createWorker ( const QStringList &fn_pt, void* ldr_fact )
    {
        this->deleteWorker();
        m_wkr = new IcDirFileScanner_Worker( fn_pt, reinterpret_cast< IcDirFileHandlerBase::Factory >( ldr_fact ));
    }

    static IcDirFileScanner_Worker_Creator*  newCreator( const QStringList &fn_pattern, IcDirFileHandlerBase::Factory ldr_fact )
    {
        IcDirFileScanner_Worker_Creator *wkr = new IcDirFileScanner_Worker_Creator( );
        if ( wkr->thread() != QThread::currentThread()) {
            QMetaObject::invokeMethod(
                wkr, "createWorker", Qt::BlockingQueuedConnection,
                Q_ARG( const QStringList &, fn_pattern ),
                Q_ARG( void*, T_PtrCast(void*, ldr_fact ))
            );
        } else {
            wkr->createWorker( fn_pattern, reinterpret_cast<void*>( ldr_fact ));
        }
        return wkr;
    }

    static void  deleteCreator( IcDirFileScanner_Worker_Creator *wkr_cr )
    {
        wkr_cr->deleteWorker();
        wkr_cr->deleteLater();
    }
};

// ///////////////////////////////////////////////////////////
//
// wrapper API of IcDirFileScanner
//
// ///////////////////////////////////////////////////////////
// ==========================================================
// CTOR
// ==========================================================
    IcDirFileScanner :: IcDirFileScanner (
         const QStringList &pattern, IcDirFileHandlerBase::Factory ldr_fact, QObject *pa
    ) : QObject( pa )
{
    gRegInQt();

    //1) create a worker
    IcDirFileScanner_Worker_Creator *wkr_creator =
        IcDirFileScanner_Worker_Creator::newCreator( pattern, ldr_fact );

    //2) connect signals
    QObject::connect(
        wkr_creator->worker_di(), SIGNAL(dataAdded( const QVector<QxPack::IcVarData>& )),
        this, SIGNAL(dataAdded( const QVector<QxPack::IcVarData>&))
    );
    QObject::connect(
        wkr_creator->worker_di(), SIGNAL(dataRemoved( const QVector<QxPack::IcVarData>&)),
        this, SIGNAL(dataRemoved( const QVector<QxPack::IcVarData> & ))
    );
    QObject::connect(
        wkr_creator->worker_di(), SIGNAL(dataLoaded( const QxPack::IcVarData &)),
        this, SIGNAL(dataLoaded( const QxPack::IcVarData &))
    );
    QObject::connect(
        wkr_creator->worker_di(), SIGNAL(workPathChanged( const QString &)),
        this, SIGNAL(workPathChanged( const QString &))
    );

    //3) remember worker
    m_obj = wkr_creator;
}

// ==========================================================
// DTOR
// ==========================================================
    IcDirFileScanner :: ~IcDirFileScanner ( )
{
    if ( m_obj != Q_NULLPTR ) {
        QObject::disconnect( T_WorkerCreator( m_obj )->worker_di(), Q_NULLPTR, Q_NULLPTR, Q_NULLPTR );
        IcDirFileScanner_Worker_Creator::deleteCreator( T_WorkerCreator( m_obj ));
    }
}

// ===========================================================
// wrap API
// ===========================================================
void IcDirFileScanner :: setWorkPath( const QString &fp )
{
    IcDirFileScanner_Worker_Creator *wkr_cr = T_WorkerCreator( m_obj );
    if ( wkr_cr->worker_di()->thread() != QThread::currentThread()) {
        bool call_ret = QMetaObject::invokeMethod(
            wkr_cr->worker_di(), "setWorkPath", Qt::BlockingQueuedConnection,
            Q_ARG( const QString&, fp )
        );
        Q_UNUSED( call_ret );
    } else {
        wkr_cr->worker_di()->setWorkPath( fp );
    }
}

QString  IcDirFileScanner :: workPath( ) const
{
    QString wkp;
    IcDirFileScanner_Worker_Creator *wkr_cr = T_WorkerCreator( m_obj );
    if ( wkr_cr->worker_di()->thread() != QThread::currentThread()) {
        bool call_ret = QMetaObject::invokeMethod(
            wkr_cr->worker_di(), "workPath", Qt::BlockingQueuedConnection,
            Q_RETURN_ARG( QString, wkp )
        );
        Q_UNUSED( call_ret );
    } else {
        wkp = wkr_cr->worker_di()->workPath();
    }
    return wkp;
}

void     IcDirFileScanner :: onReqLoadData ( const QxPack::IcVarData &var )
{
    if ( var.isNull()) { return; }

    IcDirFileScanner_Worker_Creator *wkr_cr = T_WorkerCreator( m_obj );
    if ( wkr_cr->worker_di()->thread() != QThread::currentThread()) {
        bool call_ret = QMetaObject::invokeMethod(
            wkr_cr->worker_di(), "postToWorker", Qt::QueuedConnection,
            Q_ARG( const QxPack::IcVarData &, var )
        );
        Q_UNUSED( call_ret );
    } else {
        wkr_cr->worker_di()->postToWorker( var );
    }
}


}

#include "qxpack_ic_dirfilescanner.moc"
#endif
