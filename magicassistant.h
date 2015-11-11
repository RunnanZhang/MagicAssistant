#ifndef MAGICASSISTANT_H
#define MAGICASSISTANT_H

#include <QWidget>

namespace Ui {
class MagicAssistant;
}

class ToolBar;

class MagicAssistant : public QWidget
{
    Q_OBJECT

public:
    explicit MagicAssistant(QWidget *parent = 0);
    ~MagicAssistant();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::MagicAssistant *ui;
    QPixmap _pixmap;
    QPoint _start_pos;
    ToolBar *_tool_bar;
};

#endif // MAGICASSISTANT_H
