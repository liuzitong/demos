#ifndef QXPACK_IC_GLOBAL_HXX
#define QXPACK_IC_GLOBAL_HXX

#include "qxpack/indcom/common/qxpack_ic_def.h"
#include <stdint.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4302 )
#endif

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
// utility macro and inline functions
// ////////////////////////////////////////////////////////////////////////////
template <typename T>
    inline const T& min( const T &a, const T &b ) { return ( a < b ? a : b ); }
template <typename T>
    inline const T& max( const T &a, const T &b ) { return ( a < b ? b : a ); }

// ============================================================================
//! return a value absolute value
// ============================================================================
template <typename T>
    inline T abs( const T &t) { return t >= 0 ? t : -t; }

// ============================================================================
//! round the double value into integer.
// ============================================================================
template <typename T>
    inline int    round( const T &d ) { return static_cast<int>(d); }
template <>
    inline int    round( const double &d )
    { return d >= 0.0 ? int( d + 0.5 ) : int(d - double(int(d-1)) + 0.5) + int(d-1); }
template <>
    inline int    round( const float & d )
    { return d >= 0.0f ? int(d + 0.5f) : int(d - float(int(d-1)) + 0.5f) + int(d-1); }

// ============================================================================
//! round double value into int64_t
// ============================================================================
template <typename T>
    inline int64_t  round64( const T &d ) { return static_cast<int64_t>(d); }
template <>
    inline int64_t  round64( const double &d )
    { return d >= 0.0 ? int64_t(d + 0.5) : int64_t( d - double( int64_t( d-1 )) + 0.5) + int64_t( d-1 ); }
template <>
    inline int64_t  round64( const float  &f )
    { return f >= 0.0f ? int64_t(f + 0.5f) : int64_t(f - float(int64_t(f-1)) + 0.5f) + int64_t(f-1); }


template <typename T>
    inline const T& bound( const T &min_v, const T &val, const T &max_v )
{  return max( min_v, min( max_v, val ));  }

template <typename T>
    inline bool    fuzzyIsNull( const T &) { return false; }
template <>
    inline bool    fuzzyIsNull( const double &d ) { return abs( d ) <= 0.000000000001; }
template <>
    inline bool    fuzzyIsNull( const float  &f ) { return abs( f ) <= 0.00001f;  }

template <typename T>
    inline bool    isNull( const T &d) { return ( d == 0 ); }
template <>
    inline bool    isNull( const double &d )
{
    union U { double d; int64_t i; }; U val; val.d = d;
    return ( val.i & 0x7fffffffffffffffLL ) == 0;
}
template <>
    inline bool    isNull( const float &f )
{
    union U { float f; int32_t i; }; U val; val.f = f;
    return ( val.i & 0x7fffffffL ) == 0;
}

template <typename T>
    inline bool    fuzzyCompare( const T &, const T & ) { return false; }
template <>
    inline bool    fuzzyCompare( const double &p1, const double & p2 )
{  return (abs(p1 - p2) * 1000000000000. <= min(abs(p1), abs(p2)));  }
template <>
   inline bool     fuzzyCompare( const float  &p1, const float  & p2 )
{  return (abs(p1 - p2) * 100000.f <= min(abs(p1), abs(p2))); } 


//! guess the separator in the path string
//! @details this routine will try to find '\' or '/' character in the path string
//! @note if no separator, return 0
QXPACK_IC_API  char  guessPathSeparator( const char *path );

//! trip path string as short path string by separator.
//! @details this function trip the path string from end to start.
//! @param src_path [in]  the source path string
//! @param dst      [out] target buffer, used for store the tripped string.
//! @param dst_size [in]  the dest. buffer size
//! @param sepa_num [in]  the separator number
//! @note  function will guess the separator
QXPACK_IC_API void tailPath (
    const char *src_path, char *dst, size_t dst_size, size_t sepa_num
);

//! simple LCG, generate the random value.
class QXPACK_IC_HIDDEN IcLCG {
private: uint32_t m_seed;
public :
    IcLCG( ) { m_seed = reinterpret_cast<uint32_t>(this); }
    IcLCG( uint32_t s ) { m_seed = s; }
    inline uint16_t value( )
    {
        m_seed = static_cast<uint32_t>( m_seed * 214013 + 2531011 );
        return static_cast<uint16_t>( m_seed >> 16 );
    }
};

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
