QT -= gui
CONFIG += c++11
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

INCLUDEPATH *= $$PWD/../../../

# ///////////////////////////////////////////////////////////////////////////
# sub function module
# ///////////////////////////////////////////////////////////////////////////
DEFINES *= QMLCFMBUSMULTICFMDEMO_CFG_STATIC
include( $$PWD/../../base/qmlcfmbusmulticfmdemo_base.pri )
include( $$PWD/../../main/qmlcfmbusmulticfmdemo_main.pri )
include( $$PWD/../../third-part/qxpackic_all.pri )
#include( $$PWD/../../third-part/qxpackic_all_dll.pri )

# ///////////////////////////////////////////////////////////////////////////
# put the final app into /tmp for debug
# ///////////////////////////////////////////////////////////////////////////
#APP_INSTALL_PATH=/tmp
#target.path = $${APP_INSTALL_PATH}
#INSTALLS += target
