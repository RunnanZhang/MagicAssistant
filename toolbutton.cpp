#include "toolbutton.h"
#include <QPainter>

ToolButton::ToolButton(QWidget *parent) :
    QToolButton(parent),
    _is_draw_small(true)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

ToolButton::~ToolButton()
{
}

void ToolButton::setPixmap(const QString &pixSmall, const QString &pixBig)
{
    _pix_small = pixSmall;
    _pix_big = pixBig;

    // 初始化为小图.
    _pixmap_small.load(pixSmall);
    _pixmap_big.load(pixBig);
}

void ToolButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //绘制不规则背景.
    if(_is_draw_small) {
        painter.drawPixmap(0, 0, _pixmap_small.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        painter.drawPixmap(0, 0, _pixmap_big.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

void ToolButton::enterEvent(QEvent *event)
{
    //此处必须都设置，否则效果会出错，不可只设置Min.而Max还小于Min.
    this->setMinimumSize(64, 64);
    this->setMaximumSize(64, 64);

    //图标随之更换为大图片.
    _is_draw_small = false;

    QWidget::enterEvent(event);
}

void ToolButton::leaveEvent(QEvent *event)
{
    this->setMinimumSize(32, 32);
    this->setMaximumSize(32, 32);

    //图标随之更换回为小图片.
    _is_draw_small = true;

    QWidget::leaveEvent(event);
}
