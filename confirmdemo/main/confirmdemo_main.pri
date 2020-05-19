
! contains( CONFIRMDEMO_MODULES, main ) {
  CONFIRMDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/confirmdemo_appctrl.hxx \
    $$PWD/viewmodel/confirmdemo_mm_mainvm.hxx \
    $$PWD/service/confirmdemo_mm_confirmsvc.hxx \ # [HINT] add the confirm service
    \
    $$PWD/confirmdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/confirmdemo_entry.cpp   \
    $$PWD/appctrl/confirmdemo_appctrl.cxx \
    $$PWD/viewmodel/confirmdemo_mm_mainvm.cxx \
    $$PWD/service/confirmdemo_mm_confirmsvc.cxx \ # [HINT] add the confirm service
    \
    $$PWD/confirmdemo_main.cxx \

  RESOURCES += $$PWD/view/confirmdemo_main_view.qrc

  QML_IMPORT_PATH *= $$PWD/../../ # [HINT] for use /confirmdemo/main/view code hint
}
