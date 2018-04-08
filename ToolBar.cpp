#include "ToolBar.h"
#include "ui_ToolBar.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    //初始化ToolButton的Pixmap.
    ui->pQueryScoreBtn->setPixmap(":/images/basketball_32.png", ":/images/basketball_64.png");
    ui->pQueryScoreBtn->setToolTip(tr("update"));
    ui->toolButton->setPixmap(":/images/update_32.png", ":/images/update_64.png");
    ui->toolButton->setToolTip(tr("update"));
    ui->pOpenBtn->setPixmap(":/images/search_32.png", ":/images/search_64.png");
    ui->pOpenBtn->setToolTip(tr("open"));
    ui->pCommandBtn->setPixmap(":/images/command_32.png", ":/images/command_64.png");
    ui->pCommandBtn->setToolTip(tr("command"));
    ui->pScreenshotBtn->setPixmap(":/images/screenshot_32.png", ":/images/screenshot_64.png");
    ui->pScreenshotBtn->setToolTip(tr("screenshot"));
    ui->pShutdownBtn->setPixmap(":/images/shutdown_32.png", ":/images/shutdown_64.png");
    ui->pShutdownBtn->setToolTip(tr("shutdown"));
}

ToolBar::~ToolBar()
{
    delete ui;
}

void ToolBar::on_toolButton_clicked()
{
    emit updateRequested();
}

void ToolBar::on_pQueryScoreBtn_clicked()
{
    emit queryScoreRequested();
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
