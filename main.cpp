#include "magicassistant.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return 1;
    }

    MagicAssistant w;
    w.show();

    return a.exec();
}
