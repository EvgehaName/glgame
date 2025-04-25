#include "uihealthbar.h"
#include "ui_uihealthbar.h"

UIHealthBar::UIHealthBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UIHealthBar)
{
    ui->setupUi(this);
}

UIHealthBar::~UIHealthBar()
{
    delete ui;
}
