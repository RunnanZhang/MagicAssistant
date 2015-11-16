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
#include <QTimer>
#include <QDebug>
#include <QProcess>

MagicAssistant::MagicAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicAssistant)
{
    ui->setupUi(this);

    _pixmap.load(":/images/james.png");

    _toolbar = new ToolBar;
    connect(_toolbar, &ToolBar::hideRequested, this, [=] () {hideAll();});
    connect(_toolbar, &ToolBar::showRequested, this, [=] () {showAll();});

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacAlwaysShowToolWindow);
#endif

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateState()));

    initializeGeometry();

    initializeTray();

    initializeMenu();
}

MagicAssistant::~MagicAssistant()
{
    delete ui;

    delete _toolbar;

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

void MagicAssistant::showToolBar()
{
    QRect rect = this->geometry();
    _toolbar->show();

    //让toolbar和主窗体有共同的中心线.
    int x = rect.x() + 0.5*(rect.width() - _toolbar->width());
    int y = rect.y() + rect.height();
    _toolbar->move(x, y);
}

void MagicAssistant::hideAll()
{
    this->hide();
    _toolbar->hide();
}

void MagicAssistant::showAll()
{
    this->showNormal();
    showToolBar();
}

void MagicAssistant::mousePressEvent(QMouseEvent *event)
{
    _start_pos = event->pos();
    _toolbar->hide();

    QWidget::mousePressEvent(event);
}

void MagicAssistant::mouseReleaseEvent(QMouseEvent *event)
{
    showToolBar();
    _toolbar->show();
    QWidget::mouseReleaseEvent(event);
}


void MagicAssistant::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - _start_pos);

    _geometry = this->geometry();
}

void MagicAssistant::mouseDoubleClickEvent(QMouseEvent*)
{ 
    hideAll();
}

void MagicAssistant::enterEvent(QEvent *event)
{
    showToolBar();

    // 开启定时器，检测鼠标位置.
    _timer->start(1000);

    QWidget::enterEvent(event);
}

void MagicAssistant::updateState()
{
    //取主窗体和toolbar的边框范围.此运算符是算出两个矩形的边框.由于都没有父窗体，都是全局的坐标.
    QRect unite_rect = this->geometry() | _toolbar->geometry();

    if(!unite_rect.contains(QCursor::pos())) {
        _toolbar->hide();
        _timer->stop();
    }
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
    _toolbar->setWindowOpacity(opacity);
    QSettings set(SETTING_PATH, QSettings::IniFormat);
    set.setValue(OPACITY_KEY, opacity);
}

void MagicAssistant::openInExplorer()
{
    // start是cmd内部命令，必须这么调用，/c是执行完关闭cmd的参数.具体可查阅cmd的帮助(cmd /?).
    QString path = QApplication::applicationDirPath();
    QProcess::startDetached("cmd /c start " + path);
}

void MagicAssistant::screenshotSetting(bool is_compressed)
{
    _toolbar->setCompressedEnable(is_compressed);

    QSettings set(SETTING_PATH, QSettings::IniFormat);
    set.setValue(SCREENSHOT_KEY, is_compressed);
}

void MagicAssistant::initializeMenu()
{
    _menu = new QMenu(this);
    connect(this, &QWidget::customContextMenuRequested, this, [=] () {_menu->exec(QCursor::pos());});

    ///< 开机自启动开关.
    QAction* autoStartAction = _menu->addAction(tr("Auto Start"), this, SLOT(autoStart(bool)));
    autoStartAction->setCheckable(true);
    _menu->addSeparator();

    // 根据注册表键值初始化菜单checked状态.
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);
    if(regedit.value(REGEDIT_KEY) == QVariant()) {
        autoStartAction->setChecked(false);
    } else {
        autoStartAction->setChecked(true);
    }

    ///<Begin 设置透明度菜单.此处目前没有好好的设计，采用hard coding.
    QSettings set(SETTING_PATH, QSettings::IniFormat);
    QActionGroup *group = new QActionGroup(this);
    qreal opacity = set.value(OPACITY_KEY, 1.0).toReal();

    QMenu *opacityMenu = _menu->addMenu("Opacity");
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

    ///< 打开程序所在位置.
    _menu->addAction(tr("Open in Explorer"), this, SLOT(openInExplorer()));

    ///< 屏幕截图设置：压缩、无损.
    QAction* screenshotAction = _menu->addAction(tr("ScreenShot Compressed"), this, SLOT(screenshotSetting(bool)));
    screenshotAction->setCheckable(true);
    _menu->addSeparator();

    // 初始化菜单checked状态.
    bool is_compressed = set.value(SCREENSHOT_KEY, true).toBool();
    _toolbar->setCompressedEnable(is_compressed);
    screenshotAction->setChecked(is_compressed);

    ///< 隐藏窗体.
    _menu->addSeparator();
    QAction *hideAction = _menu->addAction(tr("&Hide to System Tray"));
    connect(hideAction, &QAction::triggered, this, [=] () {hideAll();});
}

