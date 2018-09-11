#include "InfoBoard.h"
#include "ui_InfoBoard.h"

#include <QDesktopServices>
#include <QPoint>
#include <QCursor>
#include <QUrl>
#include <windows.h>

InfoBoard::InfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoBoard),
    _w(Q_NULLPTR)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    connect(ui->closeBtn, &QToolButton::clicked, this, &QWidget::close);
    connect(ui->closeBtn2, &QToolButton::clicked, this, &QWidget::close);
}

InfoBoard::~InfoBoard()
{
    delete ui;
}

void InfoBoard::setShowWidget(QWidget *w)
{
    if(w == _w) return;

    if(_w != Q_NULLPTR)
    {
        delete _w;
    }

    _w = w;
    ui->widgetLayout->addWidget(w);
}

void InfoBoard::clear()
{
//    ui->textEdit->clear();
//    ui->textEdit->setHtml(_html);
//    _filterList.clear();
}

bool InfoBoard::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);

    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);

        if(msg->message == WM_NCHITTEST)
        {
            QPoint pos = this->mapFromGlobal(QCursor::pos());
            if (this->childAt(pos) == ui->titleBar)
            {
                *result = HTCAPTION;
                return true;
            }
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void InfoBoard::on_okBtn_clicked()
{
    QString link = "http://nba.hupu.com";
    QDesktopServices::openUrl(QUrl(link));
}
