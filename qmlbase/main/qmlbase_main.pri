
! contains( QMLBASE_MODULES, main ) {
  QMLBASE_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlbase_appctrl.hxx \
    \
    $$PWD/qmlbase_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlbase_entry.cpp   \
    $$PWD/appctrl/qmlbase_appctrl.cxx \
    \
    $$PWD/qmlbase_main.cxx \

  RESOURCES += $$PWD/view/qmlbase_main_view.qrc
}
