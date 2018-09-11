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

    void setShowWidget(QWidget *w);

    void clear();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void on_okBtn_clicked();

private:
    Ui::InfoBoard *ui;

    QWidget *_w;
};

#endif // INFOBOARD_H
