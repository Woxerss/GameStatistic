#ifndef STATISTICCOLLECTOR_H
#define STATISTICCOLLECTOR_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QFile>

#include "statistic/ChatProcessing.h"

class StatisticCollector : public QThread
{
    Q_OBJECT

public slots:
    ///
    /// \brief run - Запускает поток обработки статистики.
    ///
    void run();

    ///
    /// \brief stop - Останавливает поток сбора статистики.
    ///
    void stop();

    ///
    /// \brief startChatProcessing - Запускает поток обработки логов чата.
    ///
    void startChatProcessing();

    ///
    /// \brief stopChatProcessing - Завершает поток обработки чата.
    ///
    void stopChatProcessing();

private slots:

    void onChatProcessingFinished();

    void onChatProcessingStarted();

    ///
    /// \brief onCoinsAdded - Обработчик добавления коинов.
    /// \param coins - Количество полученных коинов.
    ///
    void onCoinsAdded(const int coins);

    ///
    /// \brief onPlayerJoinedMatch - Обработчик присоединения игроков к матчу.
    /// \param ratio - Соотношение [Присоединившиеся/Общее кол-во мест].
    /// \param nickname - Никнейм игрока.
    ///
    void onPlayerJoinedMatch(QString ratio, const QString nickname);

    ///
    /// \brief onEndMatch - Обработчик конца матча.
    ///
    void onEndMatch();

private:
    bool isRunning = false;
    bool chatProcessingRunning = false;              // Поток обработки чата активен.

    QString chatLogFilePath = "C:/Users/Malee/AppData/Roaming/.vimeworld/minigames/logs/latest.log";

    ChatProcessing* chatProcessing = new ChatProcessing(chatLogFilePath);

    int sessionCoins = 0;       // Количество коинов за сессию
    int isMatch = false;        // Сейчас идет матч
};

#endif // STATISTICCOLLECTOR_H
