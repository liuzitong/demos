#ifndef QXPACK_IC_VECTOR2D_CXX
#define QXPACK_IC_VECTOR2D_CXX

#include "qxpack_ic_vector2d.hxx"
#include <cmath>

namespace QxPack {

// ===========================================
// length calc.
// ===========================================
float  IcVector2D :: length() const
{
    return float( std::sqrt( double(xp) * double(xp) + double(yp) * double(yp)));
}

// ===========================================
// normalize the vector
// ===========================================
void   IcVector2D :: normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp);

    if ( QxPack::fuzzyIsNull( len - 1.0f) || QxPack::fuzzyIsNull( len ))
    { return; }

    len = std::sqrt(len); // use inv.sqrt ?
    xp = float(double(xp) / len);
    yp = float(double(yp) / len);
}

// ===========================================
// retyrb a  normalized vector
// ===========================================
IcVector2D  IcVector2D :: normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp);
    if ( QxPack::fuzzyIsNull( len - 1.0f )) {
        return *this;
    } else if (! QxPack::fuzzyIsNull( len )) {
        double sqrtLen = std::sqrt(len);
        return IcVector2D(float(double(xp) / sqrtLen), float(double(yp) / sqrtLen));
    } else {
        return IcVector2D();
    }
}

// ============================================
// return distance to the point
// ============================================
float   IcVector2D :: distanceToPoint( const IcVector2D &pt ) const
{
    return (*this - pt ).length();
}

// ============================================
// calc. distance vertex to  a line
// ============================================
float   IcVector2D :: distanceToLine ( const IcVector2D &pt, const IcVector2D &direction ) const
{
    if (direction.isNull()) {
        return ( *this - pt ).length();
    }

    IcVector2D p = pt + this->dotProduct( *this - pt, direction ) * direction;
    return (*this - p).length();
}


}


#endif
