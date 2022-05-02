import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WindowTracker 1.0

Window {
    id: root

    /* Параметры окон Windows */
    property int titleBarHeight: 0
    property int borderWidth: 0

    /* Параметры окна игры */
    property bool isGameOpened: false
    property bool isFullscreen: false

    /* Таймер обновления оверлея */
    property int updateOverlayTimerInterval: 1000

    color: "#00000000"

    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint /*| Qt.WindowTransparentForInput*/

    title: "VA_MainOverlay"

    WindowTracker {
        id: windowTracker
    }

//    Rectangle {
//        id: rect

//        color: "red"

//        opacity: 0.5

//        anchors {
//            fill: parent

//            margins: (isFullscreen) ? 0 : borderWidth
//            topMargin: (isFullscreen) ? 0 : titleBarHeight
//        }
//    }

    Button {
        text: "Test Button"

        onClicked: {
            windowTracker.setWindowFocus()
        }
    }

    Timer {
        id: overlayUpdateTimer
        interval: updateOverlayTimerInterval
        repeat: true

        onTriggered: {
            windowTracker.getWindowParameters()
            windowTracker.getWindowFocus()
        }
    }

    Connections {
        target: windowTracker

        function onWindowOpenedChanged(value) {
            console.log("Window opened changed")
            isGameOpened = value;
        }

        function onWindowPositionChanged(nX, nY) {
            console.log("Window Pos changed")
            x = nX;
            y = nY;
        }

        function onWindowSizeChanged(nHeight, nWidth) {
            console.log("Window Size changed")
            height = 100; //nHeight;
            width = 100; //nWidth;
        }

        function onWindowFocusChanged(value) {
            if (value === true) {
                root.show()
            } else {
                root.hide()
            }

            console.log("Window Focus changed")
        }

        function onWindowFullscreenChanged(value) {
            console.log("Window Fullscreen changed")
            isFullscreen = value
        }

        function onNewWindowBorders(nTitleBarHeight, nBorderWidth) {
            console.log("Window Borders changed")
            titleBarHeight = nTitleBarHeight
            borderWidth = nBorderWidth
        }
    }

    Component.onCompleted: {
        overlayUpdateTimer.start()
        console.log("Overlay started")
    }

    Component.onDestruction: {
        console.log("Overlay destructed")
    }
}
