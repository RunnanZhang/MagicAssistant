#include "magicassistant.h"
#include "ui_magicassistant.h"
#include "toolbar.h"
#include "defines.h"
#include "nbaassistant.h"

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
#include <QInputDialog>
#include <QDateTime>
#include <QDesktopWidget>
#include <QScreen>

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include <windows.h>

MagicAssistant::MagicAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicAssistant)
{
    ui->setupUi(this);

    _pixmap.load(":/images/james.png");

    _toolbar = new ToolBar;

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacAlwaysShowToolWindow);
#endif

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateState()));

    initGeometry();

    initTray();

    initMenu();

    initHotKey();

    initToolBarFunction();
}

MagicAssistant::~MagicAssistant()
{
    delete ui;

    delete _toolbar;

    //将位置信息写入配置文件.
    QSettings regedit(SETTING_PATH, QSettings::IniFormat);
    regedit.setValue(GEOMETRY_KEY, this->geometry());

    // 注销此次注册，否则此快捷键再也注册不了了。除非重启.
    UnregisterHotKey((HWND)winId(), 1);
    UnregisterHotKey((HWND)winId(), 2);
    UnregisterHotKey((HWND)winId(), 3);
}

void MagicAssistant::initGeometry()
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

void MagicAssistant::initTray()
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

	// 先屏蔽启动弹窗功能.每次启动弹窗太烦人.
    /*if (_system_tray->isVisible()) {
        _system_tray->showMessage(tr("Information"), tr("Magic Assistant stay in system tray"));
    }*/
}

void MagicAssistant::initHotKey()
{
    // 注册快捷键.
    RegisterHotKey((HWND)winId(), 1, MOD_CONTROL | MOD_ALT |MOD_NOREPEAT, 'M');
    RegisterHotKey((HWND)winId(), 2, MOD_CONTROL | MOD_ALT |MOD_NOREPEAT, 'N');
    RegisterHotKey((HWND)winId(), 3, MOD_CONTROL | MOD_ALT |MOD_NOREPEAT, 'Z');
}

void MagicAssistant::initToolBarFunction()
{
    connect(_toolbar, &ToolBar::updateMetaDataRequested, this, &MagicAssistant::updateMetaData);
    connect(_toolbar, &ToolBar::vsbuildRequested, this, &MagicAssistant::execVSBuild);
    connect(_toolbar, &ToolBar::openCommandRequested, this, &MagicAssistant::openCommand);
    connect(_toolbar, &ToolBar::openProjectDirRequested, this, &MagicAssistant::openProjectDir);
    connect(_toolbar, &ToolBar::screenShotRequested, this, &MagicAssistant::screenShot);
    connect(_toolbar, &ToolBar::shutdownRequested, this, &MagicAssistant::shutdown);
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

bool MagicAssistant::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);
    // windows系统下,message类型才为MSG.
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);

        if(msg->message == WM_HOTKEY)
        {
            switch(msg->wParam)
            {
            case 1:
                updateMetaData();
                break;

            case 2:
                execVSBuild();
                break;

            case 3:
                showTodayScore();
                break;
            }
        }
    }
    return false;
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
    _is_compressed = is_compressed;

    QSettings set(SETTING_PATH, QSettings::IniFormat);
    set.setValue(SCREENSHOT_KEY, is_compressed);
}

void MagicAssistant::initMenu()
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
    _is_compressed = set.value(SCREENSHOT_KEY, true).toBool();
    screenshotAction->setChecked(_is_compressed);

    ///< 隐藏窗体.
    _menu->addSeparator();
    QAction *hideAction = _menu->addAction(tr("&Hide to System Tray"));
    connect(hideAction, &QAction::triggered, this, [=] () {hideAll();});
}

void MagicAssistant::updateMetaData()
{
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/update.bat";
    process.startDetached(str);
}

void MagicAssistant::execVSBuild()
{
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/vsbuild.bat";
    process.startDetached(str);
}

void MagicAssistant::openCommand()
{
    QProcess::startDetached("cmd");
}

void MagicAssistant::openProjectDir()
{
    // start是cmd内部命令，必须这么调用，/c是执行完关闭cmd的参数.具体可查阅cmd的帮助(cmd /?).
    //QProcess::startDetached("cmd /c start C:/Workspace/OrigineBeta/Builds/Win/Origine/bin");

    //为了可编辑，将此代码也移动到外部的bat中.
    QProcess process;
    QString str = QApplication::applicationDirPath() + "/execute/open.bat";
    process.startDetached(str);
}

void MagicAssistant::screenShot()
{
    // 先隐藏所有窗体.
    hideAll();

    // QScreen没有构造函数，必须通过此获取.
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
    showAll();
}

void MagicAssistant::shutdown()
{
    bool ok;
    int time = QInputDialog::getInt(_toolbar, tr("Auto shutdown setting"), tr("Time(ms):"),
                                    0, 0, 2147483647, 1, &ok);

    if(ok) {
        //由于shutdown命令,-t后面的参数只要大于0，默认就会执行-f参数，-f是强制关闭程序，所以此句会导致程序退出...
        QProcess::startDetached(QString("shutdown -s -t %1").arg(time));
    } else {
        //QProcess::startDetached("shutdown -a");
    }
}

void MagicAssistant::showTodayScore()
{
    qmlRegisterType<TeamScore>("CustomComponents", 1, 0, "TeamScore");
    QQmlEngine engine;
    NBAAssistant nbaobj;
    nbaobj.getTodayScore();
    engine.rootContext()->setContextProperty("nbaobj", &nbaobj);
    QQuickView *view = new QQuickView(&engine, NULL);
    //view->setFlags(Qt::FramelessWindowHint);
    view->setSource(QUrl("qrc:/ScoreBoard.qml"));
    view->show();

//    //NBAAssistant nba;
//    QList<TeamScore> list;
//    //nba.getTodayScore(list);
//	QString showMessage;
//	for (auto i = list.begin(); i != list.end(); ++i)
//	{
//		showMessage += (*i).homeTeam + QString::number((*i).homeScore)
//			+ " : " + (*i).awayTeam + QString::number((*i).awayScore);
//	}
//	_system_tray->showMessage(tr("Information"), showMessage);
}

