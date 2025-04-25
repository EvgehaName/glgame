#include "widget.h"
#include "close_handler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#ifdef QT_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(format);
    
    Widget w;    
    CloseHandler h;
    w.installEventFilter(&h);

    w.show();
    return a.exec();
}
