#include "InfoBoard.h"
#include "ui_InfoBoard.h"

#include <QDesktopServices>
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
    connect(ui->closeBtn2, &QToolButton::clicked, this, &QWidget::close);

    ui->textEdit->installEventFilter(this);

    _html = ui->textEdit->toHtml();
}

InfoBoard::~InfoBoard()
{
    delete ui;
}

void InfoBoard::append(const QString &text)
{
    ui->textEdit->setTextColor(Qt::black);

    for(auto str : _filterList)
    {
        if(text.contains(str))
        {
            ui->textEdit->setTextColor(_color);
            break;
        }
    }

    ui->textEdit->append(text);
}

void InfoBoard::clear()
{
    ui->textEdit->clear();
    ui->textEdit->setHtml(_html);
    _filterList.clear();
}

bool InfoBoard::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Enter)
    {
        ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else if(event->type() == QEvent::Leave)
    {
        ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    return QWidget::eventFilter(obj, event);
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

void InfoBoard::setFilterText(const QStringList &filter, const QColor &color)
{
    _filterList = filter;
    _color = color;
}
