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

    void setFilterText(const QStringList &filter, const QColor &color);

    void clear();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void on_okBtn_clicked();

private:
    Ui::InfoBoard *ui;

    QStringList _filterList;

    QColor _color;

    QString _html;
};

#endif // INFOBOARD_H
