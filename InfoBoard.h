#ifndef INFOBOARD_H
#define INFOBOARD_H

#include <QWidget>

namespace Ui {
class InfoBoard;
}

class InfoBoard : public QWidget
{
    Q_OBJECT

public:
    explicit InfoBoard(QWidget *parent = 0);
    ~InfoBoard();

    void append(const QString &text);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::InfoBoard *ui;
};

#endif // INFOBOARD_H
