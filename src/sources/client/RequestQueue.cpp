#include "client/RequestQueue.h"

///
/// \brief RequestQueue - Конструктор RequestQueue.
///
RequestQueue::RequestQueue(QObject *parent)
    : QThread{parent}
{
}

///
/// \brief run - Запускает поток обработки очереди запросов.
///
void RequestQueue::run() {
    bool isStop = false;

    while(!isStop) {
        while (requestQueue.size() > 0 && remainingRequest > 0) {
            emit newRequest(requestQueue[0].first, requestQueue[0].second);
            requestQueue.pop_front();
            remainingRequest--;
        }


        if (requestQueue.size() > 0 && remainingRequest == 0) {
            while(remainingRequest == 0) {
                QThread::msleep(1000);
            }
        } else {
            isStop = true;
        }
    }
}

///
/// \brief add - Добавляет http запрос в очередь.
/// \param sender - Отправитель запроса.
/// \param method - Метод запроса.
///
void RequestQueue::add(QObject* sender, QString method) {
    requestQueue.append({sender, method});

    if (!this->isRunning()) {
        this->start();
    }
}

///
/// \brief onReset - Обновляет число оставшихся запросов.
///
void RequestQueue::onReset() {
    remainingRequest = 60;
}
