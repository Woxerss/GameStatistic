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
    /// \brief - Функция получает состояние фокуса отслеживаемого окна.
    ///
    void getWindowFocus();

    ///
    /// \brief getWindowParameters - Функция получает параметры отслеживаемого окна если они изменились.
    ///
    void getWindowParameters();

    ///
    /// \brief resetWindowTracker - Функция приводит отслеживание окна к начальному состоянию.
    /// \details Вызывается при выключении ИЛИ непосредственно перед началом отслеживания окна.
    ///
    void resetWindowTracker();
signals:
    ///
    /// \brief windowPositionChanged - Сигнал, который вызывается при изменении позиции отслеживаемого окна в QML.
    ///
    void windowPositionChanged(const int, const int);

    ///
    /// \brief windowSizeChanged - Сигнал, который вызывается при изменении размера отслеживаемого окна в QML.
    ///
    void windowSizeChanged(const int, const int);

    ///
    /// \brief windowOpenedChanged - Сигнал, который вызывается при открытии/закрытии отслеживаемого окна в QML.
    ///
    void windowOpenedChanged(const bool);

    ///
    /// \brief windowFocusChanged - Сигнал, который вызывается при изменения фокуса отслеживаемого окна в QML.
    ///
    void windowFocusChanged(const bool);

private:
    bool isOpened = false;              // Окно открыто
    bool hasFocus = true;               // Окно имеет фокус

    HWND hWnd = nullptr;                // Хэндл окна игры
    RECT rect = {0, 0, 0, 0};           // Координаты углов окна
};

#endif // WINDOWTRACKER_H
