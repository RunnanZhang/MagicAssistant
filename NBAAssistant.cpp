#include "NBAAssistant.h"

NBAAssistant::NBAAssistant(QObject *parent) :
    QObject(parent)
{
qRegisterMetaType<QList<TeamScore*>>("TeamScore");
}

NBAAssistant::~NBAAssistant()
{
    qDeleteAll(_TeamScore);
}

QList<TeamScore*> NBAAssistant::getTodayScore()
{
    QEventLoop loop;

    QNetworkAccessManager manager;
	QNetworkRequest request;
    request.setUrl(QUrl("http://nba.hupu.com/games"));
	// 由于虎扑已经升级为https，所以可以直接将URL改为https，也可以加如下属性，可重定向为https.
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = manager.get(request);

    ///< 用事件循环就是为了在一个函数中完成处理操作，当然可以不用事件循环阻塞，将处理解析reply的代码，放到连接finished信号的槽函数中即可.
    //请求结束并下载完成后，退出子事件循环.
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环.
    loop.exec();

    // 当finished信号发出，事件循环结束，此时网页源码已下载完毕，可以开始解析.
    QString source = QString(reply->readAll());
    analyzeCode(source);

    return _TeamScore;
}

void NBAAssistant::analyzeCode(QString source)
{
    int nCPos = source.indexOf("<div class=\"gamecenter_content\">");
    while(nCPos != -1)
    {
        TeamScore *data = new TeamScore;
        source = source.mid(nCPos);
        nCPos = source.indexOf("<div class=\"txt\">");
        source = source.mid(nCPos);
        nCPos = source.indexOf("<span");
        source = source.mid(nCPos);

        // 若比赛未开始，下面没有class="num一栏.
        nCPos = source.indexOf("<span class=\"num");
        int nTemp = source.indexOf("<div class=\"txt\">");
        bool isStarted = nCPos < nTemp && nCPos >= 0;

        if(isStarted)
        {
            // away team score
            source = source.mid(nCPos);
            nCPos = source.indexOf(">") + 1;
            source = source.mid(nCPos);
            nCPos = source.indexOf("<");
            QString score = source.left(nCPos);
            data->_awayScore = score.toUInt();
        }
        else
        {
            data->_awayScore = 0;
        }


        // away team name
        source = source.mid(nCPos);
        nCPos = source.indexOf("\">") + 2;
        source = source.mid(nCPos);
        nCPos = source.indexOf("<");
        data->_awayTeam = source.left(nCPos);

        nCPos = source.indexOf("<div class=\"txt\">");
        source = source.mid(nCPos);
        nCPos = source.indexOf("<span");

        if(isStarted)
        {
            // home team score
            source = source.mid(nCPos);
            nCPos = source.indexOf(">") + 1;
            source = source.mid(nCPos);
            nCPos = source.indexOf("<");
            QString score = source.left(nCPos);
            data->_homeScore = score.toUInt();
        }
        else
        {
            data->_homeScore = 0;
        }

        // away team name
        source = source.mid(nCPos);
        nCPos = source.indexOf("\">") + 2;
        source = source.mid(nCPos);
        nCPos = source.indexOf("<");
        data->_homeTeam = source.left(nCPos);

		// game state
		nCPos = source.indexOf("<span class=\"b\">");
		source = source.mid(nCPos);
		nCPos = source.indexOf("\">") + 3;
		source = source.mid(nCPos);
		nCPos = source.indexOf("</span");
		QString state = source.left(nCPos);
		state.replace("<p>", " ");
		state.replace("</p>", "");
		state.remove(QChar::LineFeed);
		data->_state = state;
        data->_isFocusedTeam = false;
        _TeamScore << data;

        //查找下一组.
        nCPos = source.indexOf("<div class=\"txt\">");
    }
}









