
! contains( QMLPAGELAYOUTDEMO_MODULES, base ) {
  QMLPAGELAYOUTDEMO_MODULES *= base

  DEFINES *= QMLPAGELAYOUTDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLPAGELAYOUTDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLPAGELAYOUTDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlpagelayoutdemo_def.h \
    $$PWD/common/qmlpagelayoutdemo_memcntr.hxx \
    $$PWD/common/qmlpagelayoutdemo_logging.hxx \
    $$PWD/common/qmlpagelayoutdemo_pimplprivtemp.hpp \
    $$PWD/service/qmlpagelayoutdemo_appsettingssvc.hxx \
    $$PWD/service/qmlpagelayoutdemo_objmgrsvc.hxx \
    $$PWD/service/qmlpagelayoutdemo_msgbussvc.hxx \
    $$PWD/service/qmlpagelayoutdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlpagelayoutdemo_memcntr.cxx \
    $$PWD/common/qmlpagelayoutdemo_logging.cxx \
    $$PWD/service/qmlpagelayoutdemo_appsettingssvc.cxx \
    $$PWD/service/qmlpagelayoutdemo_objmgrsvc.cxx \
    $$PWD/service/qmlpagelayoutdemo_msgbussvc.cxx \
    $$PWD/service/qmlpagelayoutdemo_cfmbussvc.cxx \

}
