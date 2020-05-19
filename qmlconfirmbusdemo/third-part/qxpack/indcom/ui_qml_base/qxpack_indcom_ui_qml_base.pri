! contains( QXPACK_IC, indcom_ui_qml_base ) {
    QXPACK_IC *= indcom_ui_qml_base

    QT *= gui qml quick
    INCLUDEPATH *= $$PWD/../../../

    # -------------------------------------------------------------------------
    # if not contain QML module, need not to define the QML import path
    # -------------------------------------------------------------------------
    ! contains( QXPACK_IC_CFG, qxicqt5ui_cfg_no_qml ) {
        QML_IMPORT_PATH  *= $$PWD/../../../
    }

    # -------------------------------------------------------------------------
    # include the headers or not
    # -------------------------------------------------------------------------
    ! contains( QXPACK_IC_CFG, indcom_ui_qml_base_cfg_no_hdr ) {
        # normal headers
        HEADERS += \
             $$PWD/qxpack_ic_ui_qml_api.hxx \
             $$PWD/qxpack_ic_qsgimtexture.hxx \
             $$PWD/qxpack_ic_qsgtextureprov.hxx \
             $$PWD/qxpack_ic_qsgrenderworker.hxx \
             $$PWD/qxpack_ic_quickimagedata.hxx \
             $$PWD/qxpack_ic_quickfborenderbase.hxx \
             $$PWD/qxpack_ic_qsgqtlogorender.hxx \

        # other copy files ( used for QtCreator designer )
        QXIC_MODULE_OTHER_COPY_FILES *= \
            $$PWD/qmldir \
            $$PWD/icuiqmlapi_dummy.qml \
            $$PWD/qmldir_rls \

        message( qxpack_indcom_ui_qml_base cfg. req. header )
    } else {
        message( qxpack_indcom_ui_qml_base cfg. req. no header )
    }

    # -------------------------------------------------------------------------
    # include the sources
    # -------------------------------------------------------------------------
    ! contains( QXPACK_IC_CFG, indcom_ui_qml_base_cfg_no_src ) {

        # normal source
        SOURCES += \
              $$PWD/qxpack_ic_ui_qml_api.cxx \
              $$PWD/qxpack_ic_qsgimtexture.cxx \
              $$PWD/qxpack_ic_qsgtextureprov.cxx \
              $$PWD/qxpack_ic_qsgrenderworker.cxx \
              $$PWD/qxpack_ic_quickimagedata.cxx \
              $$PWD/qxpack_ic_quickfborenderbase.cxx \
              $$PWD/qxpack_ic_qsgqtlogorender.cxx \

        # resources, maybe included QML?
        RESOURCES += \
              $$PWD/qxpack_ic_ui_qml_base.qrc \

        message( qxpack_indcom_ui_qml_base cfg. req. source )
    } else {
        message( qxpack_indcom_ui_qml_base cfg. req. no source )
    }

}


