#ifndef UIDEATHMATCH_H
#define UIDEATHMATCH_H

#include <QWidget>

namespace Ui {
class UIDeathmatch;
}

class UIDeathmatch : public QWidget
{
    Q_OBJECT

public:
    explicit UIDeathmatch(QWidget *parent = nullptr);
    ~UIDeathmatch();

private:
    Ui::UIDeathmatch *ui;
};

#endif // UIDEATHMATCH_H
