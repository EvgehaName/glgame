#include "options_window.h"
#include "ui_options_window.h"
#include "general_options_widget.h"

OptionsWindow::OptionsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    ui->treeWidget->topLevelItem(0)->setSelected(true);
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}

void OptionsWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString category = current->text(0);

    if (category == "General") {
        ui->optionsLayout->insertWidget(1, new GeneralOptionsWidget());
    }
}

