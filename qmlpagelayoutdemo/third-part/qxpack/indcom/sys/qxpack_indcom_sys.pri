

!contains( QXPACK_IC, indcom_sys ) {
    QXPACK_IC *= indcom_sys

    contains( QT, core ) {
        QT *= network gui
        DEFINES *= QXPACK_IC_QT5_ENABLE
    }
    contains( CONFIG, qt ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../../

    ! contains( QXPACK_IC_CFG, indcom_sys_cfg_no_hdr ) {
        HEADERS += \
               $$PWD/qxpack_ic_ncstring.hxx \
               $$PWD/qxpack_ic_filemon.hxx  \
               $$PWD/qxpack_ic_stdioipc.hxx \
               $$PWD/qxpack_ic_jotaskworker.hxx \
               #$$PWD/qxpack_ic_qcguard.hxx \
               $$PWD/qxpack_ic_vardata.hxx \
               $$PWD/qxpack_ic_jsonrpc2.hxx \
               $$PWD/qxpack_ic_rmtobjcreator_priv.hxx \
               $$PWD/qxpack_ic_rmtobjdeletor_priv.hxx \
               $$PWD/qxpack_ic_rmtobjsigblocker_priv.hxx \
               $$PWD/qxpack_ic_appdcl_priv.hxx \
               $$PWD/qxpack_ic_eventloopbarrier.hxx \
               $$PWD/qxpack_ic_imagedata.hxx \
               $$PWD/qxpack_ic_simplefsm.hxx \

        message( qxpack_indcom_sys cfg. req. header )
    } else {
        message( qxpack_indcom_sys cfg. req. no header )
    }

    ! contains( QXPACK_IC_CFG, indcom_sys_cfg_no_src ) {
        SOURCES += \
               $$PWD/qxpack_ic_sharedthreadpool_priv.cxx \
               $$PWD/qxpack_ic_sysevtqthread_priv.cxx \
               \
               $$PWD/qxpack_ic_ncstring.cxx \
               $$PWD/qxpack_ic_filemon.cxx  \
               $$PWD/qxpack_ic_stdioipc.cxx \
               $$PWD/qxpack_ic_jotaskworker.cxx \
               #$$PWD/qxpack_ic_qcguard.cxx \
               $$PWD/qxpack_ic_vardata.cxx \
               $$PWD/qxpack_ic_jsonrpc2.cxx \
               $$PWD/qxpack_ic_rmtobjcreator_priv.cxx \
               $$PWD/qxpack_ic_rmtobjdeletor_priv.cxx \
               $$PWD/qxpack_ic_rmtobjsigblocker_priv.cxx \
               $$PWD/qxpack_ic_eventloopbarrier.cxx \
               $$PWD/qxpack_ic_imagedata.cxx \
               $$PWD/qxpack_ic_simplefsm.cxx \

        message( qxpack_indcom_sys cfg. req. source )
    } else {
        message( qxpack_indcom_sys cfg. req. no source )
    }

}



