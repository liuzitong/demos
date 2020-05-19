QT -= gui
CONFIG += c++11
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

INCLUDEPATH *= $$PWD/../../../

# ///////////////////////////////////////////////////////////////////////////
# sub function module
# ///////////////////////////////////////////////////////////////////////////
DEFINES *= QMLPAGELAYOUTDEMO_CFG_STATIC
include( $$PWD/../../base/qmlpagelayoutdemo_base.pri )
include( $$PWD/../../main/qmlpagelayoutdemo_main.pri )
include( $$PWD/../../third-part/qxpackic_all.pri )

# ///////////////////////////////////////////////////////////////////////////
# put the final app into /tmp for debug
# ///////////////////////////////////////////////////////////////////////////
#APP_INSTALL_PATH=/tmp
#target.path = $${APP_INSTALL_PATH}
#INSTALLS += target
