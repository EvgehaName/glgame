#pragma once

#include <qobject.h>
#include <qevent.h>

class CloseHandler : public QObject
{
	Q_OBJECT
public:
    CloseHandler();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    bool m_flag;
};
