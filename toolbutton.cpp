#include "toolbutton.h"
#include <QPainter>

ToolButton::ToolButton(QWidget *parent) :
    QToolButton(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

ToolButton::~ToolButton()
{
}

void ToolButton::setPixmap(const QString &path)
{
    _pixmap.load(path);
}

void ToolButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //绘制不规则背景.
    painter.drawPixmap(0, 0, _pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
