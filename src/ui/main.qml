import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import StatisticCollector 1.0

import "qrc:/overlay"

ApplicationWindow {
    id: root

    property var overlayComponent
    property var mainOverlayWindow

    /* Сборщик статистики */
    property bool isOverlayRun: false
    property bool isStatisticRun: false
    property bool isChatProcessing: false

    width: 640
    height: 480
    visible: true
    title: qsTr("VimeAdvisor")

    SystemTrayIcon {
        id: tray

        visible: true

        icon.source: "qrc:/icons/VimeAdvisor.png"

        menu: Menu {
            MenuItem {
                text: (root.visible) ? qsTr("Свернуть") : qsTr("Показать")

                onTriggered: {
                    if (root.visible) {
                        hide()
                    } else {
                        show()
                    }
                }
            }

            MenuSeparator { }

            MenuItem {
                text: (isOverlayRun) ? qsTr("Выключить оверлей") : qsTr("Включить оверлей")
            }

            MenuSeparator { }

            MenuItem {
                text: qsTr("Закрыть")

                onTriggered: {
                    if (!root.visible) {
                        show()
                    }

                    close()
                }
            }
        }

        onActivated: {
            console.log(reason)

            if (reason === 2 || reason === 3) {
                if (!root.visible) {
                    root.show()
                }
            }
        }
    }

    StatisticCollector {
        id: statisticCollector
    }

    Rectangle {
        id: rectA

        height: 20
        width: 20

        radius: 10

        color: "red"

        anchors {
            right: parent.right
            top: parent.top

            margins: 20
        }

        Timer {
            id: colorTimer
            repeat: false

            interval: 50

            onTriggered: {
                rectA.color = "red"
            }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 5

        Button {
            height: 60
            width: 140

            opacity: (isOverlayRun) ? 0.5 : 1

            text: (isOverlayRun) ? qsTr("End overlay") : qsTr("Start overlay")

            onClicked: {
                if (!isOverlayRun) {
                    isOverlayRun = true
                    openOverlay()
                } else {
                    isOverlayRun = false
                    closeOverlay()
                }
            }
        }

        Button {
            height: 60
            width: 140

            opacity: (isChatProcessing) ? 0.5 : 1

            text: (isChatProcessing) ? qsTr("End chat processing") : qsTr("Start chat processing")

            onClicked: {
                if (!isChatProcessing) {
                    statisticCollector.startChatProcessing()
                    isChatProcessing = true
                } else {
                    statisticCollector.stopChatProcessing()
                    isChatProcessing = false
                }
            }
        }

        TextField {
            id: textField
            height: 60
            width: 200

            text: ""
        }

        Button {
            id: but1
            height: 60
            width: 140

            property int intA: 0

            opacity: (isChatProcessing) ? 0.5 : 1

            text: "SendRequest"

            onClicked: {
                statisticCollector.sendRequest(this, textField.text)
            }

            function onTransmitInformation(msg: string) : string {
                var JsonObject = JSON.parse(msg);
                console.log(intA++)

                rectA.color = "black"
                colorTimer.start()

                textField.text = "Total:" + JsonObject.total + "  Ann:" + JsonObject.separated.ann + "  Bb:" + JsonObject.separated.bb + "  Bp:" + JsonObject.separated.bp
            }
        }
    }

    Timer {
        id: closingTimer

        interval: 500
        repeat: true

        onTriggered: {
            if (!isStatisticRun) {
                close()
            }
        }
    }

//    Connections {
//        target: statisticCollector

//        function onFinished() {
//            console.log("QML FINISH")
//            isStatisticRun = false
//        }

//        function onStarted() {
//            console.log("QML STARTED")
//            isStatisticRun = true
//        }
//    }

    Component.onCompleted: {
        overlayComponent = Qt.createComponent("qrc:/overlay/MainOverlayWindow.qml")
        //statisticCollector.start()
    }

    onClosing: {
        close.accepted = !isStatisticRun

        closingTimer.start()

        if (isStatisticRun) {
            statisticCollector.stop()
        }
    }

    function openOverlay() {
        mainOverlayWindow = overlayComponent.createObject()
    }

    function closeOverlay() {
        mainOverlayWindow.destroy()
    }
}
