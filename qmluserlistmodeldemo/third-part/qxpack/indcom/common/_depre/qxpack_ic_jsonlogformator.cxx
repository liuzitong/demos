#ifndef QXPACK_IC_JSONLOGFORMATOR_CXX
#define QXPACK_IC_JSONLOGFORMATOR_CXX

#include "qxpack_ic_jsonlogformator.hxx"
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include "qxpack/indcom/common/qxpack_ic_bytearray.hxx"
#include <chrono> // C++11

#define NO_LIMIT INT_MAX
#define NO_SEPA  0

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
// local function
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// inner structure
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcJsonLogFormatorPriv*, o )
class QXPACK_IC_HIDDEN IcJsonLogFormatorPriv {
private:
    IcByteArray m_rsl, m_msg_tmp, m_fp_tmp, m_fn_tmp;
    size_t  m_fp_limit_sz, m_fn_limit_sz, m_msg_limit_sz, m_fp_sepa_num;
public :
    IcJsonLogFormatorPriv ( );
    ~IcJsonLogFormatorPriv( );
    inline size_t  fpLimitSz( ) const { return m_fp_limit_sz; }
    inline size_t  fnLimitSz( ) const { return m_fn_limit_sz; }
    inline size_t  msgLimitSz( )const { return m_msg_limit_sz; }
    inline size_t  fpSepaNum( ) const { return m_fp_sepa_num; }
    inline size_t  ctxtSz( )    const { return m_ctxt_sz; }
    inline char*   buffer( )    const { return m_buff;    }
    inline size_t  buffSz( )    const { return m_buff_sz; }

    inline void    setFpLimitSz ( size_t n ) { m_fp_limit_sz = n; }
    inline void    setFnLimitSz ( size_t n ) { m_fn_limit_sz = n; }
    inline void    setMsgLimitSz( size_t n ) { m_msg_limit_sz = n; }
    inline void    setFpSepaNum ( size_t n ) { m_fp_sepa_num = n; }

    void  format(  );
};

// ============================================================================
// ctor
// ============================================================================
IcJsonLogFormatorPriv :: IcJsonLogFormatorPriv ( )
    : m_fp_limit_sz( NO_LIMIT ), m_fn_limit_sz( NO_LIMIT ), m_msg_limit_sz( NO_LIMIT ),
      m_fp_sepa_num( NO_LIMIT )
{
}

// ============================================================================
// dtor
// ============================================================================
IcJsonLogFormatorPriv :: ~IcJsonLogFormatorPriv ( )
{

}


// ============================================================================
// write bracket to buffer
// ============================================================================
void  IcJsonLogFormatorPriv :: writeBracket ( bool is_left )
{

}




// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcJsonLogFormator :: IcJsonLogFormator ( )
{
    m_obj = new IcJsonLogFormatorPriv;
}

// ============================================================================
// dtor
// ============================================================================
IcJsonLogFormator :: ~IcJsonLogFormator ( )
{
    delete T_PrivPtr( m_obj );
}

// ============================================================================
// method access
// ============================================================================
size_t  IcJsonLogFormator :: filePathSepaNum  ( ) const
{  return T_PrivPtr( m_obj )->fpSepaNum(); }

size_t  IcJsonLogFormator :: filePathLimitSize( ) const
{  return T_PrivPtr( m_obj )->fpLimitSz(); }

size_t  IcJsonLogFormator :: funcNameLimitSize( ) const
{  return T_PrivPtr( m_obj )->fnLimitSz();  }

size_t  IcJsonLogFormator :: messageLimitSize ( ) const
{  return T_PrivPtr( m_obj )->msgLimitSz(); }

// ============================================================================
// configurate the params
// ============================================================================
void  IcJsonLogFormator :: config(
  size_t fp_sepa_num, size_t fp_limit_sz, size_t fn_limit_sz, size_t msg_limit_sz
) {
    T_PrivPtr( m_obj )->setFpSepaNum( fp_sepa_num );
    T_PrivPtr( m_obj )->setFpLimitSz( fp_limit_sz );
    T_PrivPtr( m_obj )->setFnLimitSz( fn_limit_sz );
    T_PrivPtr( m_obj )->setMsgLimitSz( msg_limit_sz );
}

// ============================================================================
// return the buffer inner data pointer
// ============================================================================
const char* IcJsonLogFormator :: result( ) const
{  return T_PrivPtr( m_obj )->buffer(); }

// ============================================================================
// return the context size after last format() called
// ============================================================================
size_t      IcJsonLogFormator :: size( ) const
{  return T_PrivPtr( m_obj )->ctxtSz(); }

// ============================================================================
// format the message
// ============================================================================
static uint64_t gTimeStamp( )
{
    std::chrono::steady_clock::time_point tp =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(0);
    return uint64_t( tp.time_since_epoch().count());
}

void   IcJsonLogFormator :: format (
  MsgTypeID  msg_type, const char *msg, const char *file_path,
  const char *func_name, size_t  line_num
) {
    // ------------------------------------------------------------------------
    // estimate a size
    // string character translate in json:
    // \\ -- "\", \" -- """, \/ -- "/", \b -- backspace, \f -- formfeed (0x0c )
    // \n -- new line, \r -- carriage return, \t -- hori.tab, \u -- hex digits ( unicode )
    // ------------------------------------------------------------------------
    size_t msg_sz = std::strlen( msg       );
    size_t fp_sz  = std::strlen( file_path );
    size_t fn_sz  = std::strlen( func_name );



}









}

#endif
