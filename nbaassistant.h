/*
 * NBAAssistant 通过访问篮球网站，获取NBA相关数据的类，可获取比分等信息.
*/
#ifndef NBAASSISTANT_H
#define NBAASSISTANT_H

#include <QtNetwork>

///< 存储主队客队名字及其比分.
class TeamScore
{
public:
    QString homeTeam;
    QString awayTeam;
    quint16 homeScore;
    quint16 awayScore;
};

class NBAAssistant
{
public:
    NBAAssistant();
    ~NBAAssistant();

    void getTodayScore(QList<TeamScore> &list);

private:
    void analyzeCode(QString source, QList<TeamScore> &list);
};

#endif // NBAASSISTANT_H
