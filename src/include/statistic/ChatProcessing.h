#ifndef CHATPROCESSING_H
#define CHATPROCESSING_H

#include <QThread>
#include <QDebug>
#include <QFile>

class ChatProcessing : public QThread
{
    Q_OBJECT

public:
    ///
    /// \brief ChatProcessing - Конструктор ChatProcessing.
    /// \param filePath - Путь к файлу с логами чата.
    ///
    explicit ChatProcessing(const QString& filePath);

public slots:
    ///
    /// \brief run - Запускает поток обработки чата.
    ///
    void run();

private slots:
    ///
    /// \brief processChatLine - Обработка единичной строки чата.
    /// \param line - Единичная строка чата.
    ///
    void processChatLine(QString& line);

signals:
    ///
    /// \brief coinsAdded - Сигнал, который вызывается при добавлении коинов.
    ///
    void coinsAdded(const int);

    ///
    /// \brief playerJoinedMatch - Сигнал, который вызывается при присоединении игрока к матчу.
    ///
    void playerJoinedMatch(const QString, const QString);

    ///
    /// \brief endMatch - Сигнал, который вызывается при перемещении игрока в лобби.
    ///
    void endMatch();


private:
    QString chatFilePath = "";      // Путь к файлу с логами чата
};

#endif // CHATPROCESSING_H
