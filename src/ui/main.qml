import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WindowTracker 1.0

import "qrc:/overlay"

ApplicationWindow {
    id: root

    property var overlayWindow

    width: 640
    height: 480
    visible: true
    title: qsTr("VimeAdvisor")

    WindowTracker {
        id: windowTracker
    }

    Button {
        anchors.centerIn: parent
        text: qsTr("Click me")

        property bool isOpened: false

        onClicked: {
            if (!isOpened) {
                showOverlay()
                isOpened = true
            } else {
                hideOverlay()
                isOpened = false
            }
        }
    }

    Component.onCompleted: {
        var component = Qt.createComponent("qrc:/overlay/GameOverlay.qml")
        overlayWindow = component.createObject()
        overlayWindow.hide()

        windowTracker.getWindowBorders()
    }

    Timer {
        id: timer
        interval: 1000
        repeat: true

        onTriggered: {
            windowTracker.getWindowParameters()
            windowTracker.getWindowFocus()
        }
    }

    Connections {
        target: overlayWindow

        function onDeactivateOverlay() {
            console.log("DEACTIVATE")
            windowTracker.setWindowFocus()
        }
    }

    Connections {
        target: windowTracker

        function onWindowPositionChanged(wX, wY) {
            overlayWindow.changePosition(wX, wY)
        }

        function onWindowSizeChanged(wHeight, wWidth) {
            overlayWindow.changeSize(wHeight, wWidth)
        }

        function onWindowOpenedChanged(isOpened) {
            if (isOpened) {
                overlayWindow.show()
            } else {
                overlayWindow.hide()
            }
        }

        function onWindowFocusChanged(hasFocus) {
            if (hasFocus) {
                overlayWindow.show()
            } else {
                overlayWindow.hide()
            }
        }

        function onWindowFullscreenChanged(isFullscreen) {
            overlayWindow.setFullscreen(isFullscreen)
        }

        function onNewWindowBorders(nBorderWidth, nTitleBarHeight) {
            overlayWindow.setWindowFrame(nTitleBarHeight, nBorderWidth)
        }
    }

    function showOverlay() {
        overlayWindow.changePosition(-32000, -32000)
        overlayWindow.show()
        timer.start()
    }

    function hideOverlay() {
        timer.stop()
        overlayWindow.hide()
        windowTracker.resetWindowTracker();
    }
}
