#ifndef CLIENT_H
#define CLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QThread>

class Client : public QThread
{
    Q_OBJECT

public:
    ///
    /// \brief Client - Конструктор Client.
    ///
    explicit Client(QObject *parent = nullptr);

private slots:
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

private:
    QString hostname = "https://api.vimeworld.com/";
    QNetworkAccessManager * manager;
};

#endif // CLIENT_H
