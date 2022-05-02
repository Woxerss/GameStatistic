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
/// \brief makeConnect - Связывает сигнал finished со слотом передаваемого объекта.
/// \param object - Объект, к которому необходимо привязать сигнал.
///
void RequestSender::makeConnection(QObject* object) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), object, SLOT(onRequestFinished(QNetworkReply*)));
}

///
/// \brief breakConnect - Разрывает связь сигнала finished со слотом передаваемого объекта.
/// \param object - Объект, от которого необходимо отвязать сигнал.
///
void RequestSender::breakConnection(QObject* object) {
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), object, SLOT(onRequestFinished(QNetworkReply*)));
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
        emit endRequest();

        if (!resetTimer->isActive()) {
            resetTimer->setInterval(1001 * reply->rawHeader("X-RateLimit-Reset-After").toInt());
            resetTimer->start();
        }

        return;
    }

    if (!resetTimer->isActive()) {
        resetTimer->setInterval(1001 * reply->rawHeader("X-RateLimit-Reset-After").toInt());
        resetTimer->start();
    }

    qDebug() << "Получен ответ на запрос: " << reply->rawHeader("X-RateLimit-Reset-After") <<
        reply->rawHeader("X-RateLimit-Remaining") << reply->rawHeader("X-RateLimit-Limit");

    emit endRequest();
}
