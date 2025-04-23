#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QWidget>

namespace Ui {
class UISettings;
}

class UISettings : public QWidget
{
    Q_OBJECT

public:
    explicit UISettings(QWidget *parent = nullptr);
    ~UISettings();

private slots:
    void on_closeBtn_clicked();

    void on_applyBtn_clicked();

private:
    Ui::UISettings *ui;
};

#endif // UISETTINGS_H
