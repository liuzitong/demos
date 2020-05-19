
#include <QDebug>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <QtTest>
#include <QGuiApplication>
#include <qxpack/indcom/ui/qxpack_ic_ui_reg.hxx>
#include <QtQuickTest/quicktest.h>
#include <QByteArray>

// /////////////////////////////////////////////////
//
// test object
//
// /////////////////////////////////////////////////
int main( int argc, char *argv[])
{
    Q_UNUSED(argc); Q_UNUSED(argv);
    QTEST_ADD_GPU_BLACKLIST_SUPPORT
    QTEST_SET_MAIN_SOURCE_PATH
    QxPack::IcUiReg(); // register..
    char *tmp_argv[] = {
        "", "-input", APP_PWD,
        "-import", IMPORT_PWD
    };
    return quick_test_main(sizeof(tmp_argv)/sizeof(char*), tmp_argv,"ic-ui-modalpopupdialog", 0);
}
