#ifndef QXPACK_IC_JSONIPCCLI_HXX
#define QXPACK_IC_JSONIPCCLI_HXX

#include <QObject>
#include <QString>
#include <QByteArray>
#include <qxpack/indcom/common/qxpack_ic_def.h>

#ifndef QXPACK_IC_API
#define QXPACK_IC_API
#endif

#ifndef QXPACK_IC_HIDDEN
#define QXPACK_IC_HIDDEN
#endif

namespace QxPack {

class IcJsonRpc2Request;
class IcJsonRpc2Result;
class IcJsonRpc2Error;

// ////////////////////////////////////////////////////
//
// Json Ipc client
//
// ////////////////////////////////////////////////////
class QXPACK_IC_API  IcJsonIpcCli : public QObject {
    Q_OBJECT
public:
    IcJsonIpcCli ( const QString &name );
    virtual ~IcJsonIpcCli( );

    bool  waitForConnected( int msec );
    bool  isWorked( ) const;
    bool  postRequest ( const IcJsonRpc2Request & );

    Q_SIGNAL void  newResult( const QxPack::IcJsonRpc2Result & );
    Q_SIGNAL void  newError ( const QxPack::IcJsonRpc2Error  & );
    Q_SIGNAL void  ipcClosed( );

private:
    void *m_obj;
    Q_DISABLE_COPY( IcJsonIpcCli )
};



}

#endif
