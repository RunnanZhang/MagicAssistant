#include "ToolBar.h"
#include "ui_toolbar.h"

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
    emit updateMetaDataRequested();
}

void ToolBar::on_toolButton2_clicked()
{
    emit vsbuildRequested();
}

void ToolBar::on_pOpenBtn_clicked()
{
    emit openProjectDirRequested();
}

void ToolBar::on_pCommandBtn_clicked()
{
    emit openCommandRequested();
}

void ToolBar::on_pScreenshotBtn_clicked()
{
    emit screenShotRequested();
}

void ToolBar::on_pShutdownBtn_clicked()
{
    emit shutdownRequested();
}
