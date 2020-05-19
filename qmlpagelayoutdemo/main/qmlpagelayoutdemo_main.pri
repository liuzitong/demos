
! contains( QMLPAGELAYOUTDEMO_MODULES, main ) {
  QMLPAGELAYOUTDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlpagelayoutdemo_appctrl.hxx \
    \
    $$PWD/qmlpagelayoutdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlpagelayoutdemo_entry.cpp   \
    $$PWD/appctrl/qmlpagelayoutdemo_appctrl.cxx \
    \
    $$PWD/qmlpagelayoutdemo_main.cxx \

  RESOURCES += $$PWD/view/qmlpagelayoutdemo_main_view.qrc

  QML_IMPORT_PATH *= $$PWD/../../  # [HINT] add it to QML designer
}
