#ifndef QXPACK_IC_AL_GLOBAL_HPP
#define QXPACK_IC_AL_GLOBAL_HPP

#include "qxpack_ic_al_def.h"
#include <cmath>

namespace QxPack {

// ============================================================================
//! return the minimal value
// ============================================================================
template <typename T>
inline const T&  IcAl_min ( const T &a, const T &b ) { return ( a < b ? a : b ); }

// ============================================================================
//! return the maximum value
// ============================================================================
template <typename T>
inline const T&  IcAl_max ( const T &a, const T &b ) { return ( a > b ? a : b ); }

// ============================================================================
//! bound function, used to restrict the value range, like clamp..
// ============================================================================
template <typename T>
inline const T& IcAl_bound ( const T &min, const T &v, const T &max )
{ return IcAl_max( min, IcAl_min( max, v )); }

// ============================================================================
//! @brief calc. absoluate value
// ============================================================================
template <typename T>
inline T IcAl_abs( const T &t ) { return t >= 0 ? t : -t; }

// ============================================================================
//! real value fuzzy compare ( used for float, double )
// ============================================================================
template <typename T >
inline bool  IcAl_fuzzyCompare( const T &, const T & );

//! override 
template <>
inline bool  IcAl_fuzzyCompare( const double &p1, const double &p2 )
{
    return (
        IcAl_abs( p1 - p2 ) * 1000000000000. <= IcAl_min( IcAl_abs(p1), IcAl_abs(p2) )
    );
}

//! override
template <>
inline bool  IcAl_fuzzyCompare( const float &p1, const float &p2 )
{
    return ( IcAl_abs(p1 - p2) * 100000.f <= IcAl_min( IcAl_abs(p1), IcAl_abs(p2)));
}

// ============================================================================
//! check if the float is null
// ============================================================================
template <typename T>
inline bool  IcAl_fuzzyIsNull( const T & );

template <>
inline bool  IcAl_fuzzyIsNull( const double &d )
{ return IcAl_abs( d ) <= 0.000000000001; }

template <>
inline bool  IcAl_fuzzyIsNull( const float &f )
{ return IcAl_abs( f ) <= 0.00001f; }

// ============================================================================
//! linear blend function
//! @return return the value: x + ( y - x ) * a
// ============================================================================
template <typename T >
inline T    _IcAl_mix_impl_t( const T &x, const T &y, double a )
{ return x + static_cast<T>( ( y - x ) * a ); }

template <typename T >
inline T    _IcAl_mix_impl_int( const T &x, const T &y, double a )
{
    double t = ( y - x ) * a;
    return x + ( t >= 0 ? static_cast<T>( std::floor( t + 0.5 )) :
           static_cast<T>( - std::floor( -t + 0.5 )));
}

template <typename T> //! normal mixing func
inline T    IcAl_mix( const T &x, const T &y, double a )
{ return _IcAl_mix_impl_t( x, y, a ); }

template <>           //! override, IcAl_mix( )
inline int  IcAl_mix( const int &x, const int &y, double a )
{ return _IcAl_mix_impl_int( x, y, a ); }

#if defined( QXPACK_IC_AL_CFG_LLP64 )
template <>  //! override, IcAl_mix()
inline intptr_t  IcAl_mix( const intptr_t &x, const intptr_t &y, double a )
{ return _IcAl_mix_impl_int( x, y, a ); }
#endif

template <>  //! override, IcAl_mix()
inline short IcAl_mix ( const short &x, const short &y, double a )
{ return _IcAl_mix_impl_int( x, y, a ); }

template <>  //! override, IcAl_mix()
inline char  IcAl_mix ( const char &x, const char &y, double a )
{ return _IcAl_mix_impl_int( x, y, a ); }


// ============================================================================
//! scale the value 
// ============================================================================
template <typename T> // normal scale template implement
inline  T   _IcAl_scale_impl_t( const T &v, double f )
{ return static_cast<T>( v * f ); }

template <typename T> // integer scale template
inline  T  _IcAl_scale_impl_int( const T &v, double f )
{
    double t = v * f;
    return  ( t >= 0 ? static_cast<T>( std::floor( t + 0.5 )) :
            static_cast<T>( - std::floor( -t + 0.5 )) );
}

template <typename T> // normal scale
inline T    IcAl_scale( const T &v, double f )
{ return  _IcAl_scale_impl_t( v, f ); }

template <>           // override, IcAl_scale()
inline int  IcAl_scale( const int &v, double f )
{ return  _IcAl_scale_impl_int( v, f ); }

#if defined( QXPACK_IC_AL_CFG_LLP64 )
template <>  // override, IcAl_scale()
inline intptr_t  IcAl_scale( const intptr_t &v, double f )
{ return _IcAl_scale_impl_int( v, f ); }
#endif

template <>   // override, IcAl_scale()
inline short IcAl_scale( const short &v, double f )
{ return _IcAl_scale_impl_int( v, f ); }

template <>   // override, IcAl_scale()
inline char  IcAl_scale( const char &v, double f )
{ return _IcAl_scale_impl_int( v, f ); }


// ============================================================================
//! public POD ( plane old data )
// ============================================================================
template <typename T>
struct  IcAl_Pair { T first, second; };







}

#endif 
