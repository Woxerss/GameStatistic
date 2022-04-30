#include "statistic/StatisticCollector.h"

///
/// \brief StatisticCollector - Конструктор StatisticCollector.
///
StatisticCollector::StatisticCollector() {

    logFile.setFileName("statistic.log");

    if (logFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream temp(&logFile);
        log.setDevice(&logFile);

        writeLog("INIT", "Statistic Collector log started");
    }

}

///
/// \brief run - Запускает поток обработки статистики.
/// \details Испускает сигнал started() при запуске.
///
void StatisticCollector::run() {
    isStatisticCollectorRunning = true;
    writeLog("THREAD", "Statistic Collector thread running");

    connect(chatProcessing, SIGNAL(finished()), this, SLOT(onChatProcessingFinished()));
    connect(chatProcessing, SIGNAL(started()), this, SLOT(onChatProcessingStarted()));

    while (isStatisticCollectorRunning || isChatProcessingRunning) {
        QThread::msleep(1000);
    }

    writeLog("THREAD", "Statistic Collector thread finished");
}

///
/// \brief stop - Останавливает поток обработки статистики.
/// \details Испускается сигнал finished() при остановке потока.
///
void StatisticCollector::stop() {
    if (isStatisticCollectorRunning) {
        writeLog("THREAD", "Statistic Collector thread requested to stop");
        isStatisticCollectorRunning = false;
    }

    if (isChatProcessingRunning) {
        writeLog("THREAD", "Chat Processing thread requested to stop");
        chatProcessing->requestInterruption();
    }
}

///
/// \brief startChatProcessing - Запускает поток обработки логов чата.
/// \details Испускается сигнал started() при запуске.
///
void StatisticCollector::startChatProcessing() {
    if (!isChatProcessingRunning) {
        writeLog("THREAD", "Chat Processing thread requested to start");
        chatProcessing->start();
    }
}

///
/// \brief stopChatProcessing - Завершает поток обработки чата.
/// \details Испускается сигнал finished() при остановке потока.
///
void StatisticCollector::stopChatProcessing() {
    if (isChatProcessingRunning) {
        writeLog("THREAD", "Chat Processing thread requested to stop");
        chatProcessing->requestInterruption();
    }
}

///
/// \brief writeLog - Записывает сообщение в лог файл.
/// \param type - Тип сообщения.
/// \param message - Текст сообщения.
///
void StatisticCollector::writeLog(const QString& type, const QString& message) {
    log << '[' << (QTime::currentTime()).toString("HH:mm:ss") << "] " << type << ": " << message << Qt::endl;
}

///
/// \brief onChatProcessingFinished - Обрабатывает сигнал finished() объекта ChatProcessing.
///
void StatisticCollector::onChatProcessingStarted() {
    writeLog("THREAD", "Chat Processing thread started");
    isChatProcessingRunning = true;
}

///
/// \brief onChatProcessingFinished - Обрабатывает сигнал started() объекта ChatProcessing.
///
void StatisticCollector::onChatProcessingFinished() {
    writeLog("THREAD", "Chat Processing thread finished");
    isChatProcessingRunning = false;
}
