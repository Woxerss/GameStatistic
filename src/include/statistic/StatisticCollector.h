#ifndef STATISTICCOLLECTOR_H
#define STATISTICCOLLECTOR_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTime>

#include "statistic/ChatProcessing.h"

class StatisticCollector : public QThread
{
    Q_OBJECT

public:
    ///
    /// \brief StatisticCollector - Конструктор StatisticCollector.
    ///
    StatisticCollector();

public slots:
    ///
    /// \brief run - Запускает поток обработки статистики.
    /// \details Испускается сигнал started() при запуске.
    ///
    void run();

    ///
    /// \brief stop - Останавливает поток обработки статистики.
    /// \details Испускается сигнал finished() при остановке потока.
    ///
    void stop();

    ///
    /// \brief startChatProcessing - Запускает поток обработки логов чата.
    /// \details Испускается сигнал started() при запуске.
    ///
    void startChatProcessing();

    ///
    /// \brief stopChatProcessing - Завершает поток обработки чата.
    /// \details Испускается сигнал finished() при остановке потока.
    ///
    void stopChatProcessing();

private slots:
    ///
    /// \brief writeLog - Записывает сообщение в лог файл.
    /// \param type - Тип сообщения.
    /// \param message - Текст сообщения.
    ///
    void writeLog(const QString& type, const QString& message);

    ///
    /// \brief onChatProcessingFinished - Обрабатывает сигнал started() объекта ChatProcessing.
    ///
    void onChatProcessingStarted();

    ///
    /// \brief onChatProcessingFinished - Обрабатывает сигнал finished() объекта ChatProcessing.
    ///
    void onChatProcessingFinished();

private:
    QString chatLogFilePath = "C:/Users/Malee/AppData/Roaming/.vimeworld/minigames/logs/latest.log";
    ChatProcessing* chatProcessing = new ChatProcessing(chatLogFilePath);

    bool isStatisticCollectorRunning = false;           // Поток обработки статистики запущен
    bool isChatProcessingRunning = false;               // Поток обработки чата запущен

    QFile logFile;
    QTextStream log;
};

#endif // STATISTICCOLLECTOR_H
