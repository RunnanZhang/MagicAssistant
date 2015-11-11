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

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::ToolBar *ui;

    bool _is_display;
};

#endif // TOOLBAR_H
