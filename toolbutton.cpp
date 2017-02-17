#include "ToolButton.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

ToolButton::ToolButton(QWidget *parent) :
    QToolButton(parent),
    _is_draw_small(false)
{
    setAttribute(Qt::WA_TranslucentBackground);

    // 此处必须都设置，不可只设置Min.而Max还小于Min，这样会导致显示出差错.
    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumSize");
    animationMin->setDuration(200);
    animationMin->setStartValue(QSize(32, 32));
    animationMin->setEndValue(QSize(64, 64));

    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumSize");
    animationMax->setDuration(200);
    animationMax->setStartValue(QSize(32, 32));
    animationMax->setEndValue(QSize(64, 64));

    _group = new QParallelAnimationGroup(this);
    _group->addAnimation(animationMin);
    _group->addAnimation(animationMax);
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
    //绘制不规则背景.  目前先都用64*64的图，否则就得等leave动画完成后换成32*32的图.用64的缩小的32不失真，但是32的不可放大到64.
    if(_is_draw_small) {
        painter.drawPixmap(0, 0, _pixmap_small.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        painter.drawPixmap(0, 0, _pixmap_big.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

void ToolButton::enterEvent(QEvent *event)
{
    // 注意此处必须先调整Direction再启动动画，否则动画走到最后，还是向前的方向，则直接结束了.
    _group->setDirection(QAbstractAnimation::Forward);
    _group->start();

    //图标随之更换为大图片.
    //_is_draw_small = false;

    QWidget::enterEvent(event);
}

void ToolButton::leaveEvent(QEvent *event)
{
    _group->setDirection(QAbstractAnimation::Backward);
    _group->start();

    //图标随之更换回为小图片.
    //_is_draw_small = true;

    QWidget::leaveEvent(event);
}
