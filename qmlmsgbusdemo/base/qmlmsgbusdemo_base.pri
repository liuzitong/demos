
! contains( QMLMSGBUSDEMO_MODULES, base ) {
  QMLMSGBUSDEMO_MODULES *= base

  DEFINES *= QMLMSGBUSDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLMSGBUSDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLMSGBUSDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlmsgbusdemo_def.h \
    $$PWD/common/qmlmsgbusdemo_memcntr.hxx \
    $$PWD/common/qmlmsgbusdemo_logging.hxx \
    $$PWD/common/qmlmsgbusdemo_pimplprivtemp.hpp \
    $$PWD/service/qmlmsgbusdemo_appsettingssvc.hxx \
    $$PWD/service/qmlmsgbusdemo_objmgrsvc.hxx \
    $$PWD/service/qmlmsgbusdemo_msgbussvc.hxx \
    $$PWD/service/qmlmsgbusdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlmsgbusdemo_memcntr.cxx \
    $$PWD/common/qmlmsgbusdemo_logging.cxx \
    $$PWD/service/qmlmsgbusdemo_appsettingssvc.cxx \
    $$PWD/service/qmlmsgbusdemo_objmgrsvc.cxx \
    $$PWD/service/qmlmsgbusdemo_msgbussvc.cxx \
    $$PWD/service/qmlmsgbusdemo_cfmbussvc.cxx \

}
