#include "magicassistant.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MagicAssistant w;
    w.show();

    return a.exec();
}
