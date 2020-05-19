#ifndef QXPACK_IC_BYTESTREAM_HXX
#define QXPACK_IC_BYTESTREAM_HXX

#include "qxpack_ic_def.h"
#include "qxpack_ic_bytearray.hxx"

#if defined( QXPACK_IC_QT5_ENABLED )
#include <QByteArray>
#endif

namespace QxPack {

// /////////////////////////////////////////////
//
// Byte Stream
//
// //////////////////////////////////////////////
class QXPACK_IC_API IcByteStream {
public:
    IcByteStream ( int block_size = 1024, bool use_mutex = true );
    virtual ~IcByteStream( );

    void   clear( );

    IcByteStream &  append( uint8_t  c );
    IcByteStream &  append( uint8_t  *data, int len );
    IcByteArray     resultAndClear( );

#if defined( QXPACK_IC_QT5_ENABLED )
    QByteArray      resultAndClearQt5( );
#endif

private:
    IcByteStream( const IcByteStream & );
    IcByteStream & operator = ( const IcByteStream & );
    void *m_obj;
};


}
#endif
