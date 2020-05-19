
! contains( QMLICQUICKIMAGEITEMDEMO_MODULES, main ) {
  QMLICQUICKIMAGEITEMDEMO_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/qmlicquickimageitemdemo_appctrl.hxx \
    $$PWD/viewmodel/qmlicquickimageitemdemo_mm_mainvm.hxx \
    $$PWD/model/qmlicquickimageitemdemo_mm_imagelistmodel.hxx \  # [HINT] new imagelist model
    \
    $$PWD/qmlicquickimageitemdemo_main.hxx \

  SOURCES += \
    $$PWD/appctrl/qmlicquickimageitemdemo_entry.cpp   \
    $$PWD/appctrl/qmlicquickimageitemdemo_appctrl.cxx \
    $$PWD/viewmodel/qmlicquickimageitemdemo_mm_mainvm.cxx \
    $$PWD/model/qmlicquickimageitemdemo_mm_imagelistmodel.cxx \
    \
    $$PWD/qmlicquickimageitemdemo_main.cxx \

  RESOURCES += $$PWD/view/qmlicquickimageitemdemo_main_view.qrc

  QML_IMPORT_PATH *= $$PWD/../../ # [HINT] for use /qmlicquickimageitemdemo/main/view code hint
}
