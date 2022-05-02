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
    void sendRequest(QObject* sender, QString method);

private slots:
    ///
    /// \brief onRequestFinished - Обработчик сигнала finished объекта QNetworkAccessManager.
    /// \brief reply - Ответ на запрос. Необходимо удалить ВРУЧНУЮ.
    ///
    void onRequestFinished(QNetworkReply* reply);

signals:

private:
    RequestQueue* requestQueue;
    RequestSender* requestSender;
};

#endif // CLIENT_H
