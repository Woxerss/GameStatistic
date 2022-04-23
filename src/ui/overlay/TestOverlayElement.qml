import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WindowTracker 1.0

ApplicationWindow {
    width: 10
    height: 10
    visible: true

    color: "#00000000"

    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | Qt.X11BypassWindowManagerHint
         | Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput

    title: qsTr("")

    function changePosition(nX, nY) {
        x = nX
        y = nY
    }

    function changeSize(nHeight, nWidth) {
        height = nHeight
        width = nWidth
    }

    Rectangle {
        color: "red"

        height: 20
        width: 20

        anchors {
            left: parent.left
            top: parent.top
        }
    }

    Rectangle {
        color: "red"

        height: 20
        width: 20

        anchors {
            right: parent.right
            top: parent.top
        }
    }

    Rectangle {
        color: "red"

        height: 20
        width: 20

        anchors {
            left: parent.left
            bottom: parent.bottom
        }
    }

    Rectangle {
        color: "red"

        height: 20
        width: 20

        anchors {
            right: parent.right
            bottom: parent.bottom
        }
    }
}
