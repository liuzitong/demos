// ////////////////////////////////////////////////////////////////////////////
/*!
 * @file qxpack_ic_jsonrpc2_buff
 * @author nightwing
 * @date 2019/03, 2019/05
 * @verbatim
 *     <author>    <date>    <history memo.>
 *     nightwing  2019/03     build
 *     nightwing  2019/05     fixed.
 * @endverbatim
 */
// ////////////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_JSONRPC2_BUFF_CXX
#define QXPACK_IC_JSONRPC2_BUFF_CXX

#include "qxpack_ic_jsonrpc2_buff.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_numstatqueuetemp.hpp"

#include <QList>
#include <QString>
#include <QTimer>
#include <QList>
#include <QByteArray>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
//                          recv. buffer
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2PkgReadBuff :: IcJsonRpc2PkgReadBuff (
    QSharedPointer<QIODevice> &dev, QObject *pa
) : IcDataTransPkgReadBuff( dev, pa )
{
    m_obj = Q_NULLPTR;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2PkgReadBuff :: ~IcJsonRpc2PkgReadBuff ( )
{
    m_obj = Q_NULLPTR;
}

// ============================================================================
// analysis the data, find the json text
// [default] we use '\r\n\r\n' as stream terminal character
// ============================================================================
bool  IcJsonRpc2PkgReadBuff :: analyData( const QByteArray &ba, AnalyInfo &ai )
{
    int pos = ba.indexOf( "\r\n\r\n", 0 );
    if ( pos != -1 ) {
        ai.m_cb_size = sizeof( AnalyInfo );
        ai.m_sect_ofv  = 0;
        ai.m_sect_size = pos + 4; // [0,pos+4) is the range size..
        ai.m_data_ofv_in_sect = 0;
        ai.m_data_size = pos;     // [0,pos) is the actual data.
        return true;
    } else {
        ai.m_cb_size = 0;
        return false;
    }
}





// ////////////////////////////////////////////////////////////////////////////
//
//                                send queue
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonRpc2PkgWriteBuff :: IcJsonRpc2PkgWriteBuff (
    QSharedPointer<QIODevice> &dev, QObject *pa
) : IcDataTransPkgWriteBuff( dev, pa )
{
    m_obj = new QByteArray("\r\n\r\n"); // create a default data
}

// ============================================================================
// dtor
// ============================================================================
IcJsonRpc2PkgWriteBuff :: ~IcJsonRpc2PkgWriteBuff ( )
{
    delete reinterpret_cast<QByteArray*>( m_obj );
}

// ============================================================================
// append bytearray
// ============================================================================
bool     IcJsonRpc2PkgWriteBuff :: packData(
    const QByteArray &dat, QVector<QPair<bool, QByteArray> > &ba_list
) {
    ba_list.reserve(2);
    ba_list.append(QPair<bool,QByteArray>( true, dat ));
    ba_list.append(QPair<bool,QByteArray>( false, *( reinterpret_cast<QByteArray*>(m_obj))));

    return true;
}


// ////////////////////////////////////////////////////////////////////////////
//
//                  default factory
//
// ////////////////////////////////////////////////////////////////////////////
// ===========================================================================
// ctor
// ===========================================================================
IcJsonRpc2PkgDefaultFact :: IcJsonRpc2PkgDefaultFact ( IcDataTransFactory e_fact, void *e_ctxt )
{
    m_ext_fact = e_fact; m_ext_ctxt = e_ctxt;
}

// ===========================================================================
// dtor
// ===========================================================================
IcJsonRpc2PkgDefaultFact :: ~IcJsonRpc2PkgDefaultFact( )
{
}

// ===========================================================================
// do the factory
// ===========================================================================
QVariant  IcJsonRpc2PkgDefaultFact :: doFact( const QString &opr, const QVariant &p )
{
    QVariant ret;
    if        ( opr == STR_IcDataTransFactory_createReader ) {
        if (  m_ext_fact != Q_NULLPTR ) {
            ret = m_ext_fact( m_ext_ctxt, opr, p );
        }
        if ( ! ret.isValid()) {
            QSharedPointer<QIODevice> io_dev = p.value<QSharedPointer<QIODevice>>();
            if ( io_dev == Q_NULLPTR ) { qFatal("IO Device pointer is null!"); }
            ret = QVariant::fromValue( qobject_cast<QObject*>( new IcJsonRpc2PkgReadBuff( io_dev )));
        }

    } else if ( opr == STR_IcDataTransFactory_createWriter ) {
        if ( m_ext_fact != Q_NULLPTR ) {
            ret = m_ext_fact( m_ext_ctxt, opr, p );
        }
        if ( ! ret.isValid() ) {
            QSharedPointer<QIODevice> io_dev = p.value<QSharedPointer<QIODevice>>();
            if ( io_dev == Q_NULLPTR ) { qFatal("IO Device pointer is null!"); }
            ret = QVariant::fromValue( qobject_cast<QObject*>( new IcJsonRpc2PkgWriteBuff( io_dev )));
        }

    } else {
        ret = m_ext_fact( m_ext_ctxt, opr, p );
    }

    return ret;
}

// ===========================================================================
// [ static ] default factory function
// ===========================================================================
QVariant  IcJsonRpc2PkgDefaultFact :: factory( void *c, const QString &o, const QVariant &p )
{
    IcJsonRpc2PkgDefaultFact *t_this = reinterpret_cast<IcJsonRpc2PkgDefaultFact*>(c);
    if ( t_this != Q_NULLPTR ) {
       return t_this->doFact( o, p );
    } else {
       return QVariant();
    }
}

}

#endif
