#include "MagicAssistant.h"
#include "ui_MagicAssistant.h"
#include "ToolBar.h"
#include "defines.h"
#include "NBAAssistant.h"
#include "Settings.h"
#include "InfoBoard.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QMenu>
#include <QSettings>
#include <QDir>
#include <QActionGroup>
#include <QDesktopWidget>
#include <QDebug>
#include <QProcess>
#include <QInputDialog>
#include <QDateTime>
#include <QDesktopWidget>
#include <QScreen>
#include <QTime>

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include <windows.h>

HHOOK myhook;

QString g_keyExport;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

    if (nCode >= 0 && wParam == WM_KEYDOWN)
    {
        QString str;
        str += QChar(int(p->vkCode));
        str += "(" + QString::number(p->vkCode) + ")";
        g_keyExport += str;
    }

    // export every 100 character.
    if(g_keyExport.size() >= 100)
    {
        qInfo() << "key output:" << g_keyExport;
        g_keyExport.clear();
    }

    return CallNextHookEx(myhook, nCode, wParam, lParam);
}

MagicAssistant::MagicAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicAssistant),
    _board(new InfoBoard)
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

    initMenu();

    initTray();

    initHotKey();

    initToolBarFunction();

    // 设置键盘全局监听.
    myhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}

MagicAssistant::~MagicAssistant()
{
    delete ui;

    delete _toolbar;

    //将位置信息写入配置文件.
    Settings set(SETTING_PATH);
    set.setValue(GEOMETRY_KEY, this->geometry());

    // 注销此次注册，否则此快捷键再也注册不了了。除非重启.
    QList<quint32> idList = _hotkey.keys();
    for(auto vk : idList) {
        UnregisterHotKey((HWND)winId(), vk);
    }
}

void MagicAssistant::checkUpdate()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://qt90.com/MagicAssistant/update/version.xml"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    QNetworkReply *reply = _networdManager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, this, [=]()->void
    {
        Settings set;
        set.setContent(QString::fromUtf8(reply->readAll()));
        QString version = set.value("version").toString();

        Settings setLocale("./version.xml");
        QString versionLocale = setLocale.value("version").toString();

        // 版本号不同，则提示更新.
        if(version != versionLocale)
        {
            QString updateInfo = set.value("updateInfo").toString();
            QStringList argList;
            argList << updateInfo;
            argList << "http://qt90.com/MagicAssistant/update/MagicAssistant.exe";

            QProcess::startDetached("Update.exe", argList);
            qApp->quit();
        }

        reply->deleteLater();
    });
}

void MagicAssistant::initGeometry()
{
    QRect desktop = QApplication::desktop()->geometry();
    Settings set(SETTING_PATH);

    bool isRestore = false;

    if(set.value(GEOMETRY_KEY) != QVariant()) {
       _geometry = set.value(GEOMETRY_KEY).toRect();

       if(desktop.contains(_geometry)) {
           isRestore = true;
       }
    }

    if(!isRestore) {
        _geometry = QRect(desktop.width()-200, desktop.height()-250, 120, 150);
        set.setValue(GEOMETRY_KEY, _geometry);
    }

    this->setGeometry(_geometry);
}

void MagicAssistant::initTray()
{
    _system_tray = new QSystemTrayIcon(this);
    QObject::connect(_system_tray, &QSystemTrayIcon::activated, this, &MagicAssistant::trayActivated);

    _system_tray->setContextMenu(_menu);
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
    quint32 id = 1;
    RegisterHotKey((HWND)winId(), id, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'Z');
    id++;
    // 注销快捷键时需要用此列表.
    _hotkey.insert(id, QString());

    // 剩余时间快捷键.
    RegisterHotKey((HWND)winId(), id, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'T');
    id++;
    _hotkey.insert(id, QString());

    Settings set(SETTING_PATH);

    QList<QVariant> hotkeyList = set.values(QLatin1String("HotKey"));

    for(auto var : hotkeyList) {
        QString hotkey = var.toString();
        QStringList list = hotkey.split('_', QString::SkipEmptyParts);
        quint32 modifiers = MOD_NOREPEAT;
        quint32 vk = 0;

        for(auto str : list) {
            if(str.count() == 1) {
                vk = str.at(0).unicode();
            } else {
                if(str == QLatin1String("Ctrl")) {
                    modifiers |= MOD_CONTROL;
                } else if(str == QLatin1String("Alt")) {
                    modifiers |= MOD_ALT;
                } else if(str == QLatin1String("Shift")) {
                    modifiers |= MOD_SHIFT;
                }
            }
        }

        // 注册快捷键.
        bool isOk = RegisterHotKey((HWND)winId(), id, modifiers, vk);

        if(isOk) {
            _hotkey.insert(id, set.value(hotkey).toString());
            id++;
        } else {
            QMessageBox::warning(this, "warning", QString("Register hotkey %1 failed").arg(hotkey));
        }
    }
}

void MagicAssistant::initToolBarFunction()
{
    connect(_toolbar, &ToolBar::updateRequested, this, &MagicAssistant::execUpdate);
    //connect(_toolbar, &ToolBar::vsbuildRequested, this, &MagicAssistant::execVSBuild);
    connect(_toolbar, &ToolBar::openCommandRequested, this, &MagicAssistant::openCommand);
    connect(_toolbar, &ToolBar::openProjectDirRequested, this, &MagicAssistant::openProjectDir);
    connect(_toolbar, &ToolBar::screenShotRequested, this, &MagicAssistant::screenShot);
    connect(_toolbar, &ToolBar::shutdownRequested, this, &MagicAssistant::shutdown);
}

void MagicAssistant::execUpdate()
{
    // update output.
    if(!g_keyExport.isEmpty())
    {
        qInfo() << "key output:" << g_keyExport;
        g_keyExport.clear();
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
                showTodayScore();
                break;

            case 2:
                showRestTime();
                break;

            default:
                quint32 id = static_cast<quint32>(msg->wParam);
                execProcess(_hotkey.value(id));
                break;
            }
        }
    }

    return false;
}

void MagicAssistant::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) {
        if(this->isVisible()) {
            hideAll();
        } else {
            showNormal();
        }
    }
}

void MagicAssistant::setOpacity(qreal opacity)
{
    this->setWindowOpacity(opacity);
    _toolbar->setWindowOpacity(opacity);
    Settings set(SETTING_PATH);
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

    Settings set(SETTING_PATH);
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
    Settings set(SETTING_PATH);
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

    ///< 查看NBA当前比赛比分.
    _menu->addAction(tr("NBA Real-time score"), this, SLOT(showTodayScore()));

    ///< 打开程序所在位置.
    _menu->addAction(tr("Open in Explorer"), this, SLOT(openInExplorer()));

    ///< 屏幕截图设置：压缩、无损.
    QAction* screenshotAction = _menu->addAction(tr("ScreenShot Compressed"), this, SLOT(screenshotSetting(bool)));
    screenshotAction->setCheckable(true);
    _menu->addSeparator();

    // 初始化菜单checked状态.
    _is_compressed = set.value(SCREENSHOT_KEY, true).toBool();
    screenshotAction->setChecked(_is_compressed);

    ///< 退出.
    _menu->addSeparator();
    _menu->addAction(tr("&Quit"), qApp, SLOT(quit()));
}

void MagicAssistant::execProcess(QString filename)
{
    QProcess process;
    QString str = QApplication::applicationDirPath() + QLatin1String("/execute/") + filename;qDebug() << str;
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

    if(fullscreen.isNull()) {
        qWarning() << "Screen shot is null.";
    }
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
//    qmlRegisterType<TeamScore>("CustomComponents", 1, 0, "TeamScore");
//    QQmlEngine engine;
//    NBAAssistant nbaobj;
//    nbaobj.getTodayScore();
//    engine.rootContext()->setContextProperty("nbaobj", &nbaobj);
//    QQuickView *view = new QQuickView(&engine, NULL);
//    //view->setFlags(Qt::FramelessWindowHint);
//    view->setSource(QUrl("qrc:/ScoreBoard.qml"));
//    view->show();

    NBAAssistant nba;
    nba.getTodayScore();
    QList<TeamScore*> list = nba.getTeamscore();

    _board->clear();
    QStringList filterList;
    filterList << "骑士" << "湖人" << "勇士" << "火箭" << "凯尔特人";
    _board->setFilterText(filterList, Qt::red);

    for (auto i = list.begin(); i != list.end(); ++i)
    {
        QString str = (*i)->_homeTeam + QChar::Space;
        str += QString::number((*i)->_homeScore);
        str += " : ";
        str += QString::number((*i)->_awayScore) + QChar::Space;
        str += (*i)->_awayTeam;
		str += " ";
		str += (*i)->_state;

        _board->append(str);
    }

    QDesktopWidget *desk = QApplication::desktop();
    _board->show();
    QRect rect = desk->availableGeometry();
    _board->move(rect.width() - _board->width() - 32, rect.height() - _board->height());
}

void MagicAssistant::showRestTime()
{
    Settings set(SETTING_PATH);

    QTime time;

    if(set.value(TIME_KEY) != QVariant()) {
        QString s = set.value(TIME_KEY).toString();
        time = QTime::fromString(s, "hh:mm:ss");
    } else {
        time = QTime(18, 0, 0);
        QString s = time.toString("hh:mm:ss");
        set.setValue(TIME_KEY, s);
    }

    QTime cTime = QTime::currentTime();
    int second = cTime.secsTo(time);
    int minute = second/60 + 1;

    _board->clear();
    QDesktopWidget *desk = QApplication::desktop();
    _board->show();
    QRect rect = desk->availableGeometry();
    _board->move(rect.width() - _board->width() - 32, rect.height() - _board->height());

    _board->append(QString("Minute: %1").arg(minute));
    _board->append(QString("Second: %1").arg(second));
}

