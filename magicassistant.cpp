#include "magicassistant.h"
#include "ui_magicassistant.h"
#include "toolbar.h"
#include "defines.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QMenu>
#include <QSettings>
#include <QDir>
#include <QActionGroup>
#include <QDesktopWidget>

MagicAssistant::MagicAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicAssistant)
{
    ui->setupUi(this);

    _pixmap.load(":/images/james.png");

    _tool_bar = new ToolBar(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    initializeGeometry();

    initializeTray();
}

MagicAssistant::~MagicAssistant()
{
    delete ui;

    //将位置信息写入配置文件.
    QSettings regedit(SETTING_PATH, QSettings::IniFormat);
    regedit.setValue(GEOMETRY_KEY, this->geometry());
}

void MagicAssistant::initializeGeometry()
{
    QRect desktop = QApplication::desktop()->geometry();
    QSettings regedit(SETTING_PATH, QSettings::IniFormat);

    if(regedit.value(GEOMETRY_KEY) == QVariant()) {
        _geometry = QRect(desktop.width()-200, desktop.height()-250, 120, 150);
        regedit.setValue(GEOMETRY_KEY, _geometry);
    } else {
        _geometry = regedit.value(GEOMETRY_KEY).toRect();
    }

    this->setGeometry(_geometry);
}

void MagicAssistant::initializeTray()
{
    _system_tray = new QSystemTrayIcon(this);
    QObject::connect(_system_tray, &QSystemTrayIcon::activated, this, &MagicAssistant::trayActivated);

    QMenu *trayMenu = new QMenu(this);
    QAction* autoStartAction = trayMenu->addAction(tr("Auto Start"), this, SLOT(autoStart(bool)));
    autoStartAction->setCheckable(true);
    trayMenu->addSeparator();

    ///<Begin 设置透明度菜单.此处目前没有好好的设计，采用hard coding.
    QSettings set(SETTING_PATH, QSettings::IniFormat);
    QActionGroup *group = new QActionGroup(this);
    qreal opacity = set.value(OPACITY_KEY, 1.0).toReal();

    QMenu *opacityMenu = trayMenu->addMenu("Opacity");
    QAction *action = opacityMenu->addAction("100%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 1) {
        action->setChecked(true);
        setOpacity(1);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(1);});

    action = opacityMenu->addAction("90%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 0.9) {
        action->setChecked(true);
        setOpacity(0.9);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(0.9);});

    action = opacityMenu->addAction("80%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 0.8) {
        action->setChecked(true);
        setOpacity(0.8);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(0.8);});

    action = opacityMenu->addAction("70%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 0.7) {
        action->setChecked(true);
        setOpacity(0.7);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(0.7);});

    action = opacityMenu->addAction("60%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 0.6) {
        action->setChecked(true);
        setOpacity(0.6);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(0.6);});

    action = opacityMenu->addAction("50%");
    group->addAction(action);
    action->setCheckable(true);
    if(opacity == 0.5) {
        action->setChecked(true);
        setOpacity(0.5);
    }
    connect(action, &QAction::triggered, this, [=] () {setOpacity(0.5);});
    ///<End

    trayMenu->addAction(tr("&Show"), this, SLOT(showNormal()));
    trayMenu->addSeparator();

    trayMenu->addAction(tr("&Quit"), qApp, SLOT(quit()));

    _system_tray->setContextMenu(trayMenu);
    QIcon icon(":/images/james_16.ico");
    _system_tray->setIcon(icon);
    _system_tray->setToolTip("Magic Assistant");
    _system_tray->show();
    if (_system_tray->isVisible()) {
        _system_tray->showMessage(tr("Information"), tr("Magic Assistant stay in system tray"));
    }

    // 根据注册表键值初始化菜单checked状态.
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);
    if(regedit.value(REGEDIT_KEY) == QVariant()) {
        autoStartAction->setChecked(false);
    } else {
        autoStartAction->setChecked(true);
    }
}

void MagicAssistant::autoStart(bool is_start)
{
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);

    if(is_start) {
        QString sAppPath = QApplication::applicationFilePath();
        regedit.setValue(REGEDIT_KEY, QVariant(QDir::toNativeSeparators(sAppPath)));
    } else {
        regedit.setValue(REGEDIT_KEY, QVariant());
    }

}

void MagicAssistant::paintEvent(QPaintEvent*)
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

void MagicAssistant::mouseReleaseEvent(QMouseEvent*)
{
    QRect rect = this->geometry();
    _tool_bar->show();
    _tool_bar->setDisplayMark(true);
    _tool_bar->move(rect.x() - 30, rect.y() + rect.height());
}


void MagicAssistant::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - _start_pos);

    _geometry = this->geometry();
}

void MagicAssistant::mouseDoubleClickEvent(QMouseEvent*)
{ 
    //此时立刻隐藏这两个窗体，所以无需applyHide.
    _tool_bar->hide();
    this->hide();
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

void MagicAssistant::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) {
        showNormal();
    }
}

void MagicAssistant::setOpacity(qreal opacity)
{
    this->setWindowOpacity(opacity);
    _tool_bar->setWindowOpacity(opacity);
    QSettings set(SETTING_PATH, QSettings::IniFormat);
    set.setValue(OPACITY_KEY, opacity);
}

