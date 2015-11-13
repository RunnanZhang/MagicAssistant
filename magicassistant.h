#ifndef MAGICASSISTANT_H
#define MAGICASSISTANT_H

#include <QWidget>
#include <QSystemTrayIcon>

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
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

    //设置开机自动启动.
    void autoStart(bool is_start);

private:
    ///< \note 初始化托盘相关.
    void initializeTray();

    ///< \note 初始化位置.
    void initializeGeometry();

    ///< \note 设置透明度.
    void setOpacity(qreal opacity);

private:
    Ui::MagicAssistant *ui;

    ///< \note 用于绘制背景图片.
    QPixmap _pixmap;

    ///< \note 鼠标press时记录，用于移动窗体时使用.
    QPoint _start_pos;

    ///< \note 自定义工具栏.
    ToolBar *_tool_bar;

    ///< \note 系统托盘图标.
    QSystemTrayIcon *_system_tray;

    ///< \note 用于记录当前geometry，关闭时写入配置文件供下次启动加载.
    QRect _geometry;
};

#endif // MAGICASSISTANT_H
