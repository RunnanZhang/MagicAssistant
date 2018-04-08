#include "MagicAssistant.h"
#include "LogHandler.h"

#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(a.applicationDirPath());

    QDir dir(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QLocale locale = QLocale(QLocale::Chinese, QLocale::China);

    foreach(QString fileName, dir.entryList(QDir::Files))
    {
        if(fileName.contains(locale.name()))
        {
            QTranslator *translator = new QTranslator(qApp);
            translator->load(fileName, dir.absolutePath());
            qApp->installTranslator(translator);
        }
    }

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
