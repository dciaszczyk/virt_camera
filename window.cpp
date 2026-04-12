#include "window.h"
#include "GLWidget.h"

Window::Window()
{
    setCentralWidget(new GLWidget());
}
