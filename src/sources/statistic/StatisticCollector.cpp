#include "statistic/StatisticCollector.h"

///
/// \brief run - Запускает поток обработки статистики.
///
void StatisticCollector::run() {
    qDebug() << "Statistic Collecotr Thread Run";

    /* Подключаем сигналы к слотам (Обрабатываем события чата) */
    connect(chatProcessing, SIGNAL(coinsAdded(const int)), this, SLOT(onCoinsAdded(const int)));
    connect(chatProcessing, SIGNAL(playerJoinedMatch(const QString, const QString)), this, SLOT(onPlayerJoinedMatch(QString, const QString)));
    connect(chatProcessing, SIGNAL(endMatch()), this, SLOT(onEndMatch()));

    while (1) {
        // Ждать 1 секунду
        QThread::msleep(1000);
    }

    exec();
}

void StatisticCollector::startChatProcessing() {
    if (!chatProcessingActive) {
        chatProcessingActive = true;
        chatProcessing->start();
    }
}

void StatisticCollector::stopChatProcessing() {
    if (chatProcessingActive) {
        chatProcessingActive = false;
        chatProcessing->requestInterruption();
    }
}


///
/// \brief onCoinsAdded - Обработчик добавления коинов.
/// \param coins - Количество полученных коинов.
///
void StatisticCollector::onCoinsAdded(const int coins) {
    sessionCoins += coins;
    qDebug() << "Received:" << coins << "Total:" << sessionCoins;
}

///
/// \brief onPlayerJoinedMatch - Обработчик присоединения игроков к матчу.
/// \param ratio - Соотношение [Присоединившиеся/Общее кол-во мест].
/// \param nickname - Никнейм игрока.
///
void StatisticCollector::onPlayerJoinedMatch(QString ratio, const QString nickname) {
    QStringList playersCount;
    playersCount = ratio.split('/');

    if (!isMatch) {
        isMatch = true;
        qDebug() << "MATCH STARTED";
    }

    qDebug() << playersCount[0] << playersCount[1] << nickname;
}

///
/// \brief onEndMatch - Обработчик конца матча.
///
void StatisticCollector::onEndMatch() {
    if (isMatch) {
        isMatch = false;
        qDebug() << "MATCH ENDED";
    } else {
        qDebug() << "Вы были перемещены в лобби";
    }
}
