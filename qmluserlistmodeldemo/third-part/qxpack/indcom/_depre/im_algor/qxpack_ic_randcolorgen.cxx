#ifndef QXPACK_IC_RANDCOLORGEN_CXX
#define QXPACK_IC_RANDCOLORGEN_CXX

#include "qxpack_ic_randcolorgen.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include <QColor>
#include <QVector>

namespace QxPack {

// ////////////////////////////////////////////////////////////////////////////
// private object
// ////////////////////////////////////////////////////////////////////////////
#define T_PrivPtr( o )  T_ObjCast( IcRandColorGenPriv*, o )
class QXPACK_IC_HIDDEN  IcRandColorGenPriv {
private:
    QVector<QColor>  m_color_tbl;
    IcLCG m_lcg;
public :
    explicit IcRandColorGenPriv ( ) { }
    ~IcRandColorGenPriv ( ) { }
    inline IcLCG &   lcgRef( ) { return m_lcg; }
    inline QVecotr<QColor> &  colorTblRef( ) { return m_color_tbl; }
};


// ////////////////////////////////////////////////////////////////////////////
// wrap API
// ////////////////////////////////////////////////////////////////////////////
// ============================================================================
// ctor
// ============================================================================
IcRandColorGen :: IcRandColorGen ( )
{
    m_obj = qxpack_ic_new( IcRandColorGenPriv );
}

// ============================================================================
// dtor
// ============================================================================
IcRandColorGen :: ~IcRandColorGen( )
{
    qxpack_ic_delete( m_obj, IcRandColorGenPriv );
}

// ============================================================================
// make the color table
// ============================================================================
QVector<QColor> IcRandColorGen :: make ( int num, float s, float v )
{
    if ( num < 1 ) { num = 1; }
    
    // ------------------------------------------------------------------------
    // make the color from randomize algorithm
    // ------------------------------------------------------------------------
    IcLCG &lcg_ref = T_PrivPtr( m_obj )->lcgRef( );
    QVector<QColor> &color_tbl = T_PrivPtr( m_obj )->colorTblRef( );
    color_tbl = QVecotr<QColor>();
    color_tbl.reserve( num );    

    for ( int i = 0; i < num; i ++ ) {
        float h = (( lcg_ref.value() % 43 ) / 43.0 + 0.618034 );
        if ( h > 1.0 ) { h -= 1.0; }
        color_tbl[ i ] = QColor::fromHsvF( h, s, v );
    }
    
    return color_tbl;
}

// ============================================================================
// return last color table
// ============================================================================
QVector<QColor> IcRandColorGen :: lastResult ( ) const 
{ return T_PrivPtr( m_obj )->colorTblRef( ); }


}

#endif 
