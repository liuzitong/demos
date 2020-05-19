#ifndef QXPACK_IC_SIZE_CXX
#define QXPACK_IC_SIZE_CXX

#include "qxpack_ic_size.hxx"
#include <stdint.h>

namespace QxPack {

// //////////////////////////////////////////////
// IcSize implement
// //////////////////////////////////////////////
// ==============================================
// scaled the rectangle by mode
// ==============================================
IcSize  IcSize :: scaled( const IcSize &s, AspectRatioMode mode )
{
    if ( mode == IgnoreAspectRatio || wd == 0 || ht == 0) {
        return s;

    } else {
        bool useHeight;
        int64_t rw = int64_t(s.ht) * int64_t(wd) / int64_t(ht);

        if ( mode == KeepAspectRatio) {
            useHeight = ( rw <= s.wd );
        } else { // mode == KeepAspectRatioByExpanding
            useHeight = ( rw >= s.wd );
        }

        if ( useHeight ) {
            return IcSize( ( int ) rw, s.ht );
        } else {
            return IcSize(
                ( int ) rw, int( int64_t(s.wd) * int64_t(ht) / int64_t(wd))
            );
        }
    }
}

// /////////////////////////////////////////////
// IcSizeF implement
// /////////////////////////////////////////////
// ==============================================
// scaled the rectangle by mode
// ==============================================
IcSizeF   IcSizeF :: scaled( const IcSizeF &s, AspectRatioMode mode )
{
    if ( mode == IgnoreAspectRatio || QxPack::isNull(wd) || QxPack::isNull(ht)) {
        return s;

    } else {
        bool   useHeight;
        double rw = s.ht * wd / ht;

        if ( mode == KeepAspectRatio ) {
            useHeight = ( rw <= s.wd );
        } else { // mode == KeepAspectRatioByExpanding
            useHeight = ( rw >= s.wd );
        }

        if ( useHeight ) {
            return IcSizeF( float( rw ),   float( s.ht ));
        } else {
            return IcSizeF( float( s.wd ), float( s.wd * ht / wd ));
        }
    }
}

}

#endif
