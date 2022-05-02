#include "client/RequestSender.h"

///
/// \brief RequestSender - Конструктор RequestSender.
///
RequestSender::RequestSender(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));

    resetTimer = new QTimer();
    resetTimer->setSingleShot(true);
    resetTimer->start();

    connect(resetTimer, SIGNAL(timeout()), this, SIGNAL(reset()));
}

///
/// \brief sendRequest - Посылает HTTP запрос.
/// \param sender - Отправитель
/// \param method - Метод запроса
///
void RequestSender::sendRequest(QObject* sender, QString method) {
    QNetworkRequest request;

    request.setUrl(QUrl(hostname + method));
    request.setOriginatingObject(sender);
    manager->get(request);
}

///
/// \brief onRequestFinished - Обработчик сигнала finished объекта QNetworkAccessManager.
///
void RequestSender::onRequestFinished(QNetworkReply* reply) {
    if (reply->error()) {
        emit newLog("REQUEST", "ERROR OCCURED: " + reply->errorString());
        reply->deleteLater();

        emit endRequest();
        return;
    }

    if (!resetTimer->isActive()) {
        resetTimer->setInterval(1001 * reply->rawHeader("X-RateLimit-Reset-After").toInt());
        resetTimer->start();
    }

//    qDebug() << "Получен ответ на запрос: " << reply->rawHeader("X-RateLimit-Reset-After") <<
//        reply->rawHeader("X-RateLimit-Remaining") << reply->rawHeader("X-RateLimit-Limit");

    emit endRequest();
    emit newReply(reply);
}
