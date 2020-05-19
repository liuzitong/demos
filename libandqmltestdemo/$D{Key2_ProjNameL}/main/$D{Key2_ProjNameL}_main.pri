
! contains( $D{Key2_ProjNameU}_MODULES, main ) {
  $D{Key2_ProjNameU}_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/$D{Key2_ProjNameL}_appctrl.hxx \
    \
    $$PWD/$D{Key2_ProjNameL}_main.hxx \

  SOURCES += \
    $$PWD/appctrl/$D{Key2_ProjNameL}_entry.cpp   \
    $$PWD/appctrl/$D{Key2_ProjNameL}_appctrl.cxx \
    \
    $$PWD/$D{Key2_ProjNameL}_main.cxx \

  RESOURCES += $$PWD/view/$D{Key2_ProjNameL}_main_view.qrc
}
