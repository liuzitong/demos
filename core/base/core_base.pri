
! contains( CORE_MODULES, base ) {
  CORE_MODULES *= base

  DEFINES *= CORE_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= CORE_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= CORE_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/core_def.h \
    $$PWD/common/core_memcntr.hxx \
    $$PWD/common/core_logging.hxx \
    $$PWD/common/core_pimplprivtemp.hpp \
    $$PWD/service/core_appsettingssvc.hxx \
    $$PWD/service/core_objmgrsvc.hxx \
    $$PWD/service/core_msgbussvc.hxx \
    $$PWD/service/core_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/core_memcntr.cxx \
    $$PWD/common/core_logging.cxx \
    $$PWD/service/core_appsettingssvc.cxx \
    $$PWD/service/core_objmgrsvc.cxx \
    $$PWD/service/core_msgbussvc.cxx \
    $$PWD/service/core_cfmbussvc.cxx \

}
