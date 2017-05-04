#ifndef MAGICASSISTANT_H
#define MAGICASSISTANT_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMap>

namespace Ui {
class MagicAssistant;
}

class ToolBar;
class QSystemTrayIcon;

class MagicAssistant : public QWidget
{
    Q_OBJECT

public:
    explicit MagicAssistant(QWidget *parent = 0);
    ~MagicAssistant();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

    ///< \note 设置开机自动启动.
    void autoStart(bool is_start);

    ///< \note 用于定时器刷新状态，toolbar是否显示.
    void updateState();

    ///< \note 打开程序所在位置.
    void openInExplorer();

    ///< \note 屏幕截图设置.是否压缩图片.
    void screenshotSetting(bool is_compressed);

    ///< \note 执行启动程序.
    void execProcess(QString path);

    ///< \note 打开Cmd.
    void openCommand();

    ///< \note 查询当天比分.
    void showTodayScore();

    ///< \note 查询剩余时间.
    void showRestTime();

    ///< \note 打开项目工程目录.
    void openProjectDir();

    ///< \note 全屏截图.
    void screenShot();

    ///< \note 定时关机.
    void shutdown();

    ///< \note 点击更新按钮.
    void execUpdate();

private:
    ///< \note 初始化托盘相关.
    void initTray();

    ///< \note 初始化位置.
    void initGeometry();

    ///< \note 初始化菜单.
    void initMenu();

    ///< \note 初始化全局热键.
    void initHotKey();

    ///< \note toolbar功能槽函数的连接.
    void initToolBarFunction();

    ///< \note 显示toolbar.
    void showToolBar();

    ///< \note 隐藏所有窗体.
    void hideAll();

    ///< \note 显示所有窗体.
    void showAll();

    ///< \note 设置透明度.
    void setOpacity(qreal opacity);

private:
    Ui::MagicAssistant *ui;

    ///< \note 用于绘制背景图片.
    QPixmap _pixmap;

    ///< \note 鼠标press时记录，用于移动窗体时使用.
    QPoint _start_pos;

    ///< \note 自定义工具栏.
    ToolBar *_toolbar;

    ///< \note 系统托盘图标.
    QSystemTrayIcon *_system_tray;

    ///< \note 用于记录当前geometry，关闭时写入配置文件供下次启动加载.
    QRect _geometry;

    ///< \note 鼠标进入窗体时打开的定时器，用于检测鼠标位置，移出有效范围内则停止，并隐藏toolbar.
    QTimer *_timer;

    ///< \note 右键菜单.
    QMenu *_menu;

    ///< \note 屏幕截图是否压缩.
    bool _is_compressed;

    /// @brief quint32 vk id. QString 此快捷键要执行程序的名字.
    QMap<quint32, QString> _hotkey;
};

#endif // MAGICASSISTANT_H
