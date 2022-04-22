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
        console.log("Child change Position")
    }

    function changeSize(nHeight, nWidth) {
        height = nHeight
        width = nWidth
        console.log("Child change Size")
    }

    function minimize() {
        visible = false
        console.log("Child hide")
    }

    function appear() {
        visible = true
        console.log("Child show")
    }

    Rectangle {
        color: "#00000000"
        anchors.fill: parent
        radius: 5

        border.color: "red"
        border.width: 10

        opacity: 0.5
    }

    Text {
        text: "aboba"
        anchors.centerIn: parent
        color: "white"
    }
}
