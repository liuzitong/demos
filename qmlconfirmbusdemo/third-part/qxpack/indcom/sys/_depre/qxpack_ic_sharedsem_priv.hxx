#ifndef QXPACK_IC_SHAREDSEM_PRIV_HXX
#define QXPACK_IC_SHAREDSEM_PRIV_HXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <QObject>

namespace QxPack {

// /////////////////////////////////////////////////
/*!
 * @brief this class provide a shared semaphore object
 * @warning only used between 2 threads!
 */
// ////////////////////////////////////////////////
class QXPACK_IC_HIDDEN  IcSharedSemPriv {
public :
    IcSharedSemPriv ( );
    IcSharedSemPriv ( bool cr );
    IcSharedSemPriv ( const IcSharedSemPriv & );
    IcSharedSemPriv &  operator = ( const IcSharedSemPriv & );
    virtual ~IcSharedSemPriv( );

    void  release   ( int n = 1 );

    void  acquire   ( int n = 1 );
    int   available ( ) const;
    bool  tryAcquire( int n = 1 );
    bool  tryAcquire( int n, int timeout );

private:
    void *m_obj;
};

}

Q_DECLARE_METATYPE( QxPack::IcSharedSemPriv )

#endif
