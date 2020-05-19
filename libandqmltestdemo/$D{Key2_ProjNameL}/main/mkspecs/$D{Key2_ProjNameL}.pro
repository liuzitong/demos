QT -= gui
CONFIG += c++11
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

INCLUDEPATH *= $$PWD/../../../

# ///////////////////////////////////////////////////////////////////////////
# sub function module
# ///////////////////////////////////////////////////////////////////////////
DEFINES *= $D{Key2_ProjNameU}_CFG_STATIC
include( $$PWD/../../base/$D{Key2_ProjNameL}_base.pri )
include( $$PWD/../../main/$D{Key2_ProjNameL}_main.pri )
include( $$PWD/../../third-part/qxpackic_all.pri )

LIBS += -L"$$PWD/../../../out/libandqmltestdemo"
CONFIG( debug, debug|release ) {
    LIBS *= -llibandqmltestdemod
} else {
    LIBS *= -llibandqmltestdemo
}
DESTDIR = $$PWD/../../../out/libandqmltestdemo

# ///////////////////////////////////////////////////////////////////////////
# put the final app into /tmp for debug
# ///////////////////////////////////////////////////////////////////////////
#APP_INSTALL_PATH=/tmp
#target.path = $${APP_INSTALL_PATH}
#INSTALLS += target
