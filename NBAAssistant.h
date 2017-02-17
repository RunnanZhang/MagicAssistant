/*
 * NBAAssistant 通过访问篮球网站，获取NBA相关数据的类，可获取比分等信息.
*/
#ifndef NBAASSISTANT_H
#define NBAASSISTANT_H

#include <QtNetwork>

#include <QObject>

///< 存储主队客队名字及其比分.
class TeamScore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString homeTeam READ getHomeTeam)
    Q_PROPERTY(QString awayTeam READ getAwayTeam)
    Q_PROPERTY(quint16 homeScore READ getHomeScore)
    Q_PROPERTY(quint16 awayScore READ getAwayScore)

public:
    TeamScore(QObject *parent = 0) : QObject(parent) {}
    QString getHomeTeam() const {return _homeTeam;}
    QString getAwayTeam() const {return _awayTeam;}
    quint16 getHomeScore() const {return _homeScore;}
    quint16 getAwayScore() const {return _awayScore;}

public:
    QString _homeTeam;
    QString _awayTeam;
    quint16 _homeScore;
    quint16 _awayScore;
};

class NBAAssistant : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<TeamScore*> teamscore READ getTeamscore WRITE setTeamScore NOTIFY teamscoreChanged)


public:
    NBAAssistant(QObject *parent = 0);
    ~NBAAssistant();

    void getTodayScore();

    QList<TeamScore*> getTeamscore() const;
    void setTeamScore(const QList<TeamScore*>& teamscore);

signals:
    void teamscoreChanged(const QList<TeamScore*>&);

private:
    void analyzeCode(QString source);

private:
    QList<TeamScore*> _TeamScore;
};

#endif // NBAASSISTANT_H
