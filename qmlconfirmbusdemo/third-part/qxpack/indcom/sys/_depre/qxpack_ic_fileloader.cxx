#ifndef QXPACK_IC_FILELOADER_CXX
#define QXPACK_IC_FILELOADER_CXX

#include "qxpack_ic_fileloader.hxx"
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <QFile>
#include <QTextStream>

namespace QxPack {

// //////////////////////////////////////////////////////
// base loader
// //////////////////////////////////////////////////////
    IcFileLoader ::  IcFileLoader ( const QString & ) { }
    IcFileLoader :: ~IcFileLoader ( ) { }

// //////////////////////////////////////////////////////
//
//  binary file loader
//
// //////////////////////////////////////////////////////
// ======================================================
// CTOR
// ======================================================
    IcFileBinaryLoader :: IcFileBinaryLoader ( const QString &fp )
                        : IcFileLoader ( fp )
{
    m_obj = Q_NULLPTR; m_is_loaded = false;
    QFile file( fp );
    if ( file.open( QFile::ReadOnly )) {
        m_ba = file.readAll();
        file.close();
        m_is_loaded = true;
    }
}

// =====================================================
// DTOR
// =====================================================
    IcFileBinaryLoader :: ~IcFileBinaryLoader( )
{
}

// =====================================================
// APIs
// =====================================================
const char *  IcFileBinaryLoader :: loaderName() const
{  return "IcFileBinaryLoader"; }

bool         IcFileBinaryLoader :: isLoaded( ) const { return m_is_loaded; }

QByteArray   IcFileBinaryLoader :: result( ) { return m_ba; }


// /////////////////////////////////////////////////////////
//
// text file loader
//
// /////////////////////////////////////////////////////////
// ======================================================
// ctor
// ======================================================
    IcFileTextLoader :: IcFileTextLoader ( const QString &file_path )
                      : IcFileLoader( file_path )
{
    m_obj = Q_NULLPTR; m_is_loaded = false;
    QFile file( file_path );
    if ( file.open( QFile::ReadOnly | QFile::Text )) {
        QTextStream text( & file );
        m_str = text.readAll();
        file.close();
        m_is_loaded = true;
    }
}

// =====================================================
// dtor
// =====================================================
    IcFileTextLoader :: ~IcFileTextLoader ( )
{

}

// =====================================================
// APIs
// =====================================================
bool   IcFileTextLoader :: isLoaded() const { return m_is_loaded; }
const char * IcFileTextLoader :: loaderName() const { return "IcFileTextLoader"; }
QString  IcFileTextLoader :: result( ) { return m_str; }




}

#endif
