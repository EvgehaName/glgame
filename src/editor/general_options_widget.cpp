#include "general_options_widget.h"
#include "ui_general_options_widget.h"

GeneralOptionsWidget::GeneralOptionsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneralOptionsWidget)
{
    ui->setupUi(this);
}

GeneralOptionsWidget::~GeneralOptionsWidget()
{
    delete ui;
}
