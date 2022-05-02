#include "client/Client.h"

///
/// \brief Client - Конструктор Client.
///
Client::Client(QObject *parent)
    : QThread{parent} {
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));

    sendRequest(this, "user/4167839/matches");
    sendRequest(this, "user/4167839/matches");

    sendRequest(this, "user/134568/session");
    sendRequest(this, "user/134568/session");

    sendRequest(this, "user/134568/stats");
    sendRequest(this, "user/134568/stats");

    sendRequest(this, "user/134568/achievements");
    sendRequest(this, "user/134568/achievements");

    sendRequest(this, "user/943030/leaderboards");
    sendRequest(this, "user/943030/leaderboards");
}


///
/// \brief makeConnect - Связывает сигнал finished со слотом передаваемого объекта.
/// \param object - Объект, к которому необходимо привязать сигнал.
///
void Client::makeConnection(QObject* object) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), object, SLOT(onRequestFinished(QNetworkReply*)));
}

///
/// \brief breakConnect - Разрывает связь сигнала finished со слотом передаваемого объекта.
/// \param object - Объект, от которого необходимо отвязать сигнал.
///
void Client::breakConnection(QObject* object) {
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), object, SLOT(onRequestFinished(QNetworkReply*)));
}

///
/// \brief sendRequest - Посылает HTTP запрос.
/// \param sender - Отправитель
/// \param method - Метод запроса
///
void Client::sendRequest(QObject* sender, QString method) {
    QEventLoop loop;

    connect(this, SIGNAL(endRequest()), &loop, SLOT(quit()));

    QNetworkRequest request;

    request.setUrl(QUrl(hostname + method));
    request.setOriginatingObject(sender);
    manager->get(request);

    qDebug() << "Послан HTTP запрос с методом: " << method;

    loop.exec();
}

///
/// \brief onRequestFinished - Обработчик сигнала finished объекта QNetworkAccessManager.
///
void Client::onRequestFinished(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "REQUEST ERROR OCCURED: " << reply->errorString();
        emit endRequest();
        return;
    }

    //qDebug() << "Получен ответ на запрос: " << reply->rawHeader("X-RateLimit-Reset-After") <<
    //    reply->rawHeader("X-RateLimit-Remaining") << reply->rawHeader("X-RateLimit-Limit");

    if (reply->rawHeader("X-RateLimit-Remaining").toInt() == 0) {
        qDebug() << "Requests Wait: " << reply->rawHeader("X-RateLimit-Reset-After").toInt();
        QThread::msleep(1010 * reply->rawHeader("X-RateLimit-Reset-After").toInt());
        qDebug() << "END Wait";
    }

    reply->deleteLater();

    emit endRequest();

    this->exit(1);
}
