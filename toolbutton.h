#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>

class ToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit ToolButton(QWidget *parent = 0);
    ~ToolButton();

    void setPixmap(const QString &pixSmall, const QString &pixBig);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    ///< \note 用于存放所绘制的图片.
    QPixmap _pixmap_small;
    QPixmap _pixmap_big;

    ///< \note 绘制图片路径.
    QString _pix_small;
    QString _pix_big;

    ///< \note 是否绘制小图标.
    bool _is_draw_small;
};

#endif // TOOLBUTTON_H
