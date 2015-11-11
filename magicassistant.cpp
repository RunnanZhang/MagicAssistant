#include "magicassistant.h"
#include "ui_magicassistant.h"
#include "toolbar.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>

MagicAssistant::MagicAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicAssistant)
{
    ui->setupUi(this);
    _pixmap.load(":/james.png");
    _tool_bar = new ToolBar(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(120, 150);
}

MagicAssistant::~MagicAssistant()
{
    delete ui;
}

void MagicAssistant::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
     // 绘制不规则背景.
    painter.drawPixmap(0, 0, _pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MagicAssistant::mousePressEvent(QMouseEvent *event)
{
    _start_pos = event->pos();
    _tool_bar->hide();
}

void MagicAssistant::mouseReleaseEvent(QMouseEvent *)
{
    QRect rect = this->geometry();
    _tool_bar->show();
    _tool_bar->setDisplayMark(true);
    _tool_bar->move(rect.x() - 30, rect.y() + rect.height());
}


void MagicAssistant::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - _start_pos);
}

void MagicAssistant::mouseDoubleClickEvent(QMouseEvent *)
{
    qApp->quit();
}

void MagicAssistant::enterEvent(QEvent *)
{
    QRect rect = this->geometry();
    _tool_bar->show();
    _tool_bar->setDisplayMark(true);
    _tool_bar->move(rect.x() - 30, rect.y() + rect.height());
}

void MagicAssistant::leaveEvent(QEvent *)
{
    _tool_bar->setDisplayMark(false);

    //过一段时间后，申请隐藏，假如鼠标没进入ToolBar则隐藏.
    QTimer::singleShot(1500, Qt::CoarseTimer, _tool_bar, SLOT(applyHide()));
}

