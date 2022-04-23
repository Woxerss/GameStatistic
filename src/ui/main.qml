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

    Button {
        onClicked: popup.open()

        Popup {
            id: popup

            parent: Overlay.overlay

            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
            width: 100
            height: 100
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
        target: windowTracker

        onWindowPositionChanged: function(wX, wY) {
            popup.x = wX
            popup.y = wY
            overlayWindow.changePosition(wX, wY)
        }

        onWindowSizeChanged: function(wHeight, wWidth) {
            overlayWindow.changeSize(wHeight, wWidth)
        }

        onWindowOpenedChanged: function(isOpened) {
            if (isOpened) {
                overlayWindow.show()
            } else {
                overlayWindow.hide()
            }
        }

        onWindowFocusChanged: function(hasFocus) {
            if (hasFocus) {
                overlayWindow.show()
            } else {
                overlayWindow.hide()
            }
        }

        onWindowFullscreenChanged: function(isFullscreen) {
            overlayWindow.setFullscreen(isFullscreen)
        }

        onNewWindowBorders: function(nBorderWidth, nTitleBarHeight) {
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
