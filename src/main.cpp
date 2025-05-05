#include "game.h"
#include "close_handler.h"

#include <qfile.h>
#include <qapplication.h>
#include <qcommandlineparser.h>

#include "application.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;

    QCommandLineOption helpOption(QStringList() << "help", "shows this help");
    QCommandLineOption editorOption(QStringList() << "editor", "run editor");
    
    parser.addOption(helpOption);
    parser.addOption(editorOption);
    parser.process(app);

    if (parser.isSet(helpOption)) {
        parser.showHelp();
    }

    bool editorMode = false;
#ifdef QT_DEBUG
    if (parser.isSet(editorOption)) {
        editorMode = true;
    }
#endif // QT_DEBUG
    
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#ifdef QT_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(format);
    
    // game w;
    // CloseHandler h;
    // w.installEventFilter(&h);

    // w.show();


    Application engine(editorMode);
    engine.show();

    return app.exec();
}
