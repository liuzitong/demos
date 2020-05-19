
! contains( QMLBASE_MODULES, base ) {
  QMLBASE_MODULES *= base

  DEFINES *= QMLBASE_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLBASE_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLBASE_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlbase_def.h \
    $$PWD/common/qmlbase_memcntr.hxx \
    $$PWD/common/qmlbase_logging.hxx \
    $$PWD/common/qmlbase_pimplprivtemp.hpp \
    $$PWD/service/qmlbase_appsettingssvc.hxx \
    $$PWD/service/qmlbase_objmgrsvc.hxx \
    $$PWD/service/qmlbase_msgbussvc.hxx \
    $$PWD/service/qmlbase_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlbase_memcntr.cxx \
    $$PWD/common/qmlbase_logging.cxx \
    $$PWD/service/qmlbase_appsettingssvc.cxx \
    $$PWD/service/qmlbase_objmgrsvc.cxx \
    $$PWD/service/qmlbase_msgbussvc.cxx \
    $$PWD/service/qmlbase_cfmbussvc.cxx \

}
