QT -= gui
CONFIG += c++11
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

INCLUDEPATH *= $$PWD/../../../

# ///////////////////////////////////////////////////////////////////////////
# sub function module
# ///////////////////////////////////////////////////////////////////////////
DEFINES *= CONFIRMDEMO_CFG_STATIC
include( $$PWD/../../base/confirmdemo_base.pri )
include( $$PWD/../../main/confirmdemo_main.pri )
include( $$PWD/../../third-part/qxpackic_all.pri )
#include( $$PWD/../../third-part/qxpackic_all_dll.pri )

# ///////////////////////////////////////////////////////////////////////////
# put the final app into /tmp for debug
# ///////////////////////////////////////////////////////////////////////////
#APP_INSTALL_PATH=/tmp
#target.path = $${APP_INSTALL_PATH}
#INSTALLS += target
