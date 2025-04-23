#ifndef UIAMMO_H
#define UIAMMO_H

#include <QWidget>

namespace Ui {
class UIAmmo;
}

class UIAmmo : public QWidget
{
    Q_OBJECT

public:
    explicit UIAmmo(QWidget *parent = nullptr);
    ~UIAmmo();

private:
    Ui::UIAmmo *ui;
};

#endif // UIAMMO_H
