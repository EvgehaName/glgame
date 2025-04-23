#include "uideathmatch.h"
#include "ui_uideathmatch.h"

UIDeathmatch::UIDeathmatch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UIDeathmatch)
{
    ui->setupUi(this);
}

UIDeathmatch::~UIDeathmatch()
{
    delete ui;
}
