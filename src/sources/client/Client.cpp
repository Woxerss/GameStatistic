#include "client/Client.h"

///
/// \brief Client - Конструктор Client.
///
Client::Client(QObject *parent)
    : QObject{parent}
{
    // Инициализируем клиент
    requestSender = new RequestSender();
    requestSender->makeConnection(this);

    // Присодиняйем логирование к отправителю запросов
    connect(requestSender, SIGNAL(newLog(const QString&,const QString&)), this->parent(), SLOT(writeLog(const QString&,const QString&)));

    // Инициализируем очередь запросов
    requestQueue = new RequestQueue();

    // Соединяйем очередь запросов и клиент
    connect(requestQueue, SIGNAL(newRequest(QObject*,QString)), requestSender, SLOT(sendRequest(QObject*,QString)));
    connect(requestSender, SIGNAL(reset()), requestQueue, SLOT(onReset()));
}

void Client::sendRequest(QObject* sender, QString method) {
    requestQueue->add(this, method);
}

///
/// \brief onRequestFinished - Обработчик сигнала finished объекта QNetworkAccessManager.
///
void Client::onRequestFinished(QNetworkReply* reply) {
    if (reply->request().originatingObject() == this) {

        if (reply->error()) {
            return;
        }

        QString answer = reply->readAll();
        qDebug() << answer;

        reply->deleteLater();
    }
}
