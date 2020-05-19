
! contains( QMLMSGBUSMULTIWINDEMO_MODULES, msgsource ) {
  QMLMSGBUSMULTIWINDEMO_MODULES *= msgsource

  DEFINES *= QMLMSGBUSMULTIWINDEMO_USE_QT5_INFO

  HEADERS += \
      $$PWD/qmlmsgbusmultiwindemo_msgsource.hxx \
      $$PWD/service/qmlmsgbusmultiwindemo_ms_msggensvc.hxx \
     
  SOURCES += \
      $$PWD/qmlmsgbusmultiwindemo_msgsource.cxx \
      $$PWD/service/qmlmsgbusmultiwindemo_ms_msggensvc.cxx \

}
