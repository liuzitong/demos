

!contains( QXPACK_IC, indcom_accel_oclplat ) {
    QXPACK_IC *= indcom_accel_oclplat

    HEADERS += $$PWD/qxpack_ic_oclbuffer.hxx \
               $$PWD/qxpack_ic_oclkernel.hxx \
               $$PWD/qxpack_ic_oclexec.hxx \
               $$PWD/qxpack_ic_oclprogcache.hxx \
               $$PWD/qxpack_ic_oclbuffcache.hxx \

    SOURCES += $$PWD/qxpack_ic_oclbuffer.cxx \
               $$PWD/qxpack_ic_oclkernel.cxx \
               $$PWD/qxpack_ic_oclexec.cxx \
               $$PWD/qxpack_ic_oclprogcache.cxx \
               $$PWD/qxpack_ic_oclbuffcache.cxx \

    INCLUDEPATH += $$PWD/ocl_libs/include

    win32 {  # windows only
        contains( QMAKE_HOST.arch, x86 ) {
            LIBS *= -L$$PWD/ocl_libs/lib_intel/x86 -lOpenCL
        }
        contains( QMAKE_HOST.arch, x86_64) {
            LIBS *= -L$$PWD/ocl_libs/lib_intel/x64 -lOpenCL
        }
    }

    unix!:macx { # linux only

    }

    macx { # mac only
        error( the macx platform is not supported. )
    }
}

include( $$PWD/../../common/qxpack_indcom_common.pri )

HEADERS += \
    $$PWD/qxpack_ic_oclvector4d.hxx \
    $$PWD/qxpack_ic_oclsgldev.hxx

SOURCES += \
    $$PWD/qxpack_ic_oclsgldev.cxx
