#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

namespace Ui {
class ToolBar;
}

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = 0);
    ~ToolBar();

public slots:
    ///< \note 申请隐藏，假如焦点不在窗体上.
    void applyHide();

    ///< \note 设置焦点标志变量_is_display.
    void setDisplayMark(bool is_display);

private slots:
    void on_toolButton_clicked();
    void on_toolButton2_clicked();
    void on_pOpenBtn_clicked();

    ///< \note 打开命令行.
    void on_pCommandBtn_clicked();

    ///< \note 屏幕截图.
    void on_pScreenshotBtn_clicked();

    ///< \note 定时关机.
    void on_pShutdownBtn_clicked();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::ToolBar *ui;

    bool _is_display;
};

#endif // TOOLBAR_H
