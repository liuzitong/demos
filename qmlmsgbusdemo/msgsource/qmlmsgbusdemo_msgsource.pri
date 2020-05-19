
! contains( QMLMSGBUSDEMO_MODULES, msgsource ) {
  QMLMSGBUSDEMO_MODULES *= msgsource

  DEFINES *= QMLMSGBUSDEMO_USE_QT5_INFO

  HEADERS += \
      $$PWD/qmlmsgbusdemo_msgsource.hxx \
      $$PWD/service/qmlmsgbusdemo_ms_msggensvc.hxx \
     
  SOURCES += \
      $$PWD/qmlmsgbusdemo_msgsource.cxx \
      $$PWD/service/qmlmsgbusdemo_ms_msggensvc.cxx \

}
