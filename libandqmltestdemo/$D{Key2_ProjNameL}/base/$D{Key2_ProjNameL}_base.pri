
! contains( $D{Key2_ProjNameU}_MODULES, base ) {
  $D{Key2_ProjNameU}_MODULES *= base

  DEFINES *= $D{Key2_ProjNameU}_USE_QT5_INFO

  contains( QT, core ) {
      DEFINES *= $D{Key2_ProjNameU}_QT5_ENABLED
  }
  contains( CONFIG, qt ) {
      DEFINES *= $D{Key2_ProjNameU}_QT5_ENABLED
  }

  HEADERS += \
    $$PWD/common/$D{Key2_ProjNameL}_def.h \
    $$PWD/common/$D{Key2_ProjNameL}_memcntr.hxx \
    $$PWD/common/$D{Key2_ProjNameL}_logging.hxx \
    $$PWD/common/$D{Key2_ProjNameL}_pimplprivtemp.hpp \
    $$PWD/service/$D{Key2_ProjNameL}_appsettingssvc.hxx \
    $$PWD/service/$D{Key2_ProjNameL}_objmgrsvc.hxx \
    $$PWD/service/$D{Key2_ProjNameL}_msgbussvc.hxx \
    $$PWD/service/$D{Key2_ProjNameL}_cfmbussvc.hxx \

  SOURCES += \
    $$PWD/common/$D{Key2_ProjNameL}_memcntr.cxx \
    $$PWD/common/$D{Key2_ProjNameL}_logging.cxx \
    $$PWD/service/$D{Key2_ProjNameL}_appsettingssvc.cxx \
    $$PWD/service/$D{Key2_ProjNameL}_objmgrsvc.cxx \
    $$PWD/service/$D{Key2_ProjNameL}_msgbussvc.cxx \
    $$PWD/service/$D{Key2_ProjNameL}_cfmbussvc.cxx \

}
