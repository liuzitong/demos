// /////////////////////////////////////////////////////////////////////
/*!
  @file    qxpack_ic_size
  @author  night wing
  @date    2018/11
  @brief   Size object, describe a rectangle with and height
  @version 0.1.0
*/
// /////////////////////////////////////////////////////////////////////
#ifndef QXPACK_IC_SIZE_HXX
#define QXPACK_IC_SIZE_HXX

/*!
  @addtogroup  QxPack
  @{
*/
/*!
  @addtogroup  indcom
  @{
*/
/*!
  @addtogroup  gemotry_Exported_Classes
  @{
*/

#include <qxpack/indcom/common/qxpack_ic_global.hxx>
#include <qxpack/indcom/common/qxpack_ic_def.h>
#include <cassert>

namespace QxPack {

// //////////////////////////////////////////////////
/*!
 * @brief a data structure used for describe size
 */
// //////////////////////////////////////////////////
class QXPACK_IC_API IcSize {
public :
    enum AspectRatioMode {
        IgnoreAspectRatio = 0,
        KeepAspectRatio,
        KeepAspectRatioByExpanding
    };

    IcSize( ) : wd(-1), ht(-1) { }
    IcSize( int w, int h ) : wd( w ), ht( h ) { }
    ~IcSize( ) { }

    inline bool isNull()  const { return ( wd == 0 && ht == 0 ); }
    inline bool isEmpty() const { return ( wd <  1 || ht <  1 ); }
    inline bool isValid() const { return ( wd >= 0 && ht >= 0 ); }
    inline int  width()   const { return wd; }
    inline int  height()  const { return ht; }
    inline int& rwidth()  { return wd; }
    inline int& rheight() { return ht; }

    inline void setWidth ( int w ) { wd = w; }
    inline void setHeight( int h ) { ht = h; }

    inline void    transpose ( )  { int t = wd; wd = ht; ht = t; }
    inline IcSize  transposed( ) const { return IcSize( ht, wd ); }

    inline IcSize  scaled( int w, int h,    AspectRatioMode mode ) { return  this->scaled( IcSize( w, h ), mode ); }
    inline void    scale ( int w, int h,    AspectRatioMode mode ) { *this = this->scaled( IcSize( w, h ), mode ); }
    inline void    scale ( const IcSize &s, AspectRatioMode mode ) { *this = this->scaled( s, mode ); }
           IcSize  scaled( const IcSize &,  AspectRatioMode mode );

    inline IcSize  expandedTo ( const IcSize &other ) { return IcSize( QxPack::max( wd, other.wd ), QxPack::max( ht, other.ht )); }
    inline IcSize  boundedTo  ( const IcSize &other ) { return IcSize( QxPack::min( wd, other.wd ), QxPack::min( ht, other.ht )); }

    inline IcSize& operator+=( const IcSize &s ) { wd += s.wd; ht += s.ht; return *this; }
    inline IcSize& operator-=( const IcSize &s ) { wd -= s.wd; ht -= s.ht; return *this; }
    inline IcSize& operator*=( int c )    { wd *= c; ht *= c; }
    inline IcSize& operator*=( double c ) { wd = QxPack::round( wd * c ); ht = QxPack::round( ht * c ); }
    inline IcSize& operator/=( int c )    { wd /= c; ht /= c; return *this; }
    inline IcSize& operator/=( double c ) { wd = QxPack::round( wd / c ); ht = QxPack::round( ht / c ); return *this; }

    friend inline bool  operator==( const IcSize &s1, const IcSize &s2 ) { return s1.wd == s2.wd  &&  s1.ht == s2.ht; }
    friend inline bool  operator!=( const IcSize &s1, const IcSize &s2 ) { return s1.wd != s2.wd  ||  s1.ht != s2.ht; }
    friend inline const IcSize operator+( const IcSize &s1, const IcSize &s2 )  { return IcSize( s1.wd + s2.wd, s1.ht + s2.ht ); }
    friend inline const IcSize operator-( const IcSize &s1, const IcSize &s2 )  { return IcSize( s1.wd - s2.wd, s1.ht - s2.ht ); }
    friend inline const IcSize operator*( const IcSize &s, double c )  { return IcSize( QxPack::round( s.wd * c ), QxPack::round( s.ht * c )); }
    friend inline const IcSize operator*( double c, const IcSize &s )  { return IcSize( QxPack::round( s.wd * c ), QxPack::round( s.ht * c )); }
    friend inline const IcSize operator*( const IcSize &s, int c    )  { return IcSize( s.wd * c, s.ht * c ); }
    friend inline const IcSize operator*( int  c,   const IcSize &s )  { return IcSize( s.wd * c, s.ht * c ); }
    friend inline const IcSize operator/( const IcSize &s, int c )     { return IcSize( s.wd / c, s.ht / c ); }
    friend inline const IcSize operator/( const IcSize &s, double c )  { return IcSize( QxPack::round( s.wd / c ), QxPack::round( s.ht / c )); }

private:
    int wd, ht;
};


// //////////////////////////////////////////////////////////
/*!
 * @brief IcSizeF ( float version )
 */
// //////////////////////////////////////////////////////////
class QXPACK_IC_API  IcSizeF {

public:
    enum AspectRatioMode {
        IgnoreAspectRatio = 0,
        KeepAspectRatio,
        KeepAspectRatioByExpanding
    };

    IcSizeF( ) : wd(-1.0), ht(-1.0) { }
    IcSizeF( float w, float h ) : wd(w), ht(h) { }
    ~IcSizeF( ) { }

    inline bool  isNull() const { return QxPack::isNull(wd) && QxPack::isNull(ht); }
    inline bool  isEmpty()const { return wd <= 0.0 || ht <= 0.0; }
    inline bool  isValid()const { return wd >= 0.0 || ht >= 0.0; }

    inline float  width()  const { return wd; }
    inline float  height() const { return ht; }
    inline float& rwidth()   { return wd; }
    inline float& rheight()  { return ht; }

    inline void   setWidth ( float w ) { wd = w; }
    inline void   setHeight( float h ) { ht = h; }

    inline void     transpose( )  { float t = wd; wd = ht; ht = t; }
    inline IcSizeF  transposed( ) const { return IcSizeF( ht, wd ); }

    inline IcSizeF scaled( float w, float h, AspectRatioMode mode ) { return this->scaled( IcSizeF( w, h ), mode ); }
    inline void    scale ( float w, float h, AspectRatioMode mode ) { *this = this->scaled( IcSizeF( w, h ), mode ); }
    inline void    scale ( const IcSizeF &s, AspectRatioMode mode ) { *this = this->scaled( s, mode ); }
          IcSizeF  scaled( const IcSizeF &,  AspectRatioMode mode );

    inline IcSizeF  expandedTo ( const IcSizeF &other ) { return IcSizeF( QxPack::max( wd, other.wd ), QxPack::max( ht, other.ht )); }
    inline IcSizeF  boundedTo  ( const IcSizeF &other ) { return IcSizeF( QxPack::min( wd, other.wd ), QxPack::min( ht, other.ht )); }

    inline IcSizeF &  operator+= ( const IcSizeF &s ) { wd += s.wd; ht += s.ht; return *this; }
    inline IcSizeF &  operator-= ( const IcSizeF &s ) { wd -= s.wd; ht -= s.ht; return *this; }
    inline IcSizeF &  operator*= ( float c )  { wd *= c; ht *= c; return *this; }
    inline IcSizeF &  operator/= ( float c )  { wd /= c; ht /= c; return *this; }

    friend inline bool  operator==( const IcSizeF &s1, const IcSizeF &s2 ) { return   QxPack::fuzzyCompare( s1.wd, s2.wd ) && QxPack::fuzzyCompare( s1.ht, s2.ht ); }
    friend inline bool  operator!=( const IcSizeF &s1, const IcSizeF &s2 ) { return ! QxPack::fuzzyCompare( s1.wd, s2.wd ) || ! QxPack::fuzzyCompare( s1.ht, s2.ht ); }
    friend inline const IcSizeF operator+( const IcSizeF &s1, const IcSizeF &s2 )  { return IcSizeF( s1.wd + s2.wd, s1.ht + s2.ht ); }
    friend inline const IcSizeF operator-( const IcSizeF &s1, const IcSizeF &s2 )  { return IcSizeF( s1.wd - s2.wd, s1.ht - s2.ht ); }
    friend inline const IcSizeF operator*( const IcSizeF &s, float c ) { return IcSizeF( float( s.wd * c ), float( s.ht * c )); }
    friend inline const IcSizeF operator*( float c, const IcSizeF &s ) { return IcSizeF( float( s.wd * c ), float( s.ht * c )); }
    friend inline const IcSizeF operator/( const IcSizeF &s, float c ) { return IcSizeF( float( s.wd / c ), float( s.ht / c )); }

private:
    float wd, ht;
};


}

/*!
  @}
*/
/*!
  @}
*/
/*!
  @}
*/
#endif
