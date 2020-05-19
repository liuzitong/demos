
! contains( QMLCONFIRMBUSDEMO_MODULES, main ) {
  QMLCONFIRMBUSDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlconfirmbusdemo_appctrl.hxx \
    $$PWD/viewmodel/qmlconfirmbusdemo_mm_mainvm.hxx \
    $$PWD/service/qmlconfirmbusdemo_mm_confirmsvc.hxx \ # [HINT] add the confirm service
    \
    $$PWD/qmlconfirmbusdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlconfirmbusdemo_entry.cpp   \
    $$PWD/appctrl/qmlconfirmbusdemo_appctrl.cxx \
    $$PWD/viewmodel/qmlconfirmbusdemo_mm_mainvm.cxx \
    $$PWD/service/qmlconfirmbusdemo_mm_confirmsvc.cxx \ # [HINT] add the confirm service
    \
    $$PWD/qmlconfirmbusdemo_main.cxx \

  RESOURCES += $$PWD/view/qmlconfirmbusdemo_main_view.qrc

  QML_IMPORT_PATH *= $$PWD/../../ # [HINT] for use /qmlconfirmbusdemo/main/view code hint
}
