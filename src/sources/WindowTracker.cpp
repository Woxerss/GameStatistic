#include "WindowTracker.h"
#include <QDebug>

WindowTracker::WindowTracker(QObject* parent)
    : QObject(parent)
{
}

///
/// \brief - Функция получает состояние фокуса окна
/// \return Возвращает 'true' если фокус на отслеживаемом окне, иначе 'false'
///
bool WindowTracker::getWindowFocus() {
    HWND tempHwnd = GetFocus ();

    if (tempHwnd != nullptr && tempHwnd != hWnd) {
        return true;
    }

    return false;
}

///
/// \brief getWindowParameters - Функция получает параметры окна игры
///
void WindowTracker::getWindowParameters() {
    RECT tempRect;

    hWnd = FindWindow(nullptr, _T(WINDOW_NAME));

    if (hWnd == nullptr) {
        if (isOpened) {
            isOpened = false;
            emit windowClosed();
        }

        return;
    }

    if (!isOpened) {
        isOpened = true;
        emit windowOpened();
    }

    if (GetWindowRect(hWnd, &tempRect)) {
        bool isEqual = true;

        // Проверка изменения положения окна
        if (rect.left != tempRect.left || rect.top != tempRect.top) {
            // Проверка сворачивания окна
            if (tempRect.left == -32000 && tempRect.top == -32000) {
                isMinimized = true;
                emit windowMinimized();
            } else {
                if (isMinimized) {
                    isMinimized = false;
                    emit windowOpened();
                }

                emit newWindowPosition(tempRect.left, tempRect.top);
            }

            isEqual = false;
        }

        // Проверка изменения размеров окна
        if (!isMinimized && (rect.right - rect.left != tempRect.right - tempRect.left
            || rect.bottom - rect.top != tempRect.bottom - tempRect.top)) {
            emit newWindowSize(tempRect.bottom - tempRect.top, tempRect.right - tempRect.left);

            isEqual = false;
        }

        if (!isEqual) {
            rect = tempRect;
        }

        rect = tempRect;
    }
}
