/*
 * NBAAssistant 通过访问篮球网站，获取NBA相关数据的类，可获取比分等信息.
*/
#ifndef NBAASSISTANT_H
#define NBAASSISTANT_H

#include <QtNetwork>

class NBAAssistant
{
public:
    NBAAssistant();
    ~NBAAssistant();

    void getTodayScore();

private:
    void analyzeCode(QString source);
};

#endif // NBAASSISTANT_H
