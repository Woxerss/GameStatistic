#include "WindowTracker.h"

WindowTracker::WindowTracker(QObject* parent)
    : QObject(parent)
{
    screenHeight = GetSystemMetrics(SM_CXSCREEN);
    screenWidth = GetSystemMetrics(SM_CYSCREEN);
}

///
/// \brief - Функция получает состояние фокуса отслеживаемого окна.
///
void WindowTracker::getWindowFocus() {
    HWND tempHwnd = GetForegroundWindow();

    if (tempHwnd == hWnd || tempHwnd == FindWindow(nullptr, _T("VA_MainOverlay"))) {
        if (!hasFocus) {
            emit windowFocusChanged(true);
            hasFocus = true;
        }

        return;
    }

    if (hasFocus) {
        emit windowFocusChanged(false);
        hasFocus = false;
    }
}

///
/// \brief setWindowFocus - Функция устанавливает фокус на отслеживаемое окно.
///
void WindowTracker::setWindowFocus() {
    HWND hWNd = FindWindow(nullptr, _T(WINDOW_NAME));

    if (hWNd != nullptr) {
        SetForegroundWindow(hWNd);
    }
}

///
/// \brief getWindowParameters - Функция получает параметры отслеживаемого окна если они изменились.
///
void WindowTracker::getWindowParameters() {
    RECT tempRect;

    hWnd = FindWindow(nullptr, _T(WINDOW_NAME));

    // Проверка открытия/закрытия окна
    if (hWnd == nullptr) {
        if (isOpened) {
            emit windowOpenedChanged(false);
            isOpened = false;
        }

        return;
    }

    if (!isOpened) {
        emit windowOpenedChanged(true);
        isOpened = true;
    }

    // Проверка изменения параметров окна
    if (GetWindowRect(hWnd, &tempRect)) {
        bool isEqual = true;

        // Проверка изменения положения окна (X, Y)
        if (rect.left != tempRect.left || rect.top != tempRect.top) {
            emit windowPositionChanged(tempRect.left, tempRect.top);
            isEqual = false;
        }

        // Проверка изменения размеров окна (Height, Width)
        if (rect.right - rect.left != tempRect.right - tempRect.left ||
                rect.bottom - rect.top != tempRect.bottom - tempRect.top) {
            emit windowSizeChanged(tempRect.bottom - tempRect.top, tempRect.right - tempRect.left);
            isEqual = false;
        }

        // Проверка на полноэкранный режим
        if (tempRect.right - tempRect.left == screenHeight &&
                tempRect.bottom - tempRect.top == screenWidth) {
            if (!isFullscreen) {
                emit windowFullscreenChanged(true);
                isFullscreen = true;
            }
        } else {
            if (isFullscreen) {
                emit windowFullscreenChanged(false);
                isFullscreen = false;
            }
        }

        if (!isEqual) {
            rect = tempRect;
        }
    }
}

///
/// \brief resetWindowTracker - Функция приводит отслеживание окна к начальному состоянию.
/// \details Вызывается при выключении ИЛИ непосредственно перед началом отслеживания окна.
///
void WindowTracker::resetWindowTracker() {
    isOpened = false;              // Окно открыто
    hasFocus = true;               // Окно имеет фокус
    hWnd = nullptr;                // Хэндл окна игры
    rect = {0, 0, 0, 0};           // Координаты углов окна
}

///
/// \brief getWindowBorders - Функция получает размеры рамки окна в Windows.
///
void WindowTracker::getWindowBorders() {
    emit newWindowBorders(GetSystemMetrics(SM_CYFRAME), GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION));
}

