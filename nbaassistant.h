/*
 * NBAAssistant 通过访问篮球网站，获取NBA相关数据的类，可获取比分等信息.
*/
#ifndef NBAASSISTANT_H
#define NBAASSISTANT_H

#include <QtNetwork>

#include <QObject>

///< 存储主队客队名字及其比分.
class TeamScore
{
public:
    QString homeTeam;
    QString awayTeam;
    quint16 homeScore;
    quint16 awayScore;
};

//Q_DECLARE_METATYPE(TeamScore)

class NBAAssistant : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<TeamScore> teamscore READ teamscore WRITE setTeamScore NOTIFY teamscoreChanged)

public:
    NBAAssistant(QObject *parent);
    ~NBAAssistant();

    void getTodayScore(QList<TeamScore> &list);

    QList<TeamScore> teamscore() const;
    void setTeamScore(const QList<TeamScore>& teamscore);

signals:
    void teamscoreChanged(const QList<TeamScore>&);

private:
    void analyzeCode(QString source, QList<TeamScore> &list);

private:
    QList<TeamScore> _TeamScore;
};

#endif // NBAASSISTANT_H
