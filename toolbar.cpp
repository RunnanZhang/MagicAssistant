#include "toolbar.h"
#include "ui_toolbar.h"
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QInputDialog>
#include <QDateTime>
#include <QDesktopWidget>
#include <QScreen>
#include <QDir>

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar),
    _is_display(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化ToolButton的Pixmap.
    ui->toolButton->setPixmap(":/images/update.png");
    ui->toolButton2->setPixmap(":/images/build.png");
    ui->pOpenBtn->setPixmap(":/images/search.png");
    ui->pCommandBtn->setPixmap(":/images/command.png");
    ui->pScreenshotBtn->setPixmap(":/images/screenshot.png");
    ui->pShutdownBtn->setPixmap(":/images/shutdown.png");
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

void ToolBar::on_toolButton_clicked()
{
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/update.bat";
    process.startDetached(str);
}

void ToolBar::on_toolButton2_clicked()
{
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/vsbuild.bat";
    process.startDetached(str);
}

void ToolBar::on_pOpenBtn_clicked()
{
    // start是cmd内部命令，必须这么调用，/c是执行完关闭cmd的参数.具体可查阅cmd的帮助(cmd /?).
    //QProcess::startDetached("cmd /c start C:/Workspace/OrigineBeta/Builds/Win/Origine/bin");

    //为了可编辑，将此代码也移动到外部的bat中.
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/open.bat";
    process.startDetached(str);
}

void ToolBar::on_pCommandBtn_clicked()
{
     QProcess::startDetached("cmd");
}

void ToolBar::on_pScreenshotBtn_clicked()
{
    // QScreen没有构造函数，必须通过次获取.
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap fullscreen = screen->grabWindow(QApplication::desktop()->winId());

    //以当前时间命名.
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString name = QString("%1%2%3").arg(date.year()).arg(date.month()).arg(date.day());
    name += QString("%1%2%3").arg(time.hour()).arg(time.minute()).arg(time.second());

    //没有文件夹则新建.
    QDir dir("./resources/screenshot");
    if(!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    QString file_path = dir.absolutePath() + "/" + name + ".png";

    // 可以调第三个形参，为图片质量，100是无压缩，默认会压缩，但是效果不错，不影响图片分辨率大小.
    fullscreen.save(file_path, "PNG");
}

void ToolBar::on_pShutdownBtn_clicked()
{
    bool ok;
    int time = QInputDialog::getInt(this, tr("Auto shutdown setting"), tr("Time(ms):"),
                                    0, 0, 2147483647, 1, &ok);

    if(ok) {
        //由于shutdown命令,-t后面的参数只要大于0，默认就会执行-f参数，-f是强制关闭程序，所以此句会导致程序退出...
        QProcess::startDetached(QString("shutdown -s -t %1").arg(time));
    } else {
        //QProcess::startDetached("shutdown -a");
    }
}
