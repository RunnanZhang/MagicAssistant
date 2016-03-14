#include "nbaassistant.h"

NBAAssistant::NBAAssistant()
{

}

NBAAssistant::~NBAAssistant()
{

}

void NBAAssistant::getTodayScore()
{
    QUrl url = "http://nba.hupu.com/";
    QEventLoop loop;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    manager->get(QNetworkRequest(url));

    ///< 用事件循环就是为了在一个函数中完成处理操作，当然可以不用事件循环阻塞，将处理解析reply的代码，放到连接finished信号的槽函数中即可.
    //请求结束并下载完成后，退出子事件循环.
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环.
    loop.exec();

    // 当finished信号发出，事件循环结束，此时网页源码已下载完毕，可以开始解析.
    QString source = QString::fromLocal8Bit(reply->readAll());
    analyzeCode(source);
}

void NBAAssistant::analyzeCode(QString source)
{
    int nCPos = source.indexOf("<span class=\"nameText\">");
    while(nCPos != -1)
    {
        source = source.right(nCPos);
    }
}


