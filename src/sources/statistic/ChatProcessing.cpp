#include "statistic/ChatProcessing.h"

///
/// \brief ChatProcessing - Конструктор ChatProcessing.
/// \param filePath - Путь к файлу с логами чата.
///
ChatProcessing::ChatProcessing(const QString& filePath)
    : chatFilePath(filePath)
{
}

///
/// \brief run - Запускает поток обработки чата.
///
void ChatProcessing::run() {
    if (!QFile::exists(chatFilePath)) {
        qDebug() << "ERROR/t" << "File doesnt exist";
        return;
    }

    QFile chatLogsFile(chatFilePath);

    if (!chatLogsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR\t" << "Cannot open file";
        return;
    }

    chatLogsFile.seek(chatLogsFile.size());

    while (1) {
        while (!chatLogsFile.atEnd()) {
            QString line = chatLogsFile.readLine();
            processChatLine(line);
            QThread::msleep(50);
        }

        if (QThread::currentThread()->isInterruptionRequested()) {
            return;
        }

        // Ждать 1 секунду
        QThread::msleep(1000);
    }
}

///
/// \brief processChatLine - Обработка единичной строки чата.
/// \param line - Единичная строка чата.
///
void ChatProcessing::processChatLine(QString& line) {
    if (!line.contains("] [Client thread/INFO]: [CHAT]", Qt::CaseSensitive)) {
        //qDebug() << "NOT CONTAINS";
        return;
    }

    // Обрезаем лишнее
    line.chop(2);
    line = line.mid(40);
    if (line[0] == '<') {
        line = line.mid(line.indexOf(">") + 2);
    }
    if (line[0] == '+') {
        line = line.mid(2);
    }
    if ((line[0] == '[') && (!line.contains("=>", Qt::CaseSensitive)) && (!line.contains("<=", Qt::CaseSensitive))) {
        line = line.mid(line.indexOf("]") + 2);
    }

    // Игрок подключился к МАТЧУ
    if (line.startsWith("[") && (line.contains(" => Игрок ") || line.contains(" <= Игрок "))) {
        // Кол-во игроков в матче
        QString count = line.mid(1, line.indexOf("]") - 1);

        // Получаем никнейм присоединившегося
        QChar tempChar = '0';
        int end = line.contains("подключился") ? line.indexOf(" подключился") : line.indexOf(" вышел");
        int start = end;

        while(tempChar != ' ')
            tempChar = line[--start];
        start++;

        emit playerJoinedMatch(count, line.mid(start, end - start));

        return;
    }

    // Вы были перемещены в лобби (Завершить матч для пользователя)
    if (line == "Вы были перемещены в лобби") {
        emit endMatch();
    }

    // Пользователь нажал на никнейм в чате
    if (line.endsWith(" - ЛС В друзья Группа Профиль")) {
        QString temp = line.mid(0, line.size() - 29);
        qDebug() << "|PROCESSED| Player CLICKED:" << temp;
        return;
    }

    // Добавлено коинов
    if (line.startsWith("Добавлено коинов: ", Qt::CaseSensitive)) {
        int temp = line.mid(18).toInt();
        emit coinsAdded(temp);
        return;
    }

    // Вы нашли маленький подарок
    if (line.startsWith("VimeWorld > Поздравляем, вы нашли маленький подарочек", Qt::CaseSensitive)) {
        qDebug() << "|PROCESSED| Small Surprizebox";
        return;
    }

    // Вы получили новый уровень
    if (line.startsWith("VimeWorld > Поздравляем! Вы получили ", Qt::CaseSensitive)) {
        line = line.mid(37);
        line.chop(52);
        int temp = line.toInt();
        qDebug() << "|PROCESSED| New LVL: " << temp;
        return;
    }
}
