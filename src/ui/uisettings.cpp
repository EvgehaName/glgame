#include "uisettings.h"
#include "ui_uisettings.h"

UISettings::UISettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UISettings)
{
    ui->setupUi(this);
}

UISettings::~UISettings()
{
    delete ui;
}

void UISettings::on_closeBtn_clicked()
{
    close();
}


void UISettings::on_applyBtn_clicked()
{

}

