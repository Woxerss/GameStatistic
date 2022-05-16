#include "client/Client.h"

///
/// \brief Client - Конструктор Client.
///
Client::Client(QObject *parent)
    : QObject{parent}
{
    // Инициализируем отправитель запросов
    requestSender = new RequestSender();

    // Связываем клиент и отправитель запросов
    connect(requestSender, SIGNAL(newReply(QNetworkReply*)), this, SLOT(onNewReply(QNetworkReply*)));

    // Присодиняйем логирование к отправителю запросов
    connect(requestSender, SIGNAL(newLog(const QString&,const QString&)), this->parent(), SLOT(writeLog(const QString&,const QString&)));

    // Инициализируем очередь запросов
    requestQueue = new RequestQueue();

    // Связываем очередь запросов и клиент
    connect(requestQueue, SIGNAL(newRequest(QObject*,QString)), requestSender, SLOT(sendRequest(QObject*,QString)));
    connect(requestSender, SIGNAL(reset()), requestQueue, SLOT(onReset()));
    connect(requestSender, SIGNAL(requestError()), requestQueue, SLOT(onRequestError()));
}

///
/// \brief onNewReply - Обработчик сигнала finished объекта QNetworkAccessManager.
///
void Client::onNewReply(QNetworkReply* reply) {
    // Присоединяем сигнал передачи информации к слоту объекта отправителя запроса
    connect(this, SIGNAL(transmitInformation(QString)), reply->request().originatingObject(), SLOT(onTransmitInformation(QString)));

    // Вызываем сигнал передачи информации
    emit transmitInformation(reply->readAll());

    // Отсоединяем сигнал передачи информации к слоту объекта отправителя запроса
    disconnect(this, SIGNAL(transmitInformation(QString)), reply->request().originatingObject(), SLOT(onTransmitInformation(QString)));

    reply->deleteLater();
}

///
/// \brief getUsersByName - Получение информации об игроке или нескольких игроках по их нику.
/// \param sender - Объект-отправитель запроса
/// \param nameList - Список ников
/// \details Максимальное количество ников 50
///
void Client::getUsersByName(QObject* sender, const QList<QString>& nameList) {
    QString method = "user/name/";

    if (nameList.size() > 1 && nameList.size() <= 50) {
        foreach (QString name, nameList) {
            method.append(name + ',');
        }
    } else {
        method.append(nameList[0]);
    }

    requestQueue->add(sender, method);
}

///
/// \brief getUsersById - Получение информации об игроке или нескольких игроках по их id.
/// \param sender - Объект-отправитель запроса
/// \param idList - Список id
///
void Client::getUsersById(QObject* sender, const QList<QString>& idList) {
    QString method = "user/";

    if (idList.size() > 1 && idList.size() <= 50) {
        foreach (QString id, idList) {
            method.append(id + ',');
        }
    } else {
        method.append(idList[0]);
    }

    requestQueue->add(sender, method);
}

///
/// \brief getFriends - Получение списка друзей игрока.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
///
void Client::getFriends(QObject* sender, const QString& id) {
    QString method = "user/";
    method.append(id);
    method.append("/friends");

    requestQueue->add(sender, method);
}

///
/// \brief getUserSession - Получает статус игрока онлайн, человекопонятное сообщение и название игры, где он находится.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
///
void Client::getUserSession(QObject* sender, const QString& id) {
    QString method = "user/";
    method.append(id);
    method.append("/session");

    requestQueue->add(sender, method);
}

///
/// \brief getUserStats - Получает статистику всех игр, в которые играл запрашиваемый игрок.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
/// \param gameList - список игр.
///
void Client::getUserStats(QObject* sender, const QString& id, const QList<QString> gameList) {
    QString method = "user/";
    method.append(id);
    method.append("/stats");

    if (gameList.size() > 0) {
        method.append("?games=");

        foreach (QString game, gameList) {
            method.append(game + ',');
        }
    }

    requestQueue->add(sender, method);
}

///
/// \brief getUserAchievements - Получает список всех достижений игрока.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
///
void Client::getUserAchievements(QObject* sender, const QString& id) {
    QString method = "user/";
    method.append(id);
    method.append("/achievements");

    requestQueue->add(sender, method);
}

///
/// \brief getUserLeaderboards - Получает список таблиц рекордов, куда попал данный игрок, и место в них.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
///
void Client::getUserLeaderboards(QObject* sender, const QString& id) {
    QString method = "user/";
    method.append(id);
    method.append("/leaderboards");

    requestQueue->add(sender, method);
}

///
/// \brief getUserMatches - Получает список последних матчей игрока.
/// \param sender - Объект-отправитель запроса
/// \param id - ID игрока
/// \param parameter - Параметр запроса.
/// \param value - Значение параметра.
///
void Client::getUserMatches(QObject* sender, const QString& id, const QString& parameter, const int& value) {
    QString method = "user/";
    method.append(id);
    method.append("/matches");

    if (parameter != "") {
        method.append('?' + parameter + '=' + QString::number(value));
    }

    requestQueue->add(sender, method);
}

///
/// \brief getMassSession - Получает статус игроков онлайн, человекопонятное сообщение и название игры, где каждый из них находится.
/// \param sender - Объект-отправитель запроса
/// \param idList - список ID
///
void Client::getMassSession(QObject* sender, const QList<QString>& idList) {
    QString method = "user/session/";

    if (idList.size() > 1 && idList.size() <= 50) {
        foreach (QString id, idList) {
            method.append(id + ',');
        }
    } else {
        method.append(idList[0]);
    }

    requestQueue->add(sender, method);
}

///
/// \brief getGuild - Получить информации о гильдии по ее id.
/// \param sender - Объект-отправитель запроса
/// \param id - ID гильдии.
///
void Client::getGuild(QObject* sender, const QString& id) {
    QString method = "guild/get?id=";
    method.append(id);

    requestQueue->add(sender, method);
}

///
/// \brief getLeaderboard - Получает список всех доступных таблиц рекордов.
/// \param sender - Объект-отправитель запроса
/// \param type - Тип таблицы рекордов
/// \param sort - Краткое описание таблицы рекордов
/// \param size - Максимальное количество записей, которые вернет сервер
/// \param offset - Список доступных вариантов таблицы рекордов
///
void Client::getLeaderboard(QObject* sender, const QString& type, const QString& sort, const int& size, const int& offset) {
    QString method = "leaderboard/get/";
    method.append(type + '/' + sort + "?size=" + QString::number(size) + "&offset=" + QString::number(offset));

    requestQueue->add(sender, method);
}

///
/// \brief getOnline - Получает количество игроков онлайн. В сумме и по каждой игре отдельно.
/// \param sender - Объект-отправитель запроса
///
void Client::getOnline(QObject* sender) {
    QString method = "online";

    requestQueue->add(sender, method);
}

///
/// \brief getOnlineStreams - Получает список активных стримов на сервере.
/// \param sender - Объект-отправитель запроса
///
void Client::getOnlineStreams(QObject* sender) {
    QString method = "online/streams";

    requestQueue->add(sender, method);
}

///
/// \brief getOnlineStaff - Получает список модераторов, которые в данный момент находятся на сервере.
/// \param sender - Объект-отправитель запроса
///
void Client::getOnlineStaff(QObject* sender) {
    QString method = "online/staff";

    requestQueue->add(sender, method);
}

///
/// \brief getMatchById - Получает информацию о матче.
/// \param sender - Объект-отправитель запроса
/// \param id - ID матча
///
void Client::getMatchById(QObject* sender, const QString& id) {
    QString method = "match/";
    method.append(id);

    requestQueue->add(sender, method);
}

///
/// \brief getMatchLatest - Получает последние матчи на сервере.
/// \param sender - Объект-отправитель запроса
/// \param count - Количество последних матчей, которые вернет сервер. По умолчанию - 20, максимально - 100
///
void Client::getMatchLatest(QObject* sender, const int& count) {
    QString method = "match/latest?count=";
    method.append(QString::number(count));

    requestQueue->add(sender, method);
}

///
/// \brief getMatchList - Получает список матчей.
/// \param sender - Объект-отправитель запроса
/// \param before - Будут выведены все матчи, которые произошли до матча с этим ID
/// \param after - Будут выведены все матчи, которые произошли после матча с этим ID
/// \param count - Количество матчей, которые вернет сервер. По умолчанию - 20, максимально - 100
///
void Client::getMatchList(QObject* sender, const QString& before, const QString& after, const int& count) {
    QString method = "match/list?";

    if (before != 0) {
        method.append("before=" + before);
    }

    if (after != 0) {
        method.append("after=" + after);
    }

    method.append(method.append("&count=" + QString::number(count)));

    requestQueue->add(sender, method);
}

///
/// \brief getLocaleName - Получает человекочитаемые названия игр, статистики, рангов.
/// \param sender - Объект-отправитель запроса
/// \param locale - Короткое имя языка, в данный момент доступен только 'ru'
///
void Client::getLocaleName(QObject* sender, const QString& locale) {
    QString method = "locale/";
    method.append(locale);

    requestQueue->add(sender, method);
}

///
/// \brief getMiscGames - Получает список всех игр, которые поддерживаются API.
/// \param sender - Объект-отправитель запроса
///
void Client::getMiscGames(QObject* sender) {
    QString method = "misc/games";

    requestQueue->add(sender, method);
}

///
/// \brief getMiscMaps - Получает список всех карт на VimeWorld.
/// \param sender - Объект-отправитель запроса
///
void Client::getMiscMaps(QObject* sender) {
    QString method = "misc/maps";

    requestQueue->add(sender, method);
}

///
/// \brief getMiscAchievements - Получает список всех возможных достижений с их описанием, кроме секретных.
/// \param sender - Объект-отправитель запроса
///
void Client::getMiscAchievements(QObject* sender) {
    QString method = "misc/achievements";

    requestQueue->add(sender, method);
}

///
/// \brief getMiscToken - Получает информацию о токене.
/// \param sender - Объект-отправитель запроса
/// \param token - Токен
///
void Client::getMiscToken(QObject* sender, const QString& token) {
    QString method = "misc/token/";
    method.append(token);

    requestQueue->add(sender, method);
}
