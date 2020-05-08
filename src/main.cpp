#include "app/main_window.hpp"

#include <QGuiApplication>

int main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName("StoryTeller");
    app::MainWindow mw;
    return app.exec();
}
