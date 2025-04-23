#include "uiammo.h"
#include "ui_uiammo.h"

UIAmmo::UIAmmo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UIAmmo)
{
    ui->setupUi(this);
}

UIAmmo::~UIAmmo()
{
    delete ui;
}
