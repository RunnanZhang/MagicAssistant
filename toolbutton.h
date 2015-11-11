#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>

class ToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit ToolButton(QWidget *parent = 0);
    ~ToolButton();

    void setPixmap(const QString &path);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPixmap _pixmap;
};

#endif // TOOLBUTTON_H
