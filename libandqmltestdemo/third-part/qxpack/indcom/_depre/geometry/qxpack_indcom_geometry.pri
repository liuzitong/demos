
! contains( QXPACK_IC, indcom_geometry ) {
    QXPACK_IC *= indcom_geometry
    CONFIG *= c++11

    contains( QT, core ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }
    contains( CONFIG, qt ) {
        DEFINES *= QXPACK_IC_QT5_ENABLED
    }

    INCLUDEPATH *= $$PWD/../../../

    ! contains( QXPACK_IC_CFG, indcom_geometry_cfg_no_hdr ) {
        HEADERS += \
               $$PWD/qxpack_ic_point.hxx \
               $$PWD/qxpack_ic_vector2d.hxx \
               $$PWD/qxpack_ic_size.hxx \
               $$PWD/qxpack_ic_rect.hxx \

         message( qxpack_indcom_geometry cfg. req. header )
     } else {
         message( qxpack_indcom_geometry cfg. req. no header )
     }

     ! contains( QXPACK_IC_CFG, indcom_geometry_cfg_no_src ) {
        SOURCES += \
                $$PWD/qxpack_ic_point.cxx \

        
        message( qxpack_indcom_geometry cfg. req. source )
    } else {
        message( qxpack_indcom_geometry cfg. req. no source )
    }

}
