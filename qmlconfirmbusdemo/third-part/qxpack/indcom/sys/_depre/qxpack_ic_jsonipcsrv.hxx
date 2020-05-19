#ifndef QXPACK_IC_JSONIPCSRV_HXX
#define QXPACK_IC_JSONIPCSRV_HXX
  
#include <QObject>
#include <QString>
#include <qxpack/indcom/common/qxpack_ic_def.h>

#ifndef QXPACK_IC_API
#define QXPACK_IC_API
#endif

#ifndef QXPACK_IC_HIDDEN
#define QXPACK_IC_HIDDEN
#endif

namespace QxPack {

class IcJsonIpcHandler;
class IcJsonIpcSrv;
class IcJsonRpc2Request;
class IcJsonRpc2Result;
class IcJsonRpc2Error;

// ///////////////////////////////////////////
//
// Json IPC handler
//
// ///////////////////////////////////////////
class  QXPACK_IC_API IcJsonIpcHandler : public QObject {
    Q_OBJECT
public:
    virtual ~IcJsonIpcHandler( );

    bool  postResult( const IcJsonRpc2Result & );
    bool  postError ( const IcJsonRpc2Error  & );

    Q_SIGNAL void  newRequest ( const QxPack::IcJsonRpc2Request & );
    Q_SIGNAL void  ipcClosed  ( ); // the IPC channel closed

protected:
    IcJsonIpcHandler ( void *, QObject *pa = Q_NULLPTR );

friend class IcJsonIpcSrv;
friend class IcJsonIpcSrvPriv;
private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonIpcHandler )
};


// ///////////////////////////////////////////
//
// Json IPC server
//
// ///////////////////////////////////////////
class QXPACK_IC_API IcJsonIpcSrv : public QObject {
    Q_OBJECT
public:
    IcJsonIpcSrv ( const QString &ipc_name, int max_conn = 30 );
    virtual ~IcJsonIpcSrv( );

    bool   start( );
    void   stop ( );

    // the handler is in the inner.
    // delete it will remove it from inner list
    Q_SIGNAL void  newConnection( QxPack::IcJsonIpcHandler* );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonIpcSrv )
};

}

#endif
