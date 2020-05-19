
! contains( QMLCONFIRMBUSDEMO_MODULES, base ) {
  QMLCONFIRMBUSDEMO_MODULES *= base

  DEFINES *= QMLCONFIRMBUSDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLCONFIRMBUSDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLCONFIRMBUSDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlconfirmbusdemo_def.h \
    $$PWD/common/qmlconfirmbusdemo_memcntr.hxx \
    $$PWD/common/qmlconfirmbusdemo_logging.hxx \
    $$PWD/common/qmlconfirmbusdemo_pimplprivtemp.hpp \
    $$PWD/service/qmlconfirmbusdemo_appsettingssvc.hxx \
    $$PWD/service/qmlconfirmbusdemo_objmgrsvc.hxx \
    $$PWD/service/qmlconfirmbusdemo_msgbussvc.hxx \
    $$PWD/service/qmlconfirmbusdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlconfirmbusdemo_memcntr.cxx \
    $$PWD/common/qmlconfirmbusdemo_logging.cxx \
    $$PWD/service/qmlconfirmbusdemo_appsettingssvc.cxx \
    $$PWD/service/qmlconfirmbusdemo_objmgrsvc.cxx \
    $$PWD/service/qmlconfirmbusdemo_msgbussvc.cxx \
    $$PWD/service/qmlconfirmbusdemo_cfmbussvc.cxx \

}
