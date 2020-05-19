# ////////////////////////////////////////////////
# configure section
# ////////////////////////////////////////////////
TEMPLATE = lib
CONFIG += c++11 skip_target_version_ext unversioned_libname
CONFIG -= app_bundle
CONFIG -= qt
QT =

DEFINES *= QT_MESSAGELOGCONTEXT

CONFIG(debug,debug|release) {
    DEFINES *= LIBANDQMLTESTDEMO_CFG_DLL
} else {
    DEFINES *= LIBANDQMLTESTDEMO_CFG_DLL
    DEFINES *= QT_NO_DEBUG_OUTPUT
}

QXIC_MODULE_TARGET   = libandqmltestdemo
QXIC_MODULE_QT_BUILD = $$section( QMAKESPEC, /, -3, -3)
QXIC_MODULE_QT_VER   = $$section( QMAKESPEC, /, -4, -4)
QXIC_MODULE_QT_MMVER = $$section( QXIC_MODULE_QT_VER, ., 0, 1 )
QXIC_MODULE_BASE_DIR = $$PWD/../
QXIC_MODULE_INSTALL_DIR = $$PWD/../../libandqmltestdemo-build
QXIC_MODULE_INCLUDE = $$QXIC_MODULE_INSTALL_DIR/libandqmltestdemo
QXIC_MODULE_DESTDIR = $$QXIC_MODULE_INSTALL_DIR/libandqmltestdemo/build/$$QXIC_MODULE_QT_BUILD
QXIC_MODULE_OTHER_COPY_FILES =

win32:VERSION = 0.1.0.0
else: VERSION = 0.1.0

# ////////////////////////////////////////////////
# sub modules section
# ////////////////////////////////////////////////
LIBANDQMLTESTDEMO_CFG =
LIBANDQMLTESTDEMO     =
include( $$PWD/qxiccommon.pri )
include( $$PWD/../common/libandqmltestdemo_common.pri )
include( $$PWD/../main/libandqmltestdemo_main.pri )

# ////////////////////////////////////////////////
# generate a copy.pro file at install directory.
# ////////////////////////////////////////////////
QXIC_MODULE_TMP_COPY_PRO = $${QXIC_MODULE_TARGET}_tmp_cpy.pro
QXIC_MODULE_COPY_FILES = \
     $${HEADERS_COPYS} \
     $$PWD/../common/libandqmltestdemo_common.pri \
     $$PWD/../libandqmltestdemo.pri \
     $$PWD/../libandqmltestdemo_update.txt \

genTempCopyProFile( $$QXIC_MODULE_TMP_COPY_PRO, $$QXIC_MODULE_COPY_FILES, $$QXIC_MODULE_BASE_DIR, $$QXIC_MODULE_INSTALL_DIR, $$QXIC_MODULE_INCLUDE )

# QXIC_MODULE_TMP_RMV_PRO = $${QXIC_MODULE_TARGET}_tmp_rmv.pro
# QXIC_MODULE_RMV_FILES = \
#  $$QXIC_MODULE_INCLUDE/$${QXIC_MODULE_TARGET}d.ilk \
#  $$QXIC_MODULE_INCLUDE/$${QXIC_MODULE_TARGET}d.exp \
#  $$QXIC_MODULE_INCLUDE/$${QXIC_MODULE_TARGET}.ilk  \
#  $$QXIC_MODULE_INCLUDE/$${QXIC_MODULE_TARGET}.exp  \

# genTempRmvProFile( $${QXIC_MODULE_INSTALL_DIR}/$$QXIC_MODULE_TMP_RMV_PRO, $$QXIC_MODULE_RMV_FILES )

tmp_copy_pro.commands = $$QMAKE_QMAKE $$QXIC_MODULE_INSTALL_DIR/$$QXIC_MODULE_TMP_COPY_PRO

#tmp_rmv_pro.commands  = $$QMAKE_QMAKE $$QXIC_MODULE_INSTALL_DIR/$$QXIC_MODULE_TMP_RMV_PRO
#export(first.depends)
#export(QXIC_MODULE_tmp_copy_pro.commands)

QMAKE_EXTRA_TARGETS += tmp_copy_pro
POST_TARGETDEPS     += tmp_copy_pro

QMAKE_POST_LINK += $${tmp_rmv_pro.commands}


# ////////////////////////////////////////////////
# linking section
# ////////////////////////////////////////////////
win32 {
    DESTDIR = $$QXIC_MODULE_DESTDIR
    CONFIG( debug, debug|release ) {
        TARGET = $${QXIC_MODULE_TARGET}d
        LIBS += kernel32.lib user32.lib gdi32.lib
    } else {
        TARGET = $${QXIC_MODULE_TARGET}
        LIBS += kernel32.lib user32.lib gdi32.lib
#        QMAKE_CXXFLAGS *= /Zi
#        QMAKE_CFLAGS *= /Zi
#        QMAKE_LFLAGS *= /DEBUG
    }
}

unix:!android {
   CONFIG(debug,debug|release) {
       TARGET = $${QXIC_MODULE_TARGET}d
   } else {
       TARGET = $${QXIC_MODULE_TARGET}
   }
}

android {
   CONFIG(debug,debug|release) {
       TARGET = $${QXIC_MODULE_TARGET}d
   } else {
       TARGET = $${QXIC_MODULE_TARGET}
   }
   contains( QMAKE_HOST.os, Windows ) {
       cp_file_src = $${OUT_PWD}/lib$${TARGET}.so
       cp_file_dst = $${DESTDIR}/lib$${TARGET}.so
       cp_file_src = $$replace( cp_file_src, /, \\ )
       cp_file_dst = $$replace( cp_file_dst, /, \\ )
       dll_copy_tgt.commands = $$QMAKE_COPY_FILE $${cp_file_src} $${cp_file_dst}
       QMAKE_POST_LINK = $${dll_copy_tgt.commands} & $${QMAKE_POST_LINK}
   } else {
       message( you need copy $${QXPACK_MODULE_TARGET} manually. )
   }

}




