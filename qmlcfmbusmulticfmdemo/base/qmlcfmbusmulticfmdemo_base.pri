
! contains( QMLCFMBUSMULTICFMDEMO_MODULES, base ) {
  QMLCFMBUSMULTICFMDEMO_MODULES *= base

  DEFINES *= QMLCFMBUSMULTICFMDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLCFMBUSMULTICFMDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLCFMBUSMULTICFMDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlcfmbusmulticfmdemo_def.h \
    $$PWD/common/qmlcfmbusmulticfmdemo_memcntr.hxx \
    $$PWD/common/qmlcfmbusmulticfmdemo_logging.hxx \
    $$PWD/common/qmlcfmbusmulticfmdemo_pimplprivtemp.hpp \
    $$PWD/service/qmlcfmbusmulticfmdemo_appsettingssvc.hxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_objmgrsvc.hxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_msgbussvc.hxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlcfmbusmulticfmdemo_memcntr.cxx \
    $$PWD/common/qmlcfmbusmulticfmdemo_logging.cxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_appsettingssvc.cxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_objmgrsvc.cxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_msgbussvc.cxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_cfmbussvc.cxx \

}
