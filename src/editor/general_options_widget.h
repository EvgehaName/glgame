#ifndef GENERAL_OPTIONS_WIDGET_H
#define GENERAL_OPTIONS_WIDGET_H

#include <QWidget>

namespace Ui {
class GeneralOptionsWidget;
}

class GeneralOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralOptionsWidget(QWidget *parent = nullptr);
    ~GeneralOptionsWidget();

private:
    Ui::GeneralOptionsWidget *ui;
};

#endif // GENERAL_OPTIONS_WIDGET_H
