import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WindowTracker 1.0

ApplicationWindow {
    id: root

    property var window

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
                window.show()
                isOpened = true
                console.log("1")
                timer.start()
            } else {
                //window.close()
                window.hide()
                isOpened = false
                console.log("2")
                timer.stop()
            }
        }
    }

    Component.onCompleted: {
        var component = Qt.createComponent("TestOverlayElement.qml")
        window = component.createObject(root)
        window.hide()
    }

    Timer {
        id: timer
        interval: 100
        repeat: true

        onTriggered: {
            windowTracker.getWindowParameters()
        }
    }

    Connections {
        target: windowTracker

        onNewWindowPosition: function(wX, wY) {
            window.changePosition(wX, wY)
            //console.log("WindowPositionChanged")
            //console.log(wX);
            //console.log(wY);
        }

        onNewWindowSize: function(wHeight, wWidth) {
            window.changeSize(wHeight, wWidth)
            //console.log("WindowSizeChanged")
            //console.log(wHeight);
            //console.log(wWidth);
        }

        onWindowMinimized: {
            window.hide()
            //console.log("WindowMinimized")
        }

        onWindowOpened: {
            window.show()
            //console.log("WindowOpened")
        }

        onWindowClosed: {
            window.appear()
            //console.log("WindowClosed")
        }
    }
}
