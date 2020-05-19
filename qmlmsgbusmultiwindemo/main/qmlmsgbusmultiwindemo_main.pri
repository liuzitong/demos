
! contains( QMLMSGBUSMULTIWINDEMO_MODULES, main ) {
  QMLMSGBUSMULTIWINDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlmsgbusmultiwindemo_appctrl.hxx \
    $$PWD/viewmodel/qmlmsgbusmultiwindemo_mm_mainvm.hxx \
    \
    $$PWD/qmlmsgbusmultiwindemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlmsgbusmultiwindemo_entry.cpp   \
    $$PWD/appctrl/qmlmsgbusmultiwindemo_appctrl.cxx \
    $$PWD/viewmodel/qmlmsgbusmultiwindemo_mm_mainvm.cxx \
    \
    $$PWD/qmlmsgbusmultiwindemo_main.cxx \

  RESOURCES += $$PWD/view/qmlmsgbusmultiwindemo_main_view.qrc
}
