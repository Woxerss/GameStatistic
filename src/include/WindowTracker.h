#ifndef WINDOWTRACKER_H
#define WINDOWTRACKER_H

#include <QObject>
#include <windows.h>
#include <tchar.h>

#define WINDOW_NAME "VimeWorld"

class WindowTracker : public QObject {
    Q_OBJECT

public:
    explicit WindowTracker(QObject* parent = nullptr);

public slots:
    ///
    /// \brief - Функция получает состояние фокуса окна
    /// \return Возвращает 'true' если фокус на отслеживаемом окне, иначе 'false'
    ///
    bool getWindowFocus();

    ///
    /// \brief getWindowParameters - Функция получает параметры окна
    ///
    void getWindowParameters();

signals:
    ///
    /// \brief newWindowPosition - Сигнал, который вызывается при изменении позиции отслеживаемого окна в QML
    ///
    void newWindowPosition(const int, const int);

    ///
    /// \brief newWindowSize - Сигнал, который вызывается при изменении размера отслеживаемого окна в QML
    ///
    void newWindowSize(const int, const int);

    ///
    /// \brief windowHidden - Сигнал, который вызывается при сворачивании окна в QML
    ///
    void windowMinimized();

    ///
    /// \brief windowOpened - Сигнал, который вызывается при разворачивании/открытия окна в QML
    ///
    void windowOpened();

    ///
    /// \brief gameClosed - Сигнал, который вызывается при закрытии окна в QML
    ///
    void windowClosed();

private:
    bool isOpened = false;               // Окно открыто
    bool isMinimized = false;            // Окно свернуто
    HWND hWnd = nullptr;                 // Хэндл окна игры
    RECT rect = {0, 0, 0, 0};            // Координаты углов окна
};

#endif // WINDOWTRACKER_H
