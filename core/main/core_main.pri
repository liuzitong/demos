
! contains( CORE_MODULES, main ) {
  CORE_MODULES *= main

  HEADERS += \
    $$PWD/appctrl/core_appctrl.hxx \
    \
    $$PWD/core_main.hxx \

  SOURCES += \
    $$PWD/appctrl/core_entry.cpp   \
    $$PWD/appctrl/core_appctrl.cxx \
    \
    $$PWD/core_main.cxx \


}
