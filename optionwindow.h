#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QWidget>

namespace Ui {
class OptionWindow;
}

class OptionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow();

private:
    Ui::OptionWindow *ui;
};

#endif // OPTIONWINDOW_H
