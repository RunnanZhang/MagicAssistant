#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "Singleton.h"

class QFile;
class QTextStream;
class QTimer;
class LogHandler
{
    SINGLETON(LogHandler)

public:
    void installMessageHandler();

private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QMutex s_logMutex;
    static QFile *s_file;
    static QTextStream *s_stream;

    QTimer *m_flushTimer;
};

#endif // LOGHANDLER_H
