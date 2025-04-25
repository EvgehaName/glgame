#include "close_handler.h"
#include "custom_events.h"

CloseHandler::CloseHandler()
    : m_flag(false) {}

bool CloseHandler::eventFilter(QObject *watched, QEvent *event)
{
    if (watched->isWidgetType())
    {
        switch(event->type())
        {
            case QEvent::Close:
            {
                if (m_flag) {
                    event->ignore();
                }

                break;
            }

            case custom_events::Quit:
            {
                m_flag = false;
                break;
            }

            case QEvent::KeyPress:
            {
                QKeyEvent* keyEvent = (QKeyEvent*)event;
                if (keyEvent->key() == Qt::Key_F4 || keyEvent->modifiers() == Qt::AltModifier) {
                    m_flag = true;
                }
                break;
            }

            default: break;
        }
    }

    return QObject::eventFilter(watched, event);
}

