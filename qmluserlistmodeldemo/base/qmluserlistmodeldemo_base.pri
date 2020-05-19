
! contains( QMLUSERLISTMODELDEMO_MODULES, base ) {
  QMLUSERLISTMODELDEMO_MODULES *= base

  DEFINES *= QMLUSERLISTMODELDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLUSERLISTMODELDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLUSERLISTMODELDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmluserlistmodeldemo_def.h \
    $$PWD/common/qmluserlistmodeldemo_memcntr.hxx \
    $$PWD/common/qmluserlistmodeldemo_logging.hxx \
    $$PWD/common/qmluserlistmodeldemo_pimplprivtemp.hpp \
    $$PWD/service/qmluserlistmodeldemo_appsettingssvc.hxx \
    $$PWD/service/qmluserlistmodeldemo_objmgrsvc.hxx \
    $$PWD/service/qmluserlistmodeldemo_msgbussvc.hxx \
    $$PWD/service/qmluserlistmodeldemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmluserlistmodeldemo_memcntr.cxx \
    $$PWD/common/qmluserlistmodeldemo_logging.cxx \
    $$PWD/service/qmluserlistmodeldemo_appsettingssvc.cxx \
    $$PWD/service/qmluserlistmodeldemo_objmgrsvc.cxx \
    $$PWD/service/qmluserlistmodeldemo_msgbussvc.cxx \
    $$PWD/service/qmluserlistmodeldemo_cfmbussvc.cxx \

}
