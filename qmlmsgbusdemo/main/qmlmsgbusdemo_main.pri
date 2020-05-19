
! contains( QMLMSGBUSDEMO_MODULES, main ) {
  QMLMSGBUSDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlmsgbusdemo_appctrl.hxx \
    $$PWD/viewmodel/qmlmsgbusdemo_mm_mainvm.hxx \
    \
    $$PWD/qmlmsgbusdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlmsgbusdemo_entry.cpp   \
    $$PWD/appctrl/qmlmsgbusdemo_appctrl.cxx \
    $$PWD/viewmodel/qmlmsgbusdemo_mm_mainvm.cxx \
    \
    $$PWD/qmlmsgbusdemo_main.cxx \

  RESOURCES += $$PWD/view/qmlmsgbusdemo_main_view.qrc
}
