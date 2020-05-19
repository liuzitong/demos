
! contains( CONFIRMDEMO_MODULES, base ) {
  CONFIRMDEMO_MODULES *= base

  DEFINES *= CONFIRMDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= CONFIRMDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= CONFIRMDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/confirmdemo_def.h \
    $$PWD/common/confirmdemo_memcntr.hxx \
    $$PWD/common/confirmdemo_logging.hxx \
    $$PWD/common/confirmdemo_pimplprivtemp.hpp \
    $$PWD/service/confirmdemo_appsettingssvc.hxx \
    $$PWD/service/confirmdemo_objmgrsvc.hxx \
    $$PWD/service/confirmdemo_msgbussvc.hxx \
    $$PWD/service/confirmdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/confirmdemo_memcntr.cxx \
    $$PWD/common/confirmdemo_logging.cxx \
    $$PWD/service/confirmdemo_appsettingssvc.cxx \
    $$PWD/service/confirmdemo_objmgrsvc.cxx \
    $$PWD/service/confirmdemo_msgbussvc.cxx \
    $$PWD/service/confirmdemo_cfmbussvc.cxx \

}
