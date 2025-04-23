#ifndef UISTAMINABAR_H
#define UISTAMINABAR_H

#include <QWidget>

namespace Ui {
class UIStaminaBar;
}

class UIStaminaBar : public QWidget
{
    Q_OBJECT

public:
    explicit UIStaminaBar(QWidget *parent = nullptr);
    ~UIStaminaBar();

private:
    Ui::UIStaminaBar *ui;
};

#endif // UISTAMINABAR_H
