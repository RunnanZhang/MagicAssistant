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

signals:
    ///< \note 为了不让本程序出现在截图中，截图时先隐藏所有窗体，之后在显示.
    void hideRequested();
    void showRequested();

    ///< ToolButton功能信号，实现在主窗口中实现.
    void updateMetaDataRequested();
    void vsbuildRequested();
    void openCommandRequested();
    void openProjectDirRequested();
    void screenShotRequested();
    void shutdownRequested();

private slots:
    void on_toolButton_clicked();
    void on_toolButton2_clicked();
    void on_pOpenBtn_clicked();

    ///< \note 打开Command.
    void on_pCommandBtn_clicked();

    ///< \note 屏幕截图.
    void on_pScreenshotBtn_clicked();

    ///< \note 定时关机.
    void on_pShutdownBtn_clicked();

private:
    Ui::ToolBar *ui;
};

#endif // TOOLBAR_H
