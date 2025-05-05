#ifndef OPTIONS_WINDOW_H
#define OPTIONS_WINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = nullptr);
    ~OptionsWindow();

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::OptionsWindow *ui;
};

#endif // OPTIONS_WINDOW_H
