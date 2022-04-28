import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    x: -32000
    y: -32000

    // Window Borders
    property bool isFullscreen: false
    property int titleBarHeight: 0
    property int borderWidth: 0

    id: root

    color: "#00000000"

    flags: Qt.ToolTip | Qt.FramelessWindowHint | /*| Qt.X11BypassWindowManagerHint*/
            Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput

    title: "VimeAdvisor_Overlay_0"

    Rectangle {
        id: rect

        color: "red"

        opacity: 0.5

        anchors {
            fill: parent

            margins: (isFullscreen) ? 0 : borderWidth
            topMargin: (isFullscreen) ? 0 : titleBarHeight
        }
    }

    signal deactivateOverlay()

    function changePosition(nX, nY) {
        x = nX
        y = nY
    }

    function changeSize(nHeight, nWidth) {
        height = nHeight
        width = nWidth
    }

    function setWindowFrame(nTitleBarHeight, nBorderWidth) {
        titleBarHeight = nTitleBarHeight
        borderWidth = nBorderWidth
    }

    function setFullscreen(nIsFullscreen) {
        isFullscreen = nIsFullscreen
    }
}
