
! contains( QMLICQUICKIMAGEITEMDEMO_MODULES, base ) {
  QMLICQUICKIMAGEITEMDEMO_MODULES *= base

  DEFINES *= QMLICQUICKIMAGEITEMDEMO_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= QMLICQUICKIMAGEITEMDEMO_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= QMLICQUICKIMAGEITEMDEMO_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/qmlicquickimageitemdemo_def.h \
    $$PWD/common/qmlicquickimageitemdemo_memcntr.hxx \
    $$PWD/common/qmlicquickimageitemdemo_logging.hxx \
    $$PWD/common/qmlicquickimageitemdemo_pimplprivtemp.hpp \
    $$PWD/service/qmlicquickimageitemdemo_appsettingssvc.hxx \
    $$PWD/service/qmlicquickimageitemdemo_objmgrsvc.hxx \
    $$PWD/service/qmlicquickimageitemdemo_msgbussvc.hxx \
    $$PWD/service/qmlicquickimageitemdemo_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/qmlicquickimageitemdemo_memcntr.cxx \
    $$PWD/common/qmlicquickimageitemdemo_logging.cxx \
    $$PWD/service/qmlicquickimageitemdemo_appsettingssvc.cxx \
    $$PWD/service/qmlicquickimageitemdemo_objmgrsvc.cxx \
    $$PWD/service/qmlicquickimageitemdemo_msgbussvc.cxx \
    $$PWD/service/qmlicquickimageitemdemo_cfmbussvc.cxx \

}
