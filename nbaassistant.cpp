#include "nbaassistant.h"

NBAAssistant::NBAAssistant()
{

}

NBAAssistant::~NBAAssistant()
{

}

void NBAAssistant::getTodayScore(QList<TeamScore> &list)
{
    QUrl url = "http://nba.hupu.com/";
    QEventLoop loop;

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    ///< 用事件循环就是为了在一个函数中完成处理操作，当然可以不用事件循环阻塞，将处理解析reply的代码，放到连接finished信号的槽函数中即可.
    //请求结束并下载完成后，退出子事件循环.
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环.
    loop.exec();

    // 当finished信号发出，事件循环结束，此时网页源码已下载完毕，可以开始解析.
    QString source = QString::fromLocal8Bit(reply->readAll());
    analyzeCode(source, list);
}

void NBAAssistant::analyzeCode(QString source, QList<TeamScore> &list)
{
    int nCPos = source.indexOf("<span class=\"nameText\">");
    while(nCPos != -1)
    {
        TeamScore data;
        source = source.mid(nCPos);
        nCPos = source.indexOf("title");
        source = source.mid(nCPos);
        data.homeTeam = source.section('\"', 1, 1);

        nCPos = source.indexOf("<span class=\"bifen\">");
		source = source.mid(nCPos);
		// 若比赛未开始，那么bifen下面没有title一栏.
		nCPos = source.indexOf("title");
		int nTemp = source.indexOf("<span class=\"nameText\">");

        if(nCPos < nTemp)
        {
            source = source.mid(nCPos);
            nCPos = source.indexOf(">") + 1;
            source = source.mid(nCPos);
            nCPos = source.indexOf("<");
            QString score = source.left(nCPos);
            QStringList scoreList = score.split(":");
            data.homeScore = scoreList[0].toUInt();
            data.awayScore = scoreList[1].toUInt();
        }
        else
        {
            data.homeScore = 0;
            data.awayScore = 0;
        }

        nCPos = source.indexOf("<span class=\"nameText\">");
        source = source.mid(nCPos);
        nCPos = source.indexOf("title");
        source = source.mid(nCPos);
        data.awayTeam = source.section('\"', 1, 1);

        list.append(data);

        //查找下一组.
        nCPos = source.indexOf("<span class=\"nameText\">");
    }
}









