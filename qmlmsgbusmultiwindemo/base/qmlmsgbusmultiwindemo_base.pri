
! contains( QMLMSGBUSMULTIWINDEMO_MODULES, base ) {
  QMLMSGBUSMULTIWINDEMO_MODULES *= base

  DEFINES *= QMLMSGBUSMULTIWINDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLMSGBUSMULTIWINDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLMSGBUSMULTIWINDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlmsgbusmultiwindemo_def.h \
    $$PWD/common/qmlmsgbusmultiwindemo_memcntr.hxx \
    $$PWD/common/qmlmsgbusmultiwindemo_logging.hxx \
    $$PWD/common/qmlmsgbusmultiwindemo_pimplprivtemp.hpp \
    $$PWD/service/qmlmsgbusmultiwindemo_appsettingssvc.hxx \
    $$PWD/service/qmlmsgbusmultiwindemo_objmgrsvc.hxx \
    $$PWD/service/qmlmsgbusmultiwindemo_msgbussvc.hxx \
    $$PWD/service/qmlmsgbusmultiwindemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlmsgbusmultiwindemo_memcntr.cxx \
    $$PWD/common/qmlmsgbusmultiwindemo_logging.cxx \
    $$PWD/service/qmlmsgbusmultiwindemo_appsettingssvc.cxx \
    $$PWD/service/qmlmsgbusmultiwindemo_objmgrsvc.cxx \
    $$PWD/service/qmlmsgbusmultiwindemo_msgbussvc.cxx \
    $$PWD/service/qmlmsgbusmultiwindemo_cfmbussvc.cxx \

}
