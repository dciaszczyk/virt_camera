#include <QApplication>
#include <QSurfaceFormat>
#include "window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Window w;
    w.show();

    w.resize(800,600);

    return app.exec();
}
