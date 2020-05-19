#ifndef QXPACK_IC_AL_STAT_SERIES_HPP
#define QXPACK_IC_AL_STAT_SERIES_HPP

#include "qxpack_ic_al_def.h"
#include "qxpack_ic_al_global.hpp"
#include <stdint.h>
#include <cmath>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
//
// the series data type must be float, double, integer type.
//
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
/*!
 * @brief   find the minimal value in series
 * @param   dat     [in]  the series data pointer
 * @param   dat_num [in]  the data number
 * @return  minimal data
 */
// ============================================================================
template <typename T>
T  IcAl_stat_series_min ( const T *dat, size_t dat_num )
{
    if ( dat == nullptr || dat_num < 1 ) { return 0; }
    const T* limit = dat + dat_num;
    T min = *dat ++;
    while ( dat < limit ) { if ( !( min < *dat )) { min = *dat; } ++ dat; }

    return min;
}

// ============================================================================
/*!
 * @brief   find the maximum value in series
 * @param   dat     [in]  the data pointer
 * @param   dat_num [in]  the data number
 * @return  maximum data
 */
// ============================================================================
template <typename T>
T   IcAl_stat_series_max ( const T *dat, size_t dat_num )
{
    if ( dat == nullptr || dat_num < 1 ) { return 0; }
    const T* limit = dat + dat_num;
    T max = *dat ++;
    while ( dat < limit ) { if ( max < *dat ) { max = *dat; } ++ dat; }

    return max;
}

// ============================================================================
/*!
 * @brief  calculate the sum value in the series
 * @param   dat      [in]  the data pointer
 * @param   dat_num  [in]  the data number
 * @return  average value in the series
 */
// ============================================================================
template <typename T>
T       _IcAl_stat_series_sum_impl_t ( const T *dat, size_t dat_num, T ofv )
{
    if ( dat == nullptr || dat_num < 1 ) { return 0; }
    const T* limit = dat + dat_num;
    T  sum = *dat ++ - ofv;
    while ( dat < limit ) { sum += *dat ++ - ofv; }
    return sum;
}

template <typename T>
double  _IcAl_stat_series_sum_impl_dbl ( const T *dat, size_t dat_num, double ofv )
{
    if ( dat == nullptr || dat_num < 1 ) { return 0; }
    const T* limit = dat + dat_num;
    double  sum = double( *dat ++ ) - ofv;
    while ( dat < limit ) { sum += double( *dat ++ ) - ofv; }
    return sum;
}

//! calculate series sum, return type is double
template <typename T>
double   IcAl_stat_series_sumf ( const T *dat, size_t dat_num, double  ofv )
{  return _IcAl_stat_series_sum_impl_dbl( dat, dat_num, ofv ); }

template <typename T>
intptr_t IcAl_stat_series_sumi ( const T *dat, size_t dat_num, intptr_t ofv )
{  return _IcAl_stat_series_sum_impl_t( dat, dat_num, ofv ); }


// ============================================================================
/*!
 * @brief Statistic method, series, Qn ( 0% to 100% )
 * @param   dat     [in] the data pointer
 * @param   dat_num [in] the data number
 * @return  a data value; 
 * @note    The data must be sorted
 */
// ============================================================================
template <typename T > 
T   IcAl_stat_series_evalQ( const T *dat, size_t dat_num, double pct )
{
    // ------------------------------------------------------------------------
    // param. check
    // ------------------------------------------------------------------------
    if ( dat == nullptr || dat_num < 1 ) { return 0; }
    pct = IcAl_bound( 0.0, pct, 1.0 );
 
    // ------------------------------------------------------------------------
    // eval the Q by percentage
    // ------------------------------------------------------------------------  
    double  v_int = 0, v_frac = 0;
    T  q;

    if ( dat_num > 1 ) {
        v_int  = ( dat_num - 1 ) * pct;         // the data number should be absolute value
        v_frac = std::modf( v_int, & v_int );   // fract is absoluate value 
        int pos = int( v_int );
        if ( ! IcAl_fuzzyIsNull( v_frac ) ) {   // exist the fract number
            q = IcAl_mix( dat[ pos ], dat[ pos + 1 ], v_frac );
        } else {         
            q = dat[ pos ];
        }
    } else {
        q = static_cast<T>( dat[0] * pct );
    }

    return q;
}

// ============================================================================
//! estimate the normal exception value
//! @param q1 [in] the Q1 (25%) in series
//! @param q3 [in] the Q3 (75%) in series
//! @param factor [in] the factor, 1.5 or 3.0 ( extreme )
//! @return  a pair< low_bound, high_bound >
// ============================================================================
template <typename T>
inline IcAl_Pair<T> IcAl_stat_series_outlinerBound( const T &q1, const T &q3, double factor = 1.5 )
{
    // ------------------------------------------------------------------------
    // formular:
    //  iqr = q3 - q1
    //  low_bound = q1 - iqr * 1.5
    //  high_bound= q3 + iqr * 1.5
    // ------------------------------------------------------------------------
    T tmp = q3 - q1;
    tmp   = IcAl_scale( tmp, factor );

    IcAl_Pair<T> pair;
    pair.first  = q1 - tmp;
    pair.second = q3 + tmp;

    return pair;
}  





/*!
 *
 */



}
#endif 
