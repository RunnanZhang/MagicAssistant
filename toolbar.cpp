#include "toolbar.h"
#include "ui_toolbar.h"
#include <QTimer>

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar),
    _is_display(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化ToolButton的Pixmap.
    ui->toolButton->setPixmap(":/1");
    ui->toolButton2->setPixmap(":/2");
    ui->toolButton3->setPixmap(":/3");
    ui->toolButton4->setPixmap(":/4");
}

ToolBar::~ToolBar()
{
    delete ui;
}

void ToolBar::enterEvent(QEvent *)
{
    _is_display = true;
}

void ToolBar::leaveEvent(QEvent *)
{
    _is_display = false;
    QTimer::singleShot(1500, Qt::CoarseTimer, this, SLOT(applyHide()));
}

void ToolBar::applyHide()
{
    if(!_is_display) {
        hide();
    }
}

void ToolBar::setDisplayMark(bool is_display)
{
    _is_display = is_display;
}
