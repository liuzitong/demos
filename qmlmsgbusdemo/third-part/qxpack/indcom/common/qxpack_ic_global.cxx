#ifndef QXPACK_IC_GLOBAL_CXX
#define QXPACK_IC_GLOBAL_CXX

#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <qxpack/indcom/common/qxpack_ic_dyncinit_priv.hxx>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace QxPack {

// ============================================================================
// guess the separator character
// ============================================================================
QXPACK_IC_API char  guessPathSeparator( const char *path )
{
    if ( path == nullptr ) { return 0; }
    while ( *path != 0 ) {
        if ( *path == '\\') { return '\\'; }
        if ( *path == '/' ) { return '/';  }
        ++ path;
    }
    return 0;
}

// ============================================================================
// trip the path string as short string by separator
// ============================================================================
QXPACK_IC_API void tailPath (
    const char *src, char *dst, size_t max_sz,  size_t sepa_num
) {
    // ------------------------------------------------------------------------
    // cehck param. and setup the last byte to zero.
    // ------------------------------------------------------------------------
    if ( max_sz < 1 || src == nullptr || dst == nullptr ) { return; }
    dst[ -- max_sz ] = 0;
    if ( sepa_num == 0 ) { sepa_num = size_t( -1 ); }

    // ------------------------------------------------------------------------
    // skip separator until find the last separator
    // ------------------------------------------------------------------------
    size_t src_len  = std::strlen( src );
    size_t read_cnt = 0;
    size_t sepa_cnt = 0;
    char   sepa = guessPathSeparator( src );
    const char *sp = src + src_len;
    while ( read_cnt < max_sz  &&  sepa_cnt < sepa_num  &&  sp >= src ) {
        -- sp; ++ read_cnt;
        if ( *sp == sepa ) { ++ sepa_cnt; }
    }

    // ------------------------------------------------------------------------
    // copy the data into dest. buffer
    // ------------------------------------------------------------------------
    if ( sp == src ) {
        std::memcpy( dst, sp, read_cnt );
    } else {
        if ( max_sz - read_cnt > 1 ) { // space is enough to add '..' string
            dst[0] = dst[1] = '.';
            dst += 2; max_sz -= 2;
            std::memcpy( dst, sp, read_cnt );
        } else {
            std::memcpy( dst, sp, read_cnt );
            if ( max_sz > 1 ) {
                dst[0] = dst[1] = '.';
            }
        }
    }
    dst[ read_cnt ] = 0;
}



}

#endif
