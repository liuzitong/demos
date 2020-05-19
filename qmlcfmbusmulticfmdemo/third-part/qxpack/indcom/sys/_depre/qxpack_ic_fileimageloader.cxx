#ifndef QXPACK_IC_FILEIMAGELOADER_CXX
#define QXPACK_IC_FILEIMAGELOADER_CXX

#include "qxpack_ic_fileimageloader.hxx"
#include <QFile>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

namespace QxPack {

// /////////////////////////////////////////////////////////////
// local functions
// /////////////////////////////////////////////////////////////
// ===================================================================
// find the custom flag
// ===================================================================
static int  jdli_customFlagPos( const uint8_t *data, int data_size )
{
    int flag_pos = -1;

    // --------------------------------------------------------------------
    // we try to find the < 0xff, 0xef > pair, that maybe our custom flags
    // --------------------------------------------------------------------
    const uint8_t *dp = data;
    const uint8_t *limit;

    if ( dp == 0 ) { return flag_pos; }
    limit = dp + data_size;
    if ( limit - dp < 4  ||  !( dp[0] == 0xff && dp[1] == 0xd8 )) { return flag_pos; }
    dp += 2;  // skip JPEG header ( < 0xff, 0xd8 > )

    while ( dp < limit ) {
        if ( !( dp[0] == 0xff && dp[1] != 0xff )) { ++ dp; continue; }

        // seek for <0xff,0xef> label that is our custom label.
        if ( dp[1] == 0xef ) {
            flag_pos = dp - data; // the < 0xff, 0xef > position.
            if ( flag_pos + 4 >= data_size ) { flag_pos = -1; } // error, bad position
            break;
        } else {
            // skip this tag and it's content.
            dp += ( intptr_t )(( dp[2] << 8 ) | ( dp[3] )) + 2;
        }
    }

    return flag_pos;
}

// ===================================================================
// check if this JPEG data stream is OK
// ===================================================================
static bool  jdli_isJpeg ( const uint8_t *data, int  )
{
    bool is_jpeg = false;

    // --------------------------------------------------------------
    // the JPEG format the first two bytes is : 0xff 0xd8, and the last two
    // bytes is 0xff 0xd9
    // --------------------------------------------------------------
    const uint8_t *dp = data;
    if ( dp[0] == 0xff && dp[1] == 0xd8 ) { is_jpeg = true; }

    return is_jpeg;
}



// /////////////////////////////////////////////////////////////
//
//  implement section
//
// /////////////////////////////////////////////////////////////
// ==============================================================
// ctor
// ==============================================================
     IcFileImageLoader :: IcFileImageLoader ( const QString &file_path, const QSize &sz )
                      : IcFileLoader( file_path )
{
    m_obj = Q_NULLPTR; m_is_loaded = false;

    QFile file( file_path );
    if ( file.open( QFile::ReadOnly )) {
        QByteArray ba = file.readAll();
        file.close();

        QImage  im = QImage::fromData( ba );
        if ( ! im.isNull()) {
            // we should 
            if ( ! sz.isEmpty() ) {
            
            }
            // 
            if ( im.format() != QImage::Format_RGBA8888 ) {
                im = im.convertToFormat( QImage::Format_RGBA8888 );
            }
            m_im_data.setImage( im );
            this->userProcess( ba, m_im_data );
            m_is_loaded = true;
        }
    }
}

// ===============================================================
// dtor
// ===============================================================
    IcFileImageLoader :: ~IcFileImageLoader ( )
{

}

// ===============================================================
// return the loader name
// ===============================================================
const char *  IcFileImageLoader :: loaderName() const
{  return "IcFileImageLoader"; }

// ===============================================================
// check if loaded
// ===============================================================
bool    IcFileImageLoader :: isLoaded() const { return m_is_loaded; }

// ===============================================================
// the user process, default implement
// ===============================================================
void  IcFileImageLoader :: userProcess( const QByteArray &ba, IcImageData &dt )
{
    // ----------------------------------------------
    // check if the jpeg data
    // ----------------------------------------------
    bool is_jpeg = jdli_isJpeg( ( const uint8_t *) ba.constData(), ba.size());
    if ( ! is_jpeg ) { return; } // does nothting

    // ----------------------------------------------
    // read out the embedded information
    // ----------------------------------------------
    int tag_pos = jdli_customFlagPos( ( const uint8_t *) ba.constData(), ba.size() );
    if ( tag_pos < 0 ) { return; } // failed.

    QJsonObject j_obj;
    { // load from raw data
        const char *data = ba.constData();
        int  j_data_size = ((( int )( data[ tag_pos + 2 ] ) & 0x0ff ) << 8 ) | (( int )( data[ tag_pos + 3 ] ) & 0x0ff );
        QByteArray j_data( data + tag_pos + 4, j_data_size - 2 );
        QJsonDocument  j_doc = QJsonDocument::fromJson( j_data );
        j_obj = j_doc.object();
    }
    if ( ! j_obj.isEmpty()) {
        dt.setJsonData( j_obj );
    }
}






}

#endif
