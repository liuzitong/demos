
! contains( QMLUSERLISTMODELDEMO_MODULES, main ) {
  QMLUSERLISTMODELDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmluserlistmodeldemo_appctrl.hxx \
    \
    $$PWD/qmluserlistmodeldemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmluserlistmodeldemo_entry.cpp   \
    $$PWD/appctrl/qmluserlistmodeldemo_appctrl.cxx \
    \
    $$PWD/qmluserlistmodeldemo_main.cxx \

  RESOURCES += $$PWD/view/qmluserlistmodeldemo_main_view.qrc

  # [HINT] NOTE: here add this import path for QtCreator.
  QML_IMPORT_PATH *= $$PWD/../../

}
