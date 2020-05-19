import QtQuick 2.5
import QtTest  1.0
import qxicqt5ui.api 1.0

TestCase {
    id: idRoot;

    function test_apiVersionCall()
    {
        verify( QxIcUiApi.apiVersion !== "", "could not read api version not equal to 1.0");
    }

    property string currCall : "";
    function metaCall_0()
    { QxIcUiApi.leaveEventLoop(); currCall = "metaCall_0"; }
    function test_postMetalCall_0()
    {
        currCall = "";
        QxIcUiApi.postMetaCall( idRoot, "metaCall_0");
        QxIcUiApi.enterEventLoop();
        verify( currCall === "metaCall_0", "call metaCall_0 failed");
    }

    property string callParam1 : "";
    property string callParam2 : "";
    property string callParam3 : "";
    property string callParam4 : "";
    function metaCall_1( p1 )
    { QxIcUiApi.leaveEventLoop(); callParam1 = p1; }
    function metaCall_2( p1, p2 )
    { QxIcUiApi.leaveEventLoop(); callParam1 = p1; callParam2 = p2; }
    function metaCall_3( p1, p2, p3 )
    { QxIcUiApi.leaveEventLoop(); callParam1 = p1; callParam2 = p2; callParam3 = p3; }
    function metaCall_4( p1, p2, p3, p4 )
    { QxIcUiApi.leaveEventLoop(); callParam1 = p1; callParam2 = p2; callParam3 = p3; callParam4 = p4; }

    function test_postMetalCall_1()
    {
        callParam1 = ""
        QxIcUiApi.postMetaCall( idRoot, "metaCall_1", "p1");
        QxIcUiApi.enterEventLoop();
        verify( callParam1 === "p1", "1st parameter is not equal to 'p1'");
    }

    function test_postMetalCall_2()
    {
        callParam1 = ""; callParam2 = "";
        QxIcUiApi.postMetaCall( idRoot, "metaCall_2", "p1", "p2");
        QxIcUiApi.enterEventLoop();
        verify( callParam1 === "p1", "1st parameter is not equal to 'p1'");
        verify( callParam2 === "p2", "2nd parameter is not equal to 'p2'");
    }

    function test_postMetalCall_3()
    {
        callParam1 = ""; callParam2 = ""; callParam3 = "";
        QxIcUiApi.postMetaCall( idRoot, "metaCall_3", "p1", "p2", "p3");
        QxIcUiApi.enterEventLoop();
        verify( callParam1 === "p1", "1st parameter is not equal to 'p1'");
        verify( callParam2 === "p2", "2nd parameter is not equal to 'p2'");
        verify( callParam3 === "p3", "3rd parameter is not equal to 'p3'");
    }

    function test_postMetalCall_4()
    {
        callParam1 = ""; callParam2 = ""; callParam3 = ""; callParam4 = "";
        QxIcUiApi.postMetaCall( idRoot, "metaCall_4", "p1", "p2", "p3", "p4");
        QxIcUiApi.enterEventLoop();
        verify( callParam1 === "p1", "1st parameter is not equal to 'p1'");
        verify( callParam2 === "p2", "2nd parameter is not equal to 'p2'");
        verify( callParam3 === "p3", "3rd parameter is not equal to 'p3'");
        verify( callParam4 === "p4", "4th parameter is not equal to 'p4'");
    }

}
