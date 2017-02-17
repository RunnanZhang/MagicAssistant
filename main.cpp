#include "MagicAssistant.h"
#include "LogHandler.h"

#include <QDir>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(a.applicationDirPath());

    // install log tool.
    LogHandler::getInstance()->installMessageHandler();

    qInfo() << "Current dir is: " << QDir::currentPath();

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return 1;
    }

    MagicAssistant w;
    w.show();

    return a.exec();
}
