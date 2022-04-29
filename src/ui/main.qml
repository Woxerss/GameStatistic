import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import WindowTracker 1.0
import StatisticCollector 1.0

import "qrc:/overlay"

ApplicationWindow {
    id: root

    /* Оверлей */
    property var overlayMainWindow
    property bool isOverlayOpened: false

    /* Сборщик статистики */
    property bool isChatProcessing: false

    width: 640
    height: 480
    visible: true
    title: qsTr("VimeAdvisor")

    /* Меню в системном трее */
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
                text: (isOverlayOpened) ? qsTr("Выключить оверлей") : qsTr("Включить оверлей")
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

    /* Отслеживание окна игры */
    WindowTracker {
        id: windowTracker
    }

    /* Ассинхронный сборщик статистики */
    StatisticCollector {
        id: statisticCollector
    }

    /* Основное окно программы */
    Column {
        anchors.centerIn: parent
        spacing: 5

        Button {
            height: 60
            width: 140

            opacity: (isOverlayOpened) ? 0.5 : 1

            text: (isOverlayOpened) ? qsTr("End overlay") : qsTr("Start overlay")

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
    }

    /* Инициализация при загрузке компонента */
    Component.onCompleted: {
        /* Инициализация сборщика статистики */
        statisticCollector.start()

        console.log("Component on Completed")
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
        target: overlayMainWindow

        function onDeactivateOverlay() {
            windowTracker.setWindowFocus()
        }
    }

    Connections {
        target: windowTracker

        function onWindowPositionChanged(wX, wY) {
            overlayMainWindow.changePosition(wX, wY)
        }

        function onWindowSizeChanged(wHeight, wWidth) {
            overlayMainWindow.changeSize(wHeight, wWidth)
        }

        function onWindowOpenedChanged(isOpened) {
            if (isOpened) {
                overlayMainWindow.show()
            } else {
                overlayMainWindow.hide()
            }
        }

        function onWindowFocusChanged(hasFocus) {
            if (hasFocus) {
                overlayMainWindow.show()
            } else {
                overlayMainWindow.hide()
            }
        }

        function onWindowFullscreenChanged(isFullscreen) {
            overlayMainWindow.setFullscreen(isFullscreen)
        }

        function onNewWindowBorders(nBorderWidth, nTitleBarHeight) {
            overlayMainWindow.setWindowFrame(nTitleBarHeight, nBorderWidth)
        }
    }



    function showOverlay() {
        /* Инициализация оверлея */
        windowTracker.getWindowBorders()
        var component = Qt.createComponent("qrc:/overlay/GameOverlay.qml")
        overlayMainWindow = component.createObject()
        overlayMainWindow.changePosition(-32000, -32000)
        overlayMainWindow.show()
        timer.start()
    }

    function hideOverlay() {
        timer.stop()
        overlay = Component.destruction()
        //overlayMainWindow.hide()
        //windowTracker.resetWindowTracker();
    }
}
