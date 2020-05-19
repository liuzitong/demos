#ifndef QXPACK_IC_BYTESTREAM_CXX
#define QXPACK_IC_BYTESTREAM_CXX

#include "qxpack_ic_bytestream.hxx"
#include "qxpack_ic_memcntr.hxx"
#include <mutex> // C++11
#include <deque> // C++11
#include <cstdlib>
#include <cstring>
#include <memory>

namespace QxPack {

// /////////////////////////////////////////////
//
// ByteStream private object
//
// /////////////////////////////////////////////
#define T_PrivPtr( o )   (( IcByteStreamPriv *) o )
class QXPACK_IC_HIDDEN  IcByteStreamPriv {
private:
    std::mutex         m_locker;
    std::deque<char*>  m_data_list;
    int  m_block_size;
    int  m_total_bytes;
    bool m_use_locker;

protected:
    inline void  lock( )  { if ( m_use_locker ) { m_locker.lock(); }}
    inline void  unlock() { if ( m_use_locker ) { m_locker.unlock();}}
    inline bool  tryLock(){ return ( m_use_locker ? m_locker.try_lock() : true ); }
    inline bool  isBeSys(){ uint16_t d = 0x55aa; return ( (( uint8_t *)( &d ))[0] == 0x55 ); }

    int    ensureBlockIndex( int total_bytes )
    {
        int block_idx = ( total_bytes - 1 ) / m_block_size;
        if ( ( size_t )( block_idx ) >= m_data_list.size() ) { // etc. b_idx is 0, but no block ( size()==0 )
            char *ptr = ( char *) std::malloc( m_block_size );
            if ( ptr == nullptr ) { qxpack_ic_fatal("failed to allocate memory."); }
            m_data_list.push_back( ptr );
        }
        return block_idx;
    }

public :
    IcByteStreamPriv ( int block_size, bool use_mutex );
    ~IcByteStreamPriv( );
    void         clear( );
    IcByteArray  resultAndClear( );

#if defined( QXPACK_IC_QT5_ENABLED )
    QByteArray   resultAndClearQt5();
#endif

    void    append( uint8_t  c, bool use_lock );
    void    append( uint8_t *d, int len );
};

// ==============================================
// CTOR
// ==============================================
    IcByteStreamPriv :: IcByteStreamPriv ( int block_size, bool use_mutex )
{
    m_use_locker = use_mutex;
    m_block_size = ( block_size < 256 ? 256 : ( block_size + 255 ) / 256 * 256 );
    m_total_bytes = 0;
}

// ==============================================
// DTOR
// ==============================================
    IcByteStreamPriv :: ~IcByteStreamPriv ( )
{
    this->clear();
}

// ==============================================
// clear all allocated blocks
// ==============================================
void    IcByteStreamPriv :: clear()
{
    this->lock();
    while ( ! m_data_list.empty() ) {
        char *ptr = m_data_list.back();
        if ( ptr != nullptr ) { std::free( ptr ); }
        m_data_list.pop_back();
    }
    m_total_bytes = 0;
    this->unlock();
}

// ==============================================
// read all blocks, make them as IcByteArray for output
// NOTE: this function will free all blocks
// ==============================================
IcByteArray  IcByteStreamPriv :: resultAndClear( )
{
    this->lock();

    IcByteArray ba( m_total_bytes );
    char *data_ptr = ba.data();

    while ( ! m_data_list.empty()) {
        char *ptr = m_data_list.front();
        if ( ptr != nullptr && m_total_bytes > 0 ) {
            int cpy_size = ( m_total_bytes >= m_block_size ? m_block_size : m_total_bytes );
            std::memcpy( data_ptr, ptr, cpy_size );
            data_ptr      += cpy_size;
            m_total_bytes -= cpy_size;
            std::free ( ptr );
        }
        m_data_list.pop_front();
    }

    this->unlock();

    return ba;
}

#if defined( QXPACK_IC_QT5_ENABLED )
QByteArray    IcByteStreamPriv :: resultAndClearQt5()
{
    this->lock();

    QByteArray ba( m_total_bytes, 0 );
    char *data_ptr = ( char *) ba.constData();

    while ( ! m_data_list.empty()) {
        char *ptr = m_data_list.front();
        if ( ptr != nullptr && m_total_bytes > 0 ) {
            int cpy_size = ( m_total_bytes >= m_block_size ? m_block_size : m_total_bytes );
            std::memcpy( data_ptr, ptr, cpy_size );
            data_ptr      += cpy_size;
            m_total_bytes -= cpy_size;
            std::free ( ptr );
        }
        m_data_list.pop_front();
    }

    this->unlock();
    return ba;
}
#endif

// =================================================
// append the character into stream
// =================================================
void   IcByteStreamPriv :: append( uint8_t c, bool use_lock )
{
    if ( use_lock ) { this->lock(); }

    ++ m_total_bytes;
    this->ensureBlockIndex( m_total_bytes );
    int block_ofv = ( m_total_bytes - 1 ) % m_block_size;
    char *ptr = m_data_list.back();
    ptr[ block_ofv ] = c;

    if ( use_lock ) { this->unlock(); }
}
void    IcByteStreamPriv :: append( uint8_t *data, int len )
{
    this->lock();

    while ( len -- > 0) {
        ++ m_total_bytes;
        this->ensureBlockIndex( m_total_bytes );
        int block_ofv = ( m_total_bytes - 1 ) % m_block_size;
        char *ptr = m_data_list.back();
        ptr[ block_ofv ] = *data ++;
    }

    this->unlock();
}


// /////////////////////////////////////////////
//
// ByteStream wrapper API
//
// /////////////////////////////////////////////
// =============================================
// CTOR
// =============================================
    IcByteStream :: IcByteStream ( int b_sz, bool use_mutex )
{
    m_obj = qxpack_ic_new( IcByteStreamPriv, b_sz, use_mutex );
}

// ==============================================
// DTOR
// ==============================================
    IcByteStream :: ~IcByteStream ( )
{
    if ( m_obj != nullptr ) {
        qxpack_ic_delete( m_obj, IcByteStreamPriv );
    }
}

// ==============================================
// clear all elements in the stream object
// ==============================================
void   IcByteStream :: clear( )
{
    T_PrivPtr( m_obj )->clear();
}

// ==============================================
// append a character into stream object
// ==============================================
IcByteStream &  IcByteStream :: append( uint8_t c )
{
    T_PrivPtr( m_obj )->append( c, true );
    return *this;
}

// ==============================================
// append a bytes array into stream object
// ==============================================
IcByteStream &  IcByteStream :: append( uint8_t  *data, int len )
{
    T_PrivPtr( m_obj )->append( data, len );
    return *this;
}

// ==============================================
// read the stream out, and clear it
// ==============================================
IcByteArray     IcByteStream :: resultAndClear( )
{
    return T_PrivPtr( m_obj )->resultAndClear();
}

#if defined( QXPACK_IC_QT5_ENABLED )
QByteArray      IcByteStream :: resultAndClearQt5()
{
    return T_PrivPtr( m_obj )->resultAndClearQt5();
}
#endif

}

#endif
