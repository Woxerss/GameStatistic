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
            //QThread::msleep();
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
        return;
    }

    line.chop(2);
    line = line.mid(40);

    if (!line.isEmpty()) {
        if (line[0] == '<') {
            line = line.mid(line.indexOf(">") + 2);
        }
        if (line[0] == '+') {
            line = line.mid(2);
        }
        if ((line[0] == '[') && (!line.contains("=>", Qt::CaseSensitive)) && (!line.contains("<=", Qt::CaseSensitive))) {
            line = line.mid(line.indexOf("]") + 2);
        }
    } else {
        return;
    }

        /* Обычные события */
    // Игрок вошел в лобби
    if (line.endsWith(" вошел в лобби!")) {
        line.truncate(line.indexOf(" вош"));
        qDebug() << "Player joined lobby: " << line;
        return;
    }

    // Игрок подключился к МАТЧУ
    if (line.startsWith("[") && (line.contains(" => Игрок ") || line.contains(" <= Игрок ")) && !line.startsWith("[BP] ")) {
        // Кол-во игроков в матче
        QString count = line.mid(1, line.indexOf("]") - 1);

        line = line.mid(line.indexOf("Игрок ") + 6);
        if (line.contains(">")) {
            line = line.mid(line.indexOf(">") + 2);
        }

        // Получаем никнейм присоединившегося
        if (line.contains("подключился")) {
            line.truncate(line.indexOf(" подключился"));
        } else if (line.contains("вышел")) {
            line.truncate(line.indexOf(" вышел"));
        }

        qDebug() << "Player joined match: " << count << ':' << line;

        return;
    }

    // Вы были перемещены в лобби (Завершить матч для пользователя)
    if (line == "Вы были перемещены в лобби") {
        qDebug() << "Returned to lobby";
        return;
    }

    // Вы нашли маленький подарок
    if (line.startsWith("VimeWorld > Поздравляем, вы нашли", Qt::CaseSensitive)) {
        qDebug() << "Small Surprizebox";
        return;
    }

    // Вы получили новый уровень
    if (line.startsWith("VimeWorld > Поздравляем! Вы получили ", Qt::CaseSensitive)) {
        line = line.mid(37);
        line.chop(52);
        int temp = line.toInt();
        qDebug() << "New LVL: " << temp;
        return;
    }

    // Добавлено коинов
    if (line.startsWith("Добавлено коинов: ", Qt::CaseSensitive)) {
        int temp = line.midRef(18).toInt();
        qDebug() << "Coins added:" << temp;
        return;
    }

    // Игрок написал сообщение в чат
    if (line.contains(":") && !line.contains(" > Игрок ") && !line.startsWith("§2Друзья §e>") && !line.startsWith("/")) {
        if (line.startsWith("(Наблюдатели) ")) {
            line = line.mid(14);
        }

        line.truncate(line.indexOf(":"));
        qDebug() << "Player send message: " << line;
        return;
    }

    // Игра не началась из-за недостатка игроков
    if (line.startsWith("Недостаточно игроков для начала игры")) {
        qDebug() << "MATCH HAS NOT STARTED";
        return;
    }

    // Конец МАТЧА
    if (line.startsWith("           ")) {
        qDebug() << "END GAME";
        return;
    }

        /* Murder Mistery */

    // Маньяк получил оружие
    if (line == "Маньяк получил оружие!") {
        qDebug() << "Maniac has sword";
        return;
    }

    // Игрок умер играя в рулетку
    if (line.endsWith("проиграл в русской рулетке")) {
        line.truncate(line.indexOf(" проиграл"));

        qDebug() << "Death in roulette: " << line;
        return;
    }

    // Детектив убил мирного игрока
    // TODO: Проверить корректность
    if (line.contains(" убил мирного игрока ")) {
        QString firstPlayer;
        QString secondPlayer = line;

        // Получаем никнейм детектива
        line.truncate(line.indexOf(" убил"));
        firstPlayer = line;

        // Получаем ник убийцы
        secondPlayer = secondPlayer.mid(secondPlayer.indexOf("игрока ") + 7);

        qDebug() << "Death: " << firstPlayer << "Killer: " << secondPlayer;
        return;
    }

    // Маньяк убил игрока
    if (line.startsWith("Маньяк убил игрока")) {
        QString temp = line.mid(19);
        qDebug() << "Maniac killed a player: " << temp;
        return;
    }

    // Маньяк был убит игроком
    if (line.startsWith("Маньяк ") && line.contains(" был убит игроком ")) {
        line = line.mid(7);
        QString firstPlayer;
        QString secondPlayer = line;

        // Получаем никнейм детектива
        line.truncate(line.indexOf(" был"));
        firstPlayer = line;

        // Получаем ник убийцы
        secondPlayer = secondPlayer.mid(secondPlayer.lastIndexOf("игроком ") + 8);

        qDebug() << "Maniac: " << firstPlayer << "Detective: " << secondPlayer;
        return;
    }

    // Игрок вышел (выбыл из матча)
    if (line.endsWith(" покинул игру")) {
        line.truncate(line.indexOf(" поки"));

        qDebug() << "Player left: " << line;
        return;
    }

    // Игрок сгорел
    if (line.endsWith(" сгорел в лавовом резервуаре")) {
        line.truncate(line.indexOf(" сгор"));

        qDebug() << "Death: " << line;
        return;
    }

    // Игрок умер от интокцсикации
    if (line.endsWith(" умер от алкогольной интоксикации")) {
        line.truncate(line.indexOf(" умер"));

        qDebug() << "Death: " << line;
        return;
    }

    // Игрок задохнулся в водяном топливе
    if (line.endsWith(" задохнулся в водяном топливе")) {
        line.truncate(line.indexOf(" задох"));

        qDebug() << "Death: " << line;
        return;
    }

    // Игрок упал в яму
    if (line.endsWith(" упал в яму")) {
        line.truncate(line.indexOf(" упал"));
        qDebug() << "Death: " << line;
        return;
    }

    // Игрок попал в ловушку
    if (line.endsWith(" попал в ловушку")) {
        line.truncate(line.indexOf(" попал"));
        qDebug() << "Death: " << line;
        return;
    }

    // Игрок получил смертельный ожог от пара
    if (line.endsWith(" получил смертельный ожог от пара")) {
        line.truncate(line.indexOf(" получил"));
        qDebug() << "Death: " << line;
        return;
    }

    // Лук выпал
    if (line.startsWith("Лук выпал")) {
        qDebug() << "Bow dropped";
        return;
    }

        /* Сообщения о смерти */

    // Игрок был убит другим игроком
    if (line.startsWith("Игрок ") && line.contains(" был убит игроком ")) {
        line = line.mid(6);
        QString firstPlayer;
        QString secondPlayer = line;

        if (line.startsWith("<")) {
            line = secondPlayer.mid(secondPlayer.indexOf(">") + 2);
        }

        // Получаем никнейм жертвы
        line.truncate(line.indexOf(" был"));
        firstPlayer = line;

        // Получаем ник убийцы
        secondPlayer = secondPlayer.mid(secondPlayer.lastIndexOf("игроком ") + 8);
        if (secondPlayer.contains(">")) {
            secondPlayer = secondPlayer.mid(secondPlayer.indexOf(">") + 2);
        }

        secondPlayer.truncate(secondPlayer.indexOf('!'));

        qDebug() << "Death: " << firstPlayer << "Killer: " << secondPlayer;
        return;
    }

    // Игрок умер (и ему в этом никто не помог)
    if (line.startsWith("Игрок ") && line.contains(" умер!")) {
        line = line.mid(6);

        if (line.startsWith("<")) {
            line = line.mid(line.indexOf(">") + 2);
        }

        line.truncate(line.indexOf(" умер"));

        qDebug() << "DEATH: " << line;

        return;
    }

        /* Speed Builders события в чате: */
    // Игрок закончил постройку
    if (line.startsWith("Игрок ") && line.contains(" место выполнив постройку за ")) {
        QString time;
        QString top;
        QString player;

        // Игрок, выполнивший постройку
        line = line.mid(6);
        if (line.startsWith("<")) {
            line = line.mid(line.indexOf(">") + 2);
        }
        player = line;
        player.truncate(line.indexOf(" занял"));

        // Позиция в топе
        top = line.mid(line.indexOf("занял ") + 6);
        top.truncate(top.indexOf(" место"));

        // Время выполнения постройки
        time = line.mid(line.indexOf("за ") + 3);
        time.truncate(time.indexOf(" секунд"));

        qDebug() << top << " : " << player << " : " << time;
        return;
    }

    // Игрок уничтожен (выбыл)
    if (line.startsWith("Игрок ") && line.contains(" уничтожен! ")) {
        line = line.mid(6);
        if (line.startsWith("<")) {
            line = line.mid(line.indexOf(">") + 2);
        }
        line.truncate(line.indexOf(" уничтожен"));

        qDebug() << "END BUILD" << "Left: " << line;

        return;
    }

    if (line == "Игроки получили одинаковый балл. Игра продолжается.") {
        qDebug() << "END BUILD" << "NOBODY LOOSE";
        return;
    }

        /* Пользовательские события в чате: */

    // Пользователь нажал на никнейм в чате
    if (line.endsWith(" - ЛС В друзья Группа Профиль") || line.endsWith(" - ЛС В друзья Группа В гильдию Профиль")) {
        line.truncate(line.indexOf(" - "));
        qDebug() << "CLICKED ON PLAYER:" << line;
        return;
    }

    // Пользователь вывел список игроков через tab в чате
    // TODO: Заменить на регулярку
    bool flag = false;
    QStringList wordList;
    wordList = line.split(", ");
    foreach(QString word, wordList) {
        if (word.contains(QRegExp("[а-яА-Я\[<.!:; /?#]"))) {
            flag = true;
        }
    }
    if  (!flag) {
         qDebug() << "TAB";
        return;
    }
        /* Неизвестное сообщение: */

    qDebug() << "[Undefined] " << line;
}


