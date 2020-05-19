
!contains( QXPACK_IC, indcom_ui ) {
    QXPACK_IC *= indcom_ui

    contains( QT, core ) {
        QT *= gui qml quick
        DEFINES *= QXPACK_IC_QT5_ENABLE
    }
    contains( CONFIG, qt ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../../

    # -------------------------------------------------
    # if not contain QML module, need not to define the QML import path
    # -------------------------------------------------
    ! contains( QXPACK_IC_CFG, qxicqt5ui_cfg_no_qml ) {
        QML_IMPORT_PATH  *= $$PWD/../../../
        QML2_IMPORT_PATH *= $$PWD/../../../
        QML_DESIGNER_IMPORT_PATH *= $$PWD/../../../
        QML2_DESIGNER_IMPORT_PATH *= $$PWD/../../../
    }

    # -------------------------------------------------
    # include the headers or not
    # -------------------------------------------------
    ! contains( QXPACK_IC_CFG, indcom_ui_cfg_no_hdr ) {
        # normal headers
        HEADERS += \
             $$PWD/qxpack_ic_ui_reg.hxx \
             $$PWD/qxpack_ic_ui_api.hxx \

        # check if QML is support
        ! contains( QXPACK_IC_CFG, qxicqt5ui_cfg_no_qml ) {
            HEADERS  += \
                 $$PWD/base/qxpack_ic_qsgimtexture.hxx   \
                 $$PWD/base/qxpack_ic_quickimagesource.hxx \
                 $$PWD/base/qxpack_ic_quickimageitem.hxx \
                 $$PWD/base/qxpack_ic_qsgtextureprov.hxx \

        }

        message( qxpack_indcom_ui cfg. req. header )
    } else {
        message( qxpack_indcom_ui cfg. req. no header )
    }

    # ----------------------------------------------------
    # include the sources
    # ----------------------------------------------------
    ! contains( QXPACK_IC_CFG, indcom_ui_cfg_no_src ) {

        # normal source
        SOURCES += \
              $$PWD/qxpack_ic_ui_reg.cxx \
              $$PWD/qxpack_ic_ui_api.cxx \

        # resources, maybe included QML?
        RESOURCES += \
              $$PWD/base/base.qrc \

        # check if support QML
        ! contains( QXPACK_IC_CFG, qxicqt5ui_cfg_no_qml ) {
            SOURCES  += \
                 $$PWD/base/qxpack_ic_qsgimtexture.cxx   \
                 $$PWD/base/qxpack_ic_quickimagesource.cxx \
                 $$PWD/base/qxpack_ic_quickimageitem.cxx \
                 $$PWD/base/qxpack_ic_qsgtextureprov.cxx \

            QXIC_MODULE_OTHER_COPY_FILES += \
                 $$PWD/base/qmldir \
                 $$PWD/base/modalpopupdialog.qml \

        }

        message( qxpack_indcom_ui cfg. req. source )
    } else {
        message( qxpack_indcom_ui cfg. req. no source )
    }

}


