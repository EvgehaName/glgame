#include "uistaminabar.h"
#include "ui_uistaminabar.h"

UIStaminaBar::UIStaminaBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UIStaminaBar)
{
    ui->setupUi(this);
}

UIStaminaBar::~UIStaminaBar()
{
    delete ui;
}
