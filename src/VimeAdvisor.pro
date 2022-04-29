QT += quick

LIBS += -lUser32

CONFIG += c++17


HEADERS += \
    include/statistic/ChatProcessing.h \
    include/statistic/StatisticCollector.h \
    include/WindowTracker.h

SOURCES += \
    sources/statistic/ChatProcessing.cpp \
    sources/statistic/StatisticCollector.cpp \
    sources/WindowTracker.cpp \
    sources/main.cpp

RESOURCES += \
    assets/assets.qrc \
    ui/qml.qrc

INCLUDEPATH += $$PWD/include

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
