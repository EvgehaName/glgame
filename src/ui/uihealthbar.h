#ifndef UIHEALTHBAR_H
#define UIHEALTHBAR_H

#include <QWidget>

namespace Ui {
class UIHealthBar;
}

class UIHealthBar : public QWidget
{
    Q_OBJECT

public:
    explicit UIHealthBar(QWidget *parent = nullptr);
    ~UIHealthBar();

private:
    Ui::UIHealthBar *ui;
};

#endif // UIHEALTHBAR_H
