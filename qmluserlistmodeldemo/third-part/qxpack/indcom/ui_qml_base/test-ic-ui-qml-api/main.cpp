
#include <QDebug>
#include <qxpack/indcom/common/qxpack_ic_memcntr.hxx>
#include <QtTest>
#include <QGuiApplication>
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
    char *tmp_argv[] = { "", "-input", APP_PWD };
    return quick_test_main(3, tmp_argv,"ic-ui-api", 0);
}
