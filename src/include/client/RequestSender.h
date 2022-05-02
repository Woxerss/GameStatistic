#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QTimer>

class RequestSender : public QObject
{
    Q_OBJECT

public:
    ///
    /// \brief RequestSender - Конструктор RequestSender.
    ///
    explicit RequestSender(QObject *parent = nullptr);

public slots:
    ///
    /// \brief makeConnect - Связывает сигнал finished со слотом передаваемого объекта.
    /// \param object - Объект, к которому необходимо привязать сигнал.
    ///
    void makeConnection(QObject* object);

    ///
    /// \brief breakConnect - Разрывает связь сигнала finished со слотом передаваемого объекта.
    /// \param object - Объект, от которого необходимо отвязать сигнал.
    ///
    void breakConnection(QObject* object);

    ///
    /// \brief sendRequest - Посылает HTTP запрос.
    /// \param sender - Отправитель
    /// \param method - Метод запроса
    ///
    void sendRequest(QObject* sender, QString method);

private slots:
    ///
    /// \brief onRequestFinished - Обработчик сигнала finished объекта QNetworkAccessManager.
    /// \brief reply - Ответ на запрос. Необходимо удалить ВРУЧНУЮ.
    ///
    void onRequestFinished(QNetworkReply* reply);

signals:
    ///
    /// \brief endRequest - Сигнал, который вызывается при получении ответа QNetworkReply.
    ///
    void endRequest();

    ///
    /// \brief reset - Сигнал, который вызывается при обновлении таймера оставшихся запросов.
    ///
    void reset();

    ///
    /// \brief writeLog - Сигнал, который вызывается для занесения сообщеия в лог.
    ///
    void newLog(const QString& type, const QString& message);

private:
    QTimer *resetTimer;

    QString hostname = "https://api.vimeworld.com/";
    QNetworkAccessManager * manager;
};

#endif // REQUESTSENDER_H
