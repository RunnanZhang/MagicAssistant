#include "MagicAssistant.h"
#include "LogHandler.h"
#include "defines.h"

#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <Settings.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(a.applicationDirPath());

//    QDir dir(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//    QLocale locale = QLocale(QLocale::Chinese, QLocale::China);

//    foreach(QString fileName, dir.entryList(QDir::Files))
//    {
//        if(fileName.contains(locale.name()))
//        {
//            QTranslator *translator = new QTranslator(qApp);
//            translator->load(fileName, dir.absolutePath());
//            qApp->installTranslator(translator);
//        }
//    }

    QTranslator *translator = new QTranslator(qApp);
    Settings set(SETTING_PATH);
    QString lan = set.value("Language", "en_US").toString();
    translator->load(QString(":/translate/MagicAssistant_%1.qm").arg(lan));
    qApp->installTranslator(translator);

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
