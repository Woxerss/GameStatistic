import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WindowTracker 1.0

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
            Qt.WindowStaysOnTopHint /*| Qt.WindowTransparentForInput*/

    WindowTracker {
        id: windowTracker
    }

    title: "VimeAdvisor_Overlay_0"

    Rectangle {
        id: rect

        height: 100
        width: 100


        color: "red"

        opacity: 0.5

        anchors {
            centerIn: parent

            //fill: parent

            //margins: (isFullscreen) ? 0 : borderWidth
            //topMargin: (isFullscreen) ? 0 : titleBarHeight
        }
    }

    Button {
        anchors.right: rect.right
        anchors.top: rect.top

        text: "TRY TO CLICK ON ME DUDE"

        MouseArea {
            onClicked: {
                windowTracker.setWindowFocus()
                console.log("LOL BTW HOW Y DO THAT?")
            }

            anchors.fill: parent
        }
    }

    function changePosition(nX, nY) {
        x = nX
        y = nY
    }

    function changeSize(nHeight, nWidth) {
        height = 200//nHeight
        width = 300//nWidth
    }

    function setWindowFrame(nTitleBarHeight, nBorderWidth) {
        titleBarHeight = nTitleBarHeight
        borderWidth = nBorderWidth
    }

    function setFullscreen(nIsFullscreen) {
        isFullscreen = nIsFullscreen
    }
}
