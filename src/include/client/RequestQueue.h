#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTimer>

class RequestQueue : public QThread
{
    Q_OBJECT
public:
    ///
    /// \brief RequestQueue - Конструктор RequestQueue.
    ///
    explicit RequestQueue(QObject *parent = nullptr);

public slots:
    ///
    /// \brief add - Добавляет http запрос в очередь.
    /// \param sender - Отправитель запроса.
    /// \param method - Метод запроса.
    ///
    void add(QObject* sender, QString method);

private slots:
    ///
    /// \brief run - Запускает поток обработки очереди запросов.
    ///
    void run();

    ///
    /// \brief onReset - Обновляет число оставшихся запросов.
    ///
    void onReset();

signals:
    ///
    /// \brief newRequest - Сигнал, который вызывается при передаче нового http запроса.
    ///
    void newRequest(QObject* sender, QString method);

private:
    QList<QPair<QObject*, QString>> requestQueue;

    int remainingRequest = 60;
    int resetTime = 0;
};

#endif // REQUESTQUEUE_H
