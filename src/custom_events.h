#pragma once
#include <qevent.h>

namespace custom_events {
    enum EventType {
        Quit = QEvent::User + 1
    };
}
