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
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化ToolButton的Pixmap.
    ui->toolButton->setPixmap(":/images/update_32.png", ":/images/update_64.png");
    ui->toolButton->setToolTip("update");
    ui->toolButton2->setPixmap(":/images/build_32.png", ":/images/build_64.png");
    ui->toolButton2->setToolTip("build");
    ui->pOpenBtn->setPixmap(":/images/search_32.png", ":/images/search_64.png");
    ui->pOpenBtn->setToolTip("open");
    ui->pCommandBtn->setPixmap(":/images/command_32.png", ":/images/command_64.png");
    ui->pCommandBtn->setToolTip("command");
    ui->pScreenshotBtn->setPixmap(":/images/screenshot_32.png", ":/images/screenshot_64.png");
    ui->pScreenshotBtn->setToolTip("screenshot");
    ui->pShutdownBtn->setPixmap(":/images/shutdown_32.png", ":/images/shutdown_64.png");
    ui->pShutdownBtn->setToolTip("shutdown");
}

ToolBar::~ToolBar()
{
    delete ui;
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
    // 先隐藏所有窗体.
    emit hideRequested();

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
    if(_is_compressed) {
        fullscreen.save(file_path, "PNG");
    } else {
        fullscreen.save(file_path, "PNG", 100);
    }

    // 截图完毕再显示所有窗体.
    emit showRequested();
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
