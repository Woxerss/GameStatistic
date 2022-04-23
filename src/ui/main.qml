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
                timer.start()
                overlayWindow.show()
                isOpened = true
                console.log("OVERLAY ON")
            } else {
                timer.stop()
                overlayWindow.hide()
                windowTracker.resetWindowTracker();
                isOpened = false
                console.log("OVERLAY OFF")
            }
        }
    }

    Component.onCompleted: {
        var component = Qt.createComponent("qrc:/overlay/TestOverlayElement.qml")
        overlayWindow = component.createObject()
        overlayWindow.hide()
    }

    Timer {
        id: timer
        interval: 500
        repeat: true

        onTriggered: {
            windowTracker.getWindowParameters()
            windowTracker.getWindowFocus()
        }
    }

    Connections {
        target: windowTracker

        onWindowPositionChanged: function(wX, wY) {
            overlayWindow.changePosition(wX, wY)
        }

        onWindowSizeChanged: function(wHeight, wWidth) {
            overlayWindow.changeSize(wHeight, wWidth)
        }

        onWindowOpenedChanged: function(isOpened) {
            if (isOpened) {
                overlayWindow.show()
                console.log("WINDOW OPENED")
            } else {
                overlayWindow.hide()
                console.log("WINDOW CLOSED")
            }
        }

        onWindowFocusChanged: function(hasFocus) {
            if (hasFocus) {
                overlayWindow.show()
                console.log("HAS FOCUS")
            } else {
                overlayWindow.hide()
                console.log("FOCUS LOST")
            }
        }
    }
}
