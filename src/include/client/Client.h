#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "client/RequestQueue.h"
#include "client/RequestSender.h"

class Client : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Client - Конструктор Client.
    ///
    explicit Client(QObject *parent = nullptr);

public slots:
    ///
    /// \brief getUsersByName - Получение информации об игроке или нескольких игроках по их нику.
    /// \param sender - Объект-отправитель запроса
    /// \param nameList - Список ников
    /// \details Максимальное количество ников 50
    ///
    void getUsersByName(QObject* sender, const QList<QString>& nameList);

    ///
    /// \brief getUsersById - Получение информации об игроке или нескольких игроках по их id.
    /// \param sender - Объект-отправитель запроса
    /// \param idList - Список id
    ///
    void getUsersById(QObject* sender, const QList<QString>& idList);

    ///
    /// \brief getFriends - Получение списка друзей игрока.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    ///
    void getFriends(QObject* sender, const QString& id);

    ///
    /// \brief getUserSession - Получает статус игрока онлайн, человекопонятное сообщение и название игры, где он находится.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    ///
    void getUserSession(QObject* sender, const QString& id);

    ///
    /// \brief getUserStats - Получает статистику всех игр, в которые играл запрашиваемый игрок.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    /// \param gameList - список игр.
    ///
    void getUserStats(QObject* sender, const QString& id, const QList<QString> gameList);

    ///
    /// \brief getUserAchievements - Получает список всех достижений игрока.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    ///
    void getUserAchievements(QObject* sender, const QString& id);

    ///
    /// \brief getUserLeaderboards - Получает список таблиц рекордов, куда попал данный игрок, и место в них.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    ///
    void getUserLeaderboards(QObject* sender, const QString& id);

    ///
    /// \brief getUserMatches - Получает список последних матчей игрока.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID игрока
    /// \param count - Количество матчей, которые вернет сервер. По умолчанию - 20, максимально - 50
    /// \param offset - Количество пропущенных от начала матчей. По умолчанию - 0, максимально - 2000
    /// \param after - Будут выведены матчи, которые произошли до матча с этим ID
    /// \param before - Будут выведены матчи, которые произошли после матча с этим ID
    ///
    void getUserMatches(QObject* sender, const QString& id, const QString& parameter, const int& value);

    ///
    /// \brief getMassSession - Получает статус игроков онлайн, человекопонятное сообщение и название игры, где каждый из них находится.
    /// \param sender - Объект-отправитель запроса
    /// \param idList - список ID
    ///
    void getMassSession(QObject* sender, const QList<QString>& idList);

    ///
    /// \brief getGuild - Получить информации о гильдии по ее id.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID гильдии.
    ///
    void getGuild(QObject* sender, const QString& id);

    ///
    /// \brief getLeaderboard - Получает список всех доступных таблиц рекордов.
    /// \param sender - Объект-отправитель запроса
    /// \param type - Тип таблицы рекордов
    /// \param sort - Краткое описание таблицы рекордов
    /// \param size - Максимальное количество записей, которые вернет сервер
    /// \param offset - Список доступных вариантов таблицы рекордов
    ///
    void getLeaderboard(QObject* sender, const QString& type, const QString& sort, const int& size, const int& offset);

    ///
    /// \brief getOnline - Получает количество игроков онлайн. В сумме и по каждой игре отдельно.
    /// \param sender - Объект-отправитель запроса
    ///
    void getOnline(QObject* sender);

    ///
    /// \brief getOnlineStreams - Получает список активных стримов на сервере.
    /// \param sender - Объект-отправитель запроса
    ///
    void getOnlineStreams(QObject* sender);

    ///
    /// \brief getOnlineStaff - Получает список модераторов, которые в данный момент находятся на сервере.
    /// \param sender - Объект-отправитель запроса
    ///
    void getOnlineStaff(QObject* sender);

    ///
    /// \brief getMatchById - Получает информацию о матче.
    /// \param sender - Объект-отправитель запроса
    /// \param id - ID матча
    ///
    void getMatchById(QObject* sender, const QString& id);

    ///
    /// \brief getMatchLatest - Получает последние матчи на сервере.
    /// \param sender - Объект-отправитель запроса
    /// \param count - Количество последних матчей, которые вернет сервер. По умолчанию - 20, максимально - 100
    ///
    void getMatchLatest(QObject* sender, const int& count);

    ///
    /// \brief getMatchList - Получает список матчей.
    /// \param sender - Объект-отправитель запроса
    /// \param before - Будут выведены все матчи, которые произошли до матча с этим ID
    /// \param after - Будут выведены все матчи, которые произошли после матча с этим ID
    /// \param count - Количество матчей, которые вернет сервер. По умолчанию - 20, максимально - 100
    ///
    void getMatchList(QObject* sender, const QString& before, const QString& after, const int& count);

    ///
    /// \brief getLocaleName - Получает человекочитаемые названия игр, статистики, рангов.
    /// \param sender - Объект-отправитель запроса
    /// \param locale - Короткое имя языка, в данный момент доступен только 'ru'
    ///
    void getLocaleName(QObject* sender, const QString& locale);

    ///
    /// \brief getMiscGames - Получает список всех игр, которые поддерживаются API.
    /// \param sender - Объект-отправитель запроса
    ///
    void getMiscGames(QObject* sender);

    ///
    /// \brief getMiscMaps - Получает список всех карт на VimeWorld.
    /// \param sender - Объект-отправитель запроса
    ///
    void getMiscMaps(QObject* sender);

    ///
    /// \brief getMiscAchievements - Получает список всех возможных достижений с их описанием, кроме секретных.
    /// \param sender - Объект-отправитель запроса
    ///
    void getMiscAchievements(QObject* sender);

    ///
    /// \brief getMiscToken - Получает информацию о токене.
    /// \param sender - Объект-отправитель запроса
    /// \param token - Токен
    ///
    void getMiscToken(QObject* sender, const QString& token);


private slots:
    ///
    /// \brief onNewReply - Обработчик сигнала finished объекта QNetworkAccessManager.
    /// \brief reply - Ответ на запрос. Необходимо удалить ВРУЧНУЮ
    ///
    void onNewReply(QNetworkReply* reply);

private:
    RequestQueue* requestQueue;
    RequestSender* requestSender;
};

#endif // CLIENT_H
