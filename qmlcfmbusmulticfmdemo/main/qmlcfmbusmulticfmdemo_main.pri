
! contains( QMLCFMBUSMULTICFMDEMO_MODULES, main ) {
  QMLCFMBUSMULTICFMDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlcfmbusmulticfmdemo_appctrl.hxx \
    $$PWD/viewmodel/qmlcfmbusmulticfmdemo_mm_mainvm.hxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_mm_confirmsvc.hxx \ # [HINT] add the confirm service
    \
    $$PWD/qmlcfmbusmulticfmdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlcfmbusmulticfmdemo_entry.cpp   \
    $$PWD/appctrl/qmlcfmbusmulticfmdemo_appctrl.cxx \
    $$PWD/viewmodel/qmlcfmbusmulticfmdemo_mm_mainvm.cxx \
    $$PWD/service/qmlcfmbusmulticfmdemo_mm_confirmsvc.cxx \ # [HINT] add the confirm service
    \
    $$PWD/qmlcfmbusmulticfmdemo_main.cxx \

  RESOURCES += $$PWD/view/qmlcfmbusmulticfmdemo_main_view.qrc

  QML_IMPORT_PATH *= $$PWD/../../ # [HINT] for use /qmlcfmbusmulticfmdemo/main/view code hint
}
