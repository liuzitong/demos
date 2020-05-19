
! contains( QMLUSERLISTMODELDEMO_MODULES, people ) {
  QMLUSERLISTMODELDEMO_MODULES *= people

  QT *= quick qml

  HEADERS += \
    $$PWD/qmluserlistmodeldemo_people.hxx \
    \
    $$PWD/model/qmluserlistmodeldemo_pp_peoplelistmodel.hxx \
    $$PWD/model/qmluserlistmodeldemo_pp_personinfo.hxx \
    $$PWD/service/qmluserlistmodeldemo_pp_peoplemgrsvc.hxx \
    $$PWD/viewmodel/qmluserlistmodeldemo_pp_peoplemgrvm.hxx \

  SOURCES += \
    $$PWD/qmluserlistmodeldemo_people.cxx \
    \
    $$PWD/model/qmluserlistmodeldemo_pp_peoplelistmodel.cxx \
    $$PWD/model/qmluserlistmodeldemo_pp_personinfo.cxx \
    $$PWD/service/qmluserlistmodeldemo_pp_peoplemgrsvc.cxx \
    $$PWD/viewmodel/qmluserlistmodeldemo_pp_peoplemgrvm.cxx \
}
