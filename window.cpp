#include "window.h"

Window::Window()
{
    setSurfaceType(QSurface::VulkanSurface); // or OpenGLSurface

    connect(&timer, &QTimer::timeout, this, &Window::renderLoop);
    timer.start(16); // ~60 FPS
}

void Window::exposeEvent(QExposeEvent*)
{
    if (isExposed())
        renderer.initialize(this);
}

void Window::renderLoop()
{
    controller.processInput(camera);
    renderer.render(camera);
}

void Window::keyPressEvent(QKeyEvent* e)
{
    controller.onKey(e);
}
