#include "LogHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QApplication>

QMutex LogHandler::s_logMutex;
QFile* LogHandler::s_file = Q_NULLPTR;
QTextStream* LogHandler::s_stream = Q_NULLPTR;

LogHandler::LogHandler()
{
    QString path = qApp->applicationDirPath() + QLatin1String("/log");
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkpath(path);
    }

    // 以日期命名，若不存在，则创建.
    QDate currentDate = QDate::currentDate();
    QString date = currentDate.toString("yyyy-MM-dd");
    path += QDir::separator() + date + ".log";

    s_file = new QFile(path);

    qInfo() << "Log Handler path : " << path;

    if(s_file->exists())
    {
        s_stream  = (s_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ?  new QTextStream(s_file) : Q_NULLPTR;
    }
    else
    {
        s_stream  = (s_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) ?  new QTextStream(s_file) : Q_NULLPTR;
    }

    if (Q_NULLPTR != s_stream)
    {
        s_stream->setCodec("UTF-8");
    }

    *s_stream << "Begin application at:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << endl;

    // 定时刷新日志输出到文件，保证日志的实时更新.
    m_flushTimer = new QTimer;
    m_flushTimer->start(1000);

    QObject::connect(m_flushTimer, &QTimer::timeout, [this] {
        //qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); // 测试不断写入.
        QMutexLocker locker(&s_logMutex);
        if (Q_NULLPTR != s_stream) {
            s_stream->flush();
        }
    });
}

LogHandler::~LogHandler()
{
    QMutexLocker locker(&s_logMutex);
    *s_stream << "End application at:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << endl << endl <<endl;
    qInstallMessageHandler(0);

    if(m_flushTimer)
    {
        delete m_flushTimer;
        m_flushTimer = Q_NULLPTR;
    }

    if(s_file)
    {
        s_stream->flush();
        s_file->close();
        delete s_file;
        s_file = Q_NULLPTR;
    }

    if(s_stream)
    {
        delete s_stream;
        s_stream = Q_NULLPTR;
    }
}

void LogHandler::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&s_logMutex);
    QString level;
    switch (type)
    {
    case QtDebugMsg:
        level = "Debug";
        break;
    case QtInfoMsg:
        level = "Info";
        break;
    case QtWarningMsg:
        level = "Warn";
        break;
    case QtCriticalMsg:
        level = "Error";
        break;
    case QtFatalMsg:
        level = "Fatal";
        break;
    default:;
    }
    

	// 这里在VS中无法输出，原因未知，抄袭qlogging.cpp 中qDefaultMessageHandler函数中代码，临时解决此问题.
	// 输出到标准输出.
	//QByteArray localMsg = msg.toLocal8Bit();
	//fprintf(stderr, "%s\n", localMsg.constData());
	//fflush(stderr);

#if defined(Q_OS_WIN)
	QString stdOut = msg;
	stdOut.append(QLatin1Char('\n'));
	OutputDebugString(reinterpret_cast<const wchar_t *>(stdOut.utf16()));
#endif

    if (Q_NULLPTR == s_stream)
    {
        return;
    }

    // 输出到日志文件, 格式: 时间 - [Level] (文件名:行数, 函数): 消息.
    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);
    *s_stream << QString("%1 - [%2] (%3:%4, %5): %6\n")
                                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(level)
                                    .arg(fileName).arg(context.line).arg(context.function).arg(msg);
}

void LogHandler::installMessageHandler()
{
    QMutexLocker locker(&s_logMutex);
    qInstallMessageHandler(&messageHandler);
}

