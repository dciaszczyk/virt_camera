#include "window.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    window w;
    w.show();
    return a.exec();
}
