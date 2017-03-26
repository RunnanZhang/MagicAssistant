#include "InfoBoard.h"
#include "ui_InfoBoard.h"

#include <QPoint>
#include <QCursor>
#include <windows.h>

InfoBoard::InfoBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoBoard)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    connect(ui->closeBtn, &QToolButton::clicked, this, &QWidget::close);
}

InfoBoard::~InfoBoard()
{
    delete ui;
}

void InfoBoard::append(const QString &text)
{
    ui->textEdit->append(text);
}

void InfoBoard::enterEvent(QEvent *e)
{
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget::enterEvent(e);
}

void InfoBoard::leaveEvent(QEvent *e)
{
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget::leaveEvent(e);
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

    return false;
}
