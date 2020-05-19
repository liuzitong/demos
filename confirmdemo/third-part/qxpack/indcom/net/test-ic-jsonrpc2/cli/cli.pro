QT += core
QT -= gui

CONFIG += c++11

TARGET = jsonrpc2-cli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += cli.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR  = $$OUT_PWD/../bin

DEFINES *= QXPACK_CFG_STATIC
#DEFINES *= TEST_JSONRPC2_TCP  # uncomment it if test local socket


QXPACK_IC_CFG *= qxiccore_cfg_hdr_and_src
include( $$PWD/../../../qxiccore.pri )
include( $$PWD/../../../sys/qxpack_indcom_sys.pri )
include( $$PWD/../../../net/qxpack_indcom_net.pri )

CONFIG( debug, debug|release) {
    DEFINES *= QXPACK_IC_CFG_MEM_TRACE
} else {

}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
