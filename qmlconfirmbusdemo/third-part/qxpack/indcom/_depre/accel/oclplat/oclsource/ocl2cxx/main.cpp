#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <qxpack/indcom/common/qxpack_ic_minizip.hxx>
#include <qxpack/indcom/common/qxpack_ic_bytestream.hxx>
#include <string>
#include <cstdio>
#include <cstdlib>

// ======================================
// generate the header file
// ======================================
static void  gGenHeaderFile( const QString &fn )
{
    QString base_name = fn.section('.',0,0).toLower();
    QFile h_file( QCoreApplication::applicationDirPath() + "/" + base_name + ".hxx");
    if ( h_file.open( QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        QString text = QString("#ifndef %1_HXX\n"
                               "#define %1_HXX\n\n"
                               "#include <stdint.h>\n"
                               "namespace QxPack {\n\n"
                               "const uint8_t* icGetCLText_%2( ) const;\n\n"
                               "}\n"
                               "#endif\n"
                               ).arg( base_name.toUpper() ).arg( base_name );
        QTextStream str( &h_file);
        str << text;
        str.flush();
        h_file.close();
    }
}

// ==============================================
// read the source file and return the compressed data array
// ==============================================
QxPack::IcByteArray  gCompressSrc( const QString &fn )
{
    QFile r_file( QCoreApplication::applicationDirPath() + "/" + fn );
    if ( ! r_file.open( QFile::ReadOnly | QFile::Text )) { return QxPack::IcByteArray(); }

    QxPack::IcByteStream  stream;
    QTextStream src( & r_file );
    QByteArray text_ba = src.readAll().toUtf8();
    r_file.close();
    stream.append( ( uint8_t *) text_ba.constData(), text_ba.size());

    QxPack::IcByteArray   ba = stream.resultAndClear();
    QxPack::IcMiniZipComp comp( ba.constData(), ba.size());
    return comp.result();
}

// ================================================
// write the data into array string
// ================================================
QxPack::IcByteArray   gWriteByteToArray( const QxPack::IcByteArray &ba )
{
    QxPack::IcByteStream  stream;
    const uint8_t *data = ( const uint8_t *) ba.constData();
    int  data_len = ba.size(); int line_counter = 0;
    while ( data_len -- > 0 ) {
        char buff[32];
        if ( ( line_counter ++ ) % 16 == 0 ) {
            std::sprintf( buff, "%3d, \n", ( uint )( *data ++ ));
        } else {
            std::sprintf( buff, "%3d, ",   ( uint )( *data ++ ));
        }
        stream.append( ( uint8_t *) & buff[0], std::strlen( buff ));
    }
    stream.append( '0' ); // tail byte

    return stream.resultAndClear();
}

// ===============================================
// generate the cxx file
// ===============================================
void  gGenCxxFile( const QString &fn, const QxPack::IcByteArray &ba )
{
    QString base_name = fn.section('.',0,0).toLower();
    QFile c_file( QCoreApplication::applicationDirPath() + "/" + base_name + ".cxx");
    if ( ! c_file.open( QFile::WriteOnly | QFile::Text | QFile::Truncate)) { return; }

    QString text = QString("#ifndef %1_CXX\n"
                           "#define %1_CXX\n\n"
                           "#include <stdint.h>\n\n"
                           "namespace QxPack{\n\n"
                           "static const uint8_t %2[]={\n"
                           "%3\n"
                           "};\n\n"
                           "const uint8_t *  icGetCLText_%2( ) const {\n"
                           "    return  & %2[0];\n"
                           "}\n\n"
                           "}\n"
                           "#endif\n"
                           ).arg( base_name.toUpper() ).arg( base_name )
                            .arg( QString( ba.constData()) );
    QTextStream  str( & c_file );
    str << text;
    str.flush();
    c_file.close();
}

// ==============================================
// process the input file
// ==============================================
void  process( const QString &fn )
{
    gGenHeaderFile( fn );
    gGenCxxFile( fn, gWriteByteToArray( gCompressSrc( fn )));
}

// ======================================
// entry
// ======================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir app_dir( QCoreApplication::applicationDirPath(), "*.cl");
    QStringList ent_items = app_dir.entryList();
    QStringList::const_iterator c_itr = ent_items.cbegin();
    while ( c_itr != ent_items.cend()) {
        process( *c_itr ++ );
    }

    return 0;
}
