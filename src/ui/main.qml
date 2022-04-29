import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import StatisticCollector 1.0

ApplicationWindow {
    id: root

    /* Сборщик статистики */
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
                //text: (isOverlayOpened) ? qsTr("Выключить оверлей") : qsTr("Включить оверлей")
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
    }

    StatisticCollector {
        id: statisticCollector
    }

    Column {
        anchors.centerIn: parent
        spacing: 5

        Button {
            height: 60
            width: 140

            //opacity: (isOverlayOpened) ? 0.5 : 1

            //text: (isOverlayOpened) ? qsTr("End overlay") : qsTr("Start overlay")

            onClicked: {
                if (!isOverlayOpened) {
                    showOverlay()
                    isOverlayOpened = true
                } else {
                    hideOverlay()
                    isOverlayOpened = false
                }
            }
        }

        Button {
            height: 60
            width: 140

            opacity: (isChatProcessing) ? 0.5 : 1

            //text: (isChatProcessing) ? qsTr("End chat processing") : qsTr("Start chat processing")

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

    Connections {
        target: statisticCollector

        function onFinished() {
            console.log("QML FINISH")
            isStatisticRun = false
        }

        function onStarted() {
            console.log("QML STARTED")
            isStatisticRun = true
        }
    }

    Component.onCompleted: {
        statisticCollector.start()
    }

    onClosing: {
        close.accepted = !isStatisticRun

        closingTimer.start()

        if (isStatisticRun) {
            statisticCollector.stop()
        }
    }
}
